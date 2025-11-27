
/*
  Wrapper for Bosch BME280/BMP280
 */

#include "bme280.h"

LOG_MODULE_REGISTER(SolarSensor54l15bme280, LOG_LEVEL_INF);

SENSOR_DT_READ_IODEV(iodev, DT_COMPAT_GET_ANY_STATUS_OKAY(bosch_bme280),
		{SENSOR_CHAN_AMBIENT_TEMP, 0},
		{SENSOR_CHAN_HUMIDITY, 0},
		{SENSOR_CHAN_PRESS, 0});

RTIO_DEFINE(ctx, 1, 1);

static const struct device *const bme280_dev = DEVICE_DT_GET_ANY(bosch_bme280);

int bme280_init(void)
{
	int err = device_init(bme280_dev);	
	if (err) {
		LOG_ERR("\nDevice %s init failed %d\n", bme280_dev->name, err);
		return err;
	}
	err = pm_device_action_run(bme280_dev, PM_DEVICE_ACTION_SUSPEND);
	if (err) {
		LOG_ERR("\nDevice %s pm_device_action_run failed %d\n", bme280_dev->name, err);
		return err;
	}
    return 0;
}

int bme280_check_device(void)
{
	if (bme280_dev == NULL) {
		/* No such node, or the node does not have status "okay". */
		LOG_ERR("\nError: no device found.\n");
		return -1;
	}
	if (!device_is_ready(bme280_dev)) {
		LOG_ERR("Error: Device \"%s\" is not ready; "
		       "check the driver initialization logs for errors.\n",
		       bme280_dev->name);
		return -2;
	}

	LOG_INF("Found device \"%s\", getting sensor data\n", bme280_dev->name);
	return 0;
}

int bme280_chipId(void)
{
    if (bme280_dev != NULL && bme280_dev->state->initialized) {
        const struct bme280_data *pData = (const struct bme280_data *)bme280_dev->data;
        return pData->chip_id;        
    }
    return BMX280_CHIP_ID_NA;
}

int bme280_read(float *pTemp, float *pHum, float *pPress)
{
    int err;
	uint8_t buf[128];
    
    if (bme280_dev != NULL) {
        err = pm_device_action_run(bme280_dev, PM_DEVICE_ACTION_RESUME);
        if (err != 0) {
            LOG_ERR("%s: pm_device_action_run() RESUME failed: %d\n", bme280_dev->name, err);
        }
        err = bme280_check_device();

        if (err == 0) {

            // bme280_channel_get
            err = sensor_read(&iodev, &ctx, buf, 128);
            if (err != 0) {
                LOG_ERR("%s: sensor_read() failed: %d\n", bme280_dev->name, err);
                return err;
            }
            const struct sensor_decoder_api *decoder;
            err = sensor_get_decoder(bme280_dev, &decoder);

            if (err != 0) {
                LOG_ERR("%s: sensor_get_decode() failed: %d\n", bme280_dev->name, err);
                return err;
            }		
            
            if (pTemp != NULL) {
                uint32_t temp_fit = 0;
                struct sensor_q31_data temp_data = {0};                
                if (decoder->decode(buf, (struct sensor_chan_spec) {SENSOR_CHAN_AMBIENT_TEMP, 0}, &temp_fit, 1, &temp_data) > 0)
                {
                    *pTemp = temp_data.readings[0].temperature*1.0f/32768.0f;
                    LOG_INF("temp: %.2f", (double)*pTemp);
                }
            }

            if (pPress != NULL) {
                uint32_t press_fit = 0;
                struct sensor_q31_data press_data = {0};
                if (decoder->decode(buf, (struct sensor_chan_spec) {SENSOR_CHAN_PRESS, 0}, &press_fit, 1, &press_data) > 0)
                {
                    *pPress = press_data.readings[0].pressure*10.0f/256.0f;
                    LOG_INF("press: %.2f", (double)*pPress);
                }
            }

            if (pHum != NULL && bme280_chipId() == BME280_CHIP_ID) {
                uint32_t hum_fit = 0;
                struct sensor_q31_data hum_data = {0};
                if (decoder->decode(buf, (struct sensor_chan_spec) {SENSOR_CHAN_HUMIDITY, 0}, &hum_fit, 1, &hum_data) > 0) 
                {
                    *pHum = hum_data.readings[0].humidity*1.0f/1024.0f;
                    LOG_INF("hum: %.2f", (double)*pHum);
                }
            }
        }
        err = pm_device_action_run(bme280_dev, PM_DEVICE_ACTION_SUSPEND);
        if (err != 0) {
            LOG_ERR("%s: pm_device_action_run() SUSPEND failed: %d\n", bme280_dev->name, err);
        }
    }
    return 0;
}