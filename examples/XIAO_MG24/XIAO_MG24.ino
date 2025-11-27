/*
  A low power BtHome temperature/humidity/pressure sensor device with XIAO MG24
  Average current consumption is as low as 60µA! Can be even more reduced with longer sleep time.
  Current during LowPower.sleep() is ~8µA including sensor (measured with BMP280, 3.3V version without level shifter). 
  Current during LowPower.deepSleep() would be less, but waking up from deep sleep needs a lot of energy. 
  So LowPower.sleep() is the better choice for this sensor with TIME_TO_SLEEP=15s.
  At 60µA (with built on voltage divider), the sensor should run for more than 10 months on a 350mAh LiPo battery.
 */

#ifndef ARDUINO_SILABS_STACK_BLE_SILABS
  #error "This example is only compatible with the Silicon Labs BLE stack. Please select 'BLE (Silabs)' in 'Tools > Protocol stack'."
#endif

#include <BtHomeV2Device.h>
#include <ArduinoLowPower.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
// modified adc module to gain access to MG24 features like oversampling, gain or differential mode
#include "adc_mod.h"

//-----------------------------------------------------------------------------
// Configuration switches

#define CONF_EXTERNAL_ANTENNA     0
#define CONF_USE_ENCRYPTION       1
#define CONF_SERIAL_DBG           0
#define CONF_BMP280               1
#define CONF_BUILTIN_BATT_VOLTAGE 0

//-----------------------------------------------------------------------------
#if CONF_BMP280
#include <Adafruit_BMP280.h>
#else
#include <Adafruit_BME280.h>
#endif

//-----------------------------------------------------------------------------
// Defines

#define ADVERTISING_DURATION_MS   100

#define TIME_TO_SLEEP       15          /* Time to sleep (in seconds) */
#define TIME_TO_SLEEP_LOW   (30*60)     /* Time to sleep when voltage is low (in seconds) */

#define LOW_VOLTAGE         3.5         /* Threshold for longer sleep time */

// I prefer not to transfer names
// When using encryption, space is limited
// BTHome will use part of MAC address for identification, that's fine for me
#define BTHOME_SHORT_NAME ""
#define BTHOME_COMPLETE_NAME ""

#if !CONF_BUILTIN_BATT_VOLTAGE
#define BATT_PIN        PC0
#define R1              6.2e6f
#define R2              1.5e6f
#endif

// XIAO MG24 pins
// on board antenna switch
#define RF_SW_PWR_PIN   PB5
#define RF_SW_SEL_PIN   PB4

// on board Flash SPI_1 pins
#define CS1 PA6   // (21)
#define CLK1 PA0  // (17), D17
#define MOSI1 PB0 // (15), D15
#define MISO1 PB1 // (16), D16

// on board peripherals pins
#define VBAT_EN PD3 // (25)

// Flash commands
#define READ_DATA 0x03
#define WRITE_ENABLE 0x06
#define PAGE_PROGRAM 0x02
#define SECTOR_ERASE 0x20

//-----------------------------------------------------------------------------
// Global variables

#if CONF_BMP280
// Bosch BMP280 connected via I2C
Adafruit_BMP280 bmx;
#else
// Bosch BME280 connected via I2C
Adafruit_BME280 bmx;
#endif

#if CONF_USE_ENCRYPTION    
// Bind key: aab3147d9822c05fe14a0c3b77d68e55
const uint8_t key[16] = {
  0xAA, 0xB3, 0x14, 0x7D, 0x98, 0x22, 0xC0, 0x5F,
  0xE1, 0x4A, 0x0C, 0x3B, 0x77, 0xD6, 0x8E, 0x55
};
bd_addr bleAddress;
#endif

bool bleReady = false;

BtHomeV2Device *pBtHomeV2Device;
uint32_t counter = 0;
float temperature = 0;
float airpressure = 0;
#if !CONF_BMP280
float humidity = 0;
#endif
float voltage = 0;

//-----------------------------------------------------------------------------

void setup()
{  
#if CONF_SERIAL_DBG
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
#endif  

  // XIAO Settings
  // On board flash
  pinMode(CS1, OUTPUT);
  pinMode(CLK1, OUTPUT);
  pinMode(MOSI1, OUTPUT);
  pinMode(MISO1, INPUT);
  digitalWrite(CS1, HIGH); // CS1 HIGH

  // Flash Deep Power Down
  writeEnable();
  digitalWrite(CS1, LOW);
  sendSPI(0xB9);
  digitalWrite(CS1, HIGH);

  // Disable JTAG debug interface, see ERF32xG24 reference manual page 903
  // reduces current while sleeping from 82µA to 7.6µA (!)
  GPIO->DBGROUTEPEN = 0;

  // Switch on board LED on
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  if (!setup_bmx(true)) {
#if CONF_SERIAL_DBG
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
#endif    
  }

  // Switch on board LED off
  digitalWrite(LED_BUILTIN, HIGH);

  // Turn antenna switch supply off
  pinMode(RF_SW_PWR_PIN, OUTPUT);  
  digitalWrite(RF_SW_PWR_PIN, LOW);
  // Select chip antenna for now
  pinMode(RF_SW_SEL_PIN, OUTPUT);
  digitalWrite(RF_SW_SEL_PIN, LOW);

  // Output for supply voltage divider
  pinMode(VBAT_EN, OUTPUT);
  digitalWrite(VBAT_EN, LOW);

  // Wait until BLE subsystem is ready
  while (!bleReady) {
    LowPower.sleep(10);
  }
#if CONF_USE_ENCRYPTION
  pBtHomeV2Device = new BtHomeV2Device(BTHOME_SHORT_NAME, BTHOME_COMPLETE_NAME, false, key, bleAddress.addr);
#else  
  pBtHomeV2Device = new BtHomeV2Device(BTHOME_SHORT_NAME, BTHOME_COMPLETE_NAME, false);
#endif  
  ADC_MOD.set_read_resolution(16);
}

