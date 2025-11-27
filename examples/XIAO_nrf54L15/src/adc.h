#pragma once
/*
  Wrapper for ADC
 */

#include <zephyr/logging/log.h>
#include <zephyr/drivers/adc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize given ADC channel
 *
 * @param channel index of ADC channel
 *
 * @retval 0 if initialization was successfull, negative value otherwise
 */
extern int adcSetup(uint8_t channel);
/**
 * @brief Read value from ADC channel
 *
 * @param pValMV pointer to result in milli volts, may be NULL
 * @param pRaw pointer to raw value, may be NULL
 *
 * @retval 0 if value could be read, negative value otherwise
 */
extern int adcRead(int32_t *pValMV, int32_t *pRaw, uint8_t channel);

#ifdef __cplusplus
}
#endif
