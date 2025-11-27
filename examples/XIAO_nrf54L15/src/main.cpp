/*
  A ultra low power Home Assistant/BtHome temperature/humidity/pressure sensor device with XIAO nrf54L15
  Average current consumption is as low as 12µA! Can be even more reduced with longer sleep time.
  Current during LowPower.deepSleep() would be less, but waking up from deep sleep needs a lot of energy. 
  So LowPower.sleep() is the better choice for this sensor with TIME_TO_SLEEP=15s.
  At 12µA, the sensor should run for more than 3 years on a 350mAh LiPo battery.
 */

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(SolarSensor54l15, LOG_LEVEL_INF);

#define USE_EXT_ANT			0
#define USE_BME280			1
#define USE_ENCRYPTION		1

#define TIME_TO_SLEEP		15
#define TIME_TO_SLEEP_LOW	(60*5)

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>

#include <zephyr/drivers/regulator.h>
#include <zephyr/drivers/gpio.h>

#include <zephyr/device.h>
#include <zephyr/pm/device.h>
#include <zephyr/pm/device_runtime.h>

#include <zephyr/sys/poweroff.h>
#include <zephyr/drivers/timer/nrf_grtc_timer.h>
#include <zephyr/drivers/retained_mem.h>

#include "adc.h"
#include "bme280.h"
#include "wdt.h"
#include "../../src/BtHomeV2Device.h"
#include <string>

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
 !DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
	#error "No suitable devicetree overlay specified"
#endif

/****************************************** BTHOME KEY **********************************************/
#if USE_ENCRYPTION
//  91CDEC4192407AEC3F6FCF04AAB6F7A4
static const uint8_t bindKey[16] = {
	0x91, 0xCD, 0xEC, 0x41, 0x92, 0x40, 0x7A, 0xEC, 
	0x3F, 0x6F, 0xCF, 0x04, 0xAA, 0xB6, 0xF7, 0xA4 
};
#endif

/****************************************** WDT **********************************************/
#define WDT_MAX_WINDOW  ((TIME_TO_SLEEP_LOW + 60)*1000)
#define WDT_MIN_WINDOW  0U
#define WDT_OPT WDT_OPT_PAUSE_HALTED_BY_DBG

/****************************************** DEVICES **********************************************/
static const struct device *const vbat_reg = DEVICE_DT_GET(DT_NODELABEL(vbat_pwr));
static const struct device *const rfsw_pwr_reg = DEVICE_DT_GET(DT_NODELABEL(rfsw_pwr));
#if USE_EXT_ANT	
static const struct device *const rfsw_ctl_reg = DEVICE_DT_GET(DT_NODELABEL(rfsw_ctl));
#endif
static const struct device *const i2c22_dev = DEVICE_DT_GET(DT_NODELABEL(i2c22));
static const struct device *const i2c30_dev = DEVICE_DT_GET(DT_NODELABEL(i2c30));
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

/****************************************** BLINK_CODE **********************************************/
void blinkCode(int code)
{
	while(true) {
		for (int i = 0; i < code; i++) {
			gpio_pin_set_dt(&led, 0);
			k_sleep(K_MSEC(100));
			gpio_pin_set_dt(&led, 1);
			k_sleep(K_MSEC(300));
		}
		k_sleep(K_MSEC(1000));
	}
}

/****************************************** ADVERTISING **********************************************/
#define ADV_PARAM BT_LE_ADV_PARAM(BT_LE_ADV_OPT_USE_IDENTITY, \
				  32, \
				  33, NULL)

/****************************************** BTHome Header **********************************************/
static struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR), // 3 bytes { len 0x02 type 0x01 data 0x06 }
	BT_DATA(BT_DATA_SVC_DATA16, NULL, 0)
};

int advertize(BtHomeV2Device *pDev)
{
	uint8_t advertisementData[MAX_ADVERTISEMENT_SIZE];
	uint8_t size = pDev->getAdvertisementData(advertisementData);
	// skip first 5 bytes, already in ad struct:
	// 1 length=(2)
	// 2 BT_DATA_FLAGS 
	// 3 BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR
	// 4 length=X 
	// 5 BT_DATA_SVC_DATA16
	ad[1].data_len = size - 5;
	ad[1].data = &advertisementData[5];

	/* Start advertising for 5+95ms */
	int err = bt_le_adv_start(ADV_PARAM, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		LOG_ERR("Advertising failed to start (err %d)\n", err);
		return 0;
	}
	/* Flash led for 5ms */
	err = gpio_pin_set_dt(&led, 0);
	if (err < 0) {
		LOG_ERR("gpio_pin_set_dt failed (err %d)\n", err);
	}		
	k_sleep(K_MSEC(5));
	err = gpio_pin_set_dt(&led, 1);
	if (err < 0) {
		LOG_ERR("gpio_pin_set_dt failed (err %d)\n", err);
	}		
	k_sleep(K_MSEC(95));
	err = bt_le_adv_stop();
	if (err) {
		LOG_ERR("Advertising failed to stop (err %d)\n", err);
		return -1;
	}
	return 0;
}

/****************************************** MAIN **********************************************/

