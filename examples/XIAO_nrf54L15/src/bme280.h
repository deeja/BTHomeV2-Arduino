#pragma once

/*
  Wrapper for Bosch BME280/BMP280
 */

#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/pm/device.h>
#include <zephyr/pm/device_runtime.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BMX280_CHIP_ID_NA       0x00
#define BMP280_CHIP_ID_MP       0x58
#define BME280_CHIP_ID          0x60

struct bme280_decoder_header {
	uint64_t timestamp;
} __attribute__((__packed__));

struct bme280_reading {
	/* Compensated values. */
	int32_t comp_temp;
	uint32_t comp_press;
	uint32_t comp_humidity;
};

struct bme280_encoded_data {
	struct bme280_decoder_header header;
	struct {
		/** Set if `temp` has data */
		uint8_t has_temp: 1;
		/** Set if `press` has data */
		uint8_t has_press: 1;
		/** Set if `humidity` has data */
		uint8_t has_humidity: 1;
	} __attribute__((__packed__));
	struct bme280_reading reading;
};

struct bme280_data {
	/* Compensation parameters. */
	uint16_t dig_t1;
	int16_t dig_t2;
	int16_t dig_t3;
	uint16_t dig_p1;
	int16_t dig_p2;
	int16_t dig_p3;
	int16_t dig_p4;
	int16_t dig_p5;
	int16_t dig_p6;
	int16_t dig_p7;
	int16_t dig_p8;
	int16_t dig_p9;
	uint8_t dig_h1;
	int16_t dig_h2;
	uint8_t dig_h3;
	int16_t dig_h4;
	int16_t dig_h5;
	int8_t dig_h6;

	/* Carryover between temperature and pressure/humidity compensation. */
	int32_t t_fine;

	uint8_t chip_id;

	struct bme280_reading reading;
};

/**
 * @brief Initialize sensor
 *
 * @retval 0 if initialization was successfull, negative value otherwise
 */
extern int bme280_init(void);
/**
 * @brief Check if sensor is ready
 *
 * @retval 0 if sensor is ready, negative value otherwise
 */
extern int bme280_check_device(void);
/**
 * @brief Read values from sensor
 *
 * @param pTemp pointer to temperature value in °C, may be NULL
 * @param pHum pointer to humidity value on %, may be NULL
 * @param pPress pointer to pressure in hPa, may be NULL
 * 
 * @retval 0 if sensor could be read, negative value otherwise
 */
extern int bme280_read(float *pTemp, float *pHum, float *pPress);
/**
 * @brief Get chip ID
 * 
 * @retval BMX280_CHIP_ID_NA: unknown device, BMP280_CHIP_ID_MP: BMP280, BME280_CHIP_ID: BME280
 */
extern int bme280_chipId(void);

#ifdef __cplusplus
}
#endif