//-----------------------------------------------------------------------------

void loop(void)
{
  measurements();

  activateAntenna();

  prepareBtPacket1();
  ble_advertise();

  prepareBtPacket2();
  ble_advertise();

  deactivateAntenna();

  if (voltage < LOW_VOLTAGE) {
    LowPower.sleep(TIME_TO_SLEEP_LOW * 1000);
  } else {
    LowPower.sleep(TIME_TO_SLEEP * 1000);
  }
}

//-----------------------------------------------------------------------------
// Read BME280 and supply voltage

void measurements(void)
{
  bmx.takeForcedMeasurement();
  temperature = bmx.readTemperature();
  // we want hPa, not mBar
  airpressure = bmx.readPressure() * 0.01f;
#if !CONF_BMP280
  humidity = bmx.readHumidity();
#endif  

#if CONF_BUILTIN_BATT_VOLTAGE
  // activate on board voltage divider
  digitalWrite(VBAT_EN, HIGH);
  // wait a bit for voltage divider to settle
  LowPower.sleep(10);
  // read supply voltage, 1210mV reference, voltage divider by 2, gain 0.5, 16 bits resolution, 64x oversampling
  // (1210mV reference is much more stable than 3V3 reference)
  voltage = ADC_MOD.get_sample(PD4, PIN_NAME_NC, AR_INTERNAL1V2, iadcCfgAnalogGain0P5x, iadcCfgOsrHighSpeed64x) 
    * (4.0f * 0.001f * ADC_MOD.get_reference_voltage_mv() / ((1L << ADC_MOD.get_read_resolution()) - 1));
  // deactivate on board voltage divider
  digitalWrite(VBAT_EN, LOW);
#else
  // read supply voltage, 1210mV reference, voltage divider R1=6M2 R2=1M5, gain 1.0, 16 bits resolution, 64x oversampling
  voltage = ADC_MOD.get_sample(BATT_PIN, PIN_NAME_NC, AR_INTERNAL1V2, iadcCfgAnalogGain1x, iadcCfgOsrHighSpeed64x) 
    * (0.001f * ADC_MOD.get_reference_voltage_mv() * (1.0f + R1/R2) / ((1L << ADC_MOD.get_read_resolution()) - 1));
#endif  

#if CONF_SERIAL_DBG
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");
    
  Serial.print("Pressure = ");
  Serial.print(airpressure);
  Serial.println(" hPa");
  
#if !CONF_BMP280
  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");
#endif

  Serial.print("Voltage = ");
  Serial.print(voltage);
  Serial.println(" V");

  Serial.println("----------------------");
#endif  
}

//-----------------------------------------------------------------------------
// BtHome Packet1

void prepareBtPacket1(void)
{
  pBtHomeV2Device->clearMeasurementData();
  // Available bytes when using encryption: 11, 23 otherwise
  // Remaining bytes: 11
  counter += 1;
  // Send a counter value to Home Assistant, can be use for i.e. packet loss statistics
  pBtHomeV2Device->addCount_0_4294967295(counter);
  // Remaining bytes: 6
  pBtHomeV2Device->addVoltage_0_to_65_resolution_0_001(voltage);
  // Remaining bytes: 3
  pBtHomeV2Device->addTemperature_neg327_to_327_Resolution_0_01(temperature);
  // Remaining bytes: 0
}

//-----------------------------------------------------------------------------
// BtHome Packet2

void prepareBtPacket2(void)
{
  pBtHomeV2Device->clearMeasurementData();
  // Available bytes when using encryption: 11, 23 otherwise
#if !CONF_BMP280
  // Remaining bytes: 11
  pBtHomeV2Device->addHumidityPercent_Resolution_0_01(humidity);
#endif  
  // Remaining bytes: 7
  pBtHomeV2Device->addPressureHpa(airpressure);
  // Remaining bytes: 3
}

/**************************************************************************//**
 * Starts BLE advertisement
 * Initializes advertising if it's called for the first time
 *****************************************************************************/
void ble_advertise()
{
  // set up the buffer for the advertisement data
  uint8_t advertisementData[MAX_ADVERTISEMENT_SIZE];
  uint8_t size = 0;
  uint8_t advertising_set_handle = 0xff;
  sl_status_t sc;

  // Switch on board LED on while advertising
  digitalWrite(LED_BUILTIN, LOW);

  size = pBtHomeV2Device->getAdvertisementData(advertisementData);  

  sc = sl_bt_advertiser_create_set(&advertising_set_handle);
  app_assert_status(sc);

  // Set advertising interval to 20ms  
  sc = sl_bt_advertiser_set_timing(
    advertising_set_handle,
    32,    // minimum advertisement interval (milliseconds * 1.6) 32=20ms 80=50ms 160=100ms
    33,    // maximum advertisement interval (milliseconds * 1.6)
    0,     // advertisement duration, 0 = until stopped
    0);    // maximum number of advertisement events, 0 = endless
  app_assert_status(sc);

  // Generate data for advertising
  sc = sl_bt_legacy_advertiser_set_data(advertising_set_handle, sl_bt_advertiser_advertising_data_packet, size, advertisementData);
  app_assert_status(sc);

  // Start advertising and enable connections
  sc = sl_bt_legacy_advertiser_start(advertising_set_handle, sl_bt_legacy_advertiser_non_connectable);
  app_assert_status(sc);

#if CONF_SERIAL_DBG
  Serial.println("Raw advertising started!");
#endif  
  LowPower.sleep(ADVERTISING_DURATION_MS);

  sc = sl_bt_advertiser_stop(advertising_set_handle);
  app_assert_status(sc);

  sc = sl_bt_advertiser_delete_set(advertising_set_handle);
  app_assert_status(sc);

#if CONF_SERIAL_DBG
  Serial.println("Raw advertising ended!");
#endif
  // Switch on board LED off
  digitalWrite(LED_BUILTIN, HIGH);
}

/**************************************************************************//**
 * Bluetooth stack event handler
 * Called when an event happens on BLE the stack
 *
 * @param[in] evt Event coming from the Bluetooth stack
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  sl_status_t sc = SL_STATUS_OK;
  uint8_t bleAddressType;

  switch (SL_BT_MSG_ID(evt->header)) {
    // This event is received when the BLE device has successfully booted
    case sl_bt_evt_system_boot_id:
      // Get BLE address and address type
      sc = sl_bt_system_get_identity_address(&bleAddress, &bleAddressType);
      app_assert_status(sc);
      bleReady = true;
      break;

    // Default event handler
    default:
#if CONF_SERIAL_DBG
      Serial.print("BLE event: ");
      Serial.println(SL_BT_MSG_ID(evt->header));
#endif      
      break;
  }
}

//-----------------------------------------------------------------------------
// Flash functions
void sendSPI(byte data)
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(MOSI1, data & 0x80);
    data <<= 1;
    digitalWrite(CLK1, HIGH);
    delayMicroseconds(1);
    digitalWrite(CLK1, LOW);
    delayMicroseconds(1);
  }
}

void writeEnable()
{
  digitalWrite(CS1, LOW);
  sendSPI(WRITE_ENABLE);
  digitalWrite(CS1, HIGH);
}

//-----------------------------------------------------------------------------
// BME280/BMP280 functions

bool setup_bmx(bool altAddress)
{
  
#if CONF_BMP280
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  bool bmxOk = bmx.begin(altAddress ? BMP280_ADDRESS_ALT : BMP280_ADDRESS);
#else
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  bool bmxOk = bmx.begin(altAddress ? BME280_ADDRESS_ALTERNATE : BME280_ADDRESS);
#endif  
  if (!bmxOk) {
    return false;
  }  
#if CONF_BMP280
  bmx.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X1,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X1,     /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_OFF,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_1);   /* Standby time. */ 
#else
  bmx.setSampling(Adafruit_BME280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BME280::SAMPLING_X1,     /* Temp. oversampling */
                  Adafruit_BME280::SAMPLING_X1,     /* Pressure oversampling */
                  Adafruit_BME280::SAMPLING_X1,     /* Humidity oversampling */
                  Adafruit_BME280::FILTER_OFF,      /* Filtering. */
                  Adafruit_BME280::STANDBY_MS_0_5); /* Standby time. */ 
#endif                  
  return true;
}
//-----------------------------------------------------------------------------
// On board antenna

void activateAntenna(void)
{
  // turn on supply for antenna switch
  digitalWrite(RF_SW_PWR_PIN, HIGH);
  // HIGH -> Use external antenna / LOW -> Use built-in chip antenna
#if CONF_EXTERNAL_ANTENNA
  digitalWrite(RF_SW_SEL_PIN, HIGH);  
#else
  digitalWrite(RF_SW_SEL_PIN, LOW);  
#endif  
}

void deactivateAntenna(void)
{
  // turn off supply for antenna switch
  digitalWrite(RF_SW_SEL_PIN, LOW); // RFSW sel OFF
  digitalWrite(RF_SW_PWR_PIN, LOW); // RFSW Power OFF
}