int main(void)
{
	int err;
	uint32_t counter = 0;

	LOG_INF("Starting BTHome sensor template\n");

	// Initialize watchdog
	int32_t wdt_channel_id = init_wdt(WDT_MIN_WINDOW, WDT_MAX_WINDOW, WDT_OPT);
	if (wdt_channel_id < 0) {
		return 0;
	}

	err = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (err < 0) {
		LOG_ERR("LED GPIO configure failed %d\n", err);
  		return 0;
	}	
	err = gpio_pin_set_dt(&led, 0);
	if (err < 0) {
		LOG_ERR("gpio_pin_set_dt failed (err %d)\n", err);
	}
	// Suspend i2c30: save 145µA while sleeping
	err = pm_device_action_run(i2c30_dev, PM_DEVICE_ACTION_SUSPEND);
	if (err) {
		LOG_ERR("Suspend %s failed %d\n", i2c30_dev->name, err);
	}
	if (!gpio_is_ready_dt(&led)) {
		LOG_ERR("LED GPIO not ready\n");
  		return 0;
	}
	// ADC channel 0 for solar current
	err = adcSetup(0);
	if (err < 0) {
		return 0;
	}
	// ADC channel 7 for battery voltage
	err = adcSetup(7);
	if (err < 0) {
		return 0;
	}
	err = device_init(i2c22_dev);	
	if (err) {
		LOG_ERR("\nDevice %s init failed %d\n", i2c22_dev->name, err);
		return 0;
	}
#if USE_BME280	
	int err_bme280 = bme280_init();	
	if (err_bme280) {
		blinkCode(2);
		return err_bme280;
	}
#endif	
	err = pm_device_action_run(i2c22_dev, PM_DEVICE_ACTION_SUSPEND);
	if (err) {
		LOG_ERR("Suspend %s failed %d\n", i2c22_dev->name, err);
	}
#if USE_EXT_ANT	
	regulator_disable(rfsw_ctl_reg);	
#endif

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(NULL);
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)\n", err);
		return 0;
	}
	LOG_INF("Bluetooth initialized\n");

#if USE_ENCRYPTION
	bt_le_oob oob;
	bt_le_oob_get_local(BT_ID_DEFAULT, &oob);
	// Leave short and complete name empty, device will be identified by MAC address
	BtHomeV2Device dev("", "", false, bindKey, oob.addr.a.val);
#else
	BtHomeV2Device dev("", "", true);
#endif		

	while(1) {
		feed_wdt(wdt_channel_id);

#if USE_BME280	
		err = pm_device_action_run(i2c22_dev, PM_DEVICE_ACTION_RESUME);
		if (err != 0) {
			LOG_ERR("%s: pm_device_action_run() RESUME failed: %d\n", i2c22_dev->name, err);
		}
		float temp = 0, hum = 0, press = 0;
		err = bme280_read(&temp, &hum, &press);
		if (err != 0) {
			return err;
		}
		// Suspend i2c22: save 100µA while sleeping
		err = pm_device_action_run(i2c22_dev, PM_DEVICE_ACTION_SUSPEND);
		if (err != 0) {
			LOG_ERR("%s: pm_device_action_run() SUSPEND failed: %d\n", i2c22_dev->name, err);
		}		
#endif		
		int32_t val_mv;
		regulator_enable(vbat_reg);	
		// Wait 5ms for voltage divider to settle
		k_sleep(K_MSEC(5));		
		// Read battery voltage on channel 7 (pin AIN7/P1.14)
		adcRead(&val_mv, NULL, 7);
		// Disable battery voltage divider switch supply: save 250µA while sleeping
	 	regulator_disable(vbat_reg);
		// Voltage divider divides by 2
		val_mv *= 2;

		int32_t raw;
		// Read solar current on channel 0 (differential mode, pins AIN0/P1.04 and AIN1/P1.05)
		adcRead(NULL, &raw, 0);
		// shunt 10R, 14bits resolution, internal reference (0.9V), gain 2, oversampling 8
		int32_t current_ua = raw*90000/16383;

		counter++;

		// Enable antenna switch supply
		regulator_enable(rfsw_pwr_reg);	

		// Collect data for advertising packet 1
		dev.resetMeasurement();

		// Available bytes with encryption: 11, else 23
		// 11
		if (!dev.addCount_0_4294967295(counter)) {
			LOG_ERR("addCount_0_4294967295 failed");
		}
		// 6
		if (!dev.addVoltage_0_to_65_resolution_0_001(val_mv*0.001f)) {
			LOG_ERR("addVoltage_0_to_65_resolution_0_001 failed");
		}
		// 3
		if (!dev.addTemperature_neg327_to_327_Resolution_0_01(temp)) {
			LOG_ERR("addTemperature_neg327_to_327_Resolution_0_01 failed");
		}
		// 0
		// Advertize packet 1
		advertize(&dev);

		// Collect data for advertising packet 2
		dev.resetMeasurement();
		// 11
		if (bme280_chipId() == BME280_CHIP_ID) {
			if (!dev.addHumidityPercent_Resolution_0_01(hum)) {
				LOG_ERR("addHumidityPercent_Resolution_0_01 failed");
			}
		}
		// 7
		if (!dev.addCurrentAmps_neg32_to_32_Resolution_0_001(current_ua*0.001f)) {
			LOG_ERR("addCurrentAmps_neg32_to_32_Resolution_0_001 failed");
		}
		// 4
		if (!dev.addPressureHpa(press)) {
			LOG_ERR("addPressureHpa failed");
		}
		// 0
		// Advertize packet 2
		advertize(&dev);

		// Disable antenna switch supply: save 67µA while sleeping
		regulator_disable(rfsw_pwr_reg);
		if (val_mv < 3500) {
			k_sleep(K_MSEC(TIME_TO_SLEEP_LOW*1000));
		} else {
			k_sleep(K_MSEC(TIME_TO_SLEEP*1000));
		}
	}
	return 0;
}