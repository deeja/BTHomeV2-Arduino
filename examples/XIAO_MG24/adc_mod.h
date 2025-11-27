/*
 * This file is part of the Silicon Labs Arduino Core
 *
 * The MIT License (MIT)
 *
 * Copyright 2024 Silicon Laboratories Inc. www.silabs.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Arduino.h"
#include "pinDefinitions.h"

#ifndef __ARDUINO_ADC_MOD_H
#define __ARDUINO_ADC_MOD_H

#include <cmath>
#include <inttypes.h>
#include "em_cmu.h"
#include "em_iadc.h"
#include "em_ldma.h"
#include "dmadrv.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "sl_status.h"
#include "adc.h"

namespace arduino {
class AdcClassMod {
public:

  /***************************************************************************//**
   * Constructor for AdcClassMod
   ******************************************************************************/
  AdcClassMod();

  /***************************************************************************//**
   * Performs a single ADC measurement on the provided pin and returns the sample
   *
   * @param[in] ppin The pin number of the positive ADC input
   * @param[in] npin The pin number of the negative ADC input (set to PIN_NAME_NC for non differential positive mode)
   * @param[in] reference The selected voltage reference from 'analog_references'
   * @param[in] gain The selected gain from 'IADC_CfgAnalogGain_t'
   * @param[in] osr The selected oversampling rate from 'IADC_CfgOsrHighSpeed_t'
   *
   * @return the measured ADC sample
   ******************************************************************************/
  uint16_t get_sample(PinName ppin, PinName npin = PIN_NAME_NC, uint8_t reference = AR_VDD, 
    IADC_CfgAnalogGain_t gain = iadcCfgAnalogGain1x, IADC_CfgOsrHighSpeed_t osr = iadcCfgOsrHighSpeed2x);

  /***************************************************************************//**
   * Sets the ADC voltage reference
   *
   * @param[in] reference The selected voltage reference from 'analog_references'
   ******************************************************************************/
  void set_reference(uint8_t reference);

  /***************************************************************************//**
   * Sets the ADC read resolution
   *
   * @param[in] resolution The selected read resolution in bits
   ******************************************************************************/
  void set_read_resolution(uint8_t resolution);

  /***************************************************************************//**
   * Starts ADC in scan (continuous) mode
   *
   * @param[in] buffer The buffer where the sampled data is stored
   * @param[in] size The size of the buffer
   * @param[in] channel The number of the LDMA channel used
   *
   * @return Status of the scan init process
   ******************************************************************************/
  sl_status_t scan_start(PinName pin, uint32_t *buffer, uint32_t size, void (*user_onsampling_finished_callback)());

  /***************************************************************************//**
   * Stops ADC scan
   ******************************************************************************/
  void scan_stop();

  /***************************************************************************//**
   * De-initialize the ADC
   ******************************************************************************/
  void deinit();

  /***************************************************************************//**
   * Callback handler for the DMA transfer
   ******************************************************************************/
  void handle_dma_finished_callback();

  // The maximum read resolution of the ADC
  static const uint8_t max_read_resolution_bits = 16u;

  /***************************************************************************//**
   * Sets the ADC gain factor
   *
   * @param[in] gain The selected gain from 'IADC_CfgAnalogGain_t'
   ******************************************************************************/
  void set_gain(IADC_CfgAnalogGain_t gain);

  /***************************************************************************//**
   * Sets the ADC oversampling rate
   *
   * @param[in] osr The selected oversampling rate from 'IADC_CfgOsrHighSpeed_t'
   ******************************************************************************/
  void set_oversampling(IADC_CfgOsrHighSpeed_t osr);

  /***************************************************************************//**
   * Gets the ADC read resolution
   *
   * @return The selected read resolution in bits
   ******************************************************************************/
  uint8_t get_read_resolution(void);

  /***************************************************************************//**
   * Gets the voltage of the selected voltage reference in milli volts
   *
   * @return Voltage on milli volts
   ******************************************************************************/
  int16_t get_reference_voltage_mv(void);

protected:
  /***************************************************************************//**
   * Initializes the ADC hardware as single shot
   *
   * @param[in] ppin The pin number of the positive ADC input
   * @param[in] npin The pin number of the negative ADC input
   * @param[in] reference The selected voltage reference from 'analog_references'
   * @param[in] gain The selected gain from 'IADC_CfgAnalogGain_t'
   * @param[in] osr The selected oversampling rate from 'IADC_CfgOsrHighSpeed_t'
   ******************************************************************************/
  void init_single(PinName ppin, PinName npin, uint8_t reference, IADC_CfgAnalogGain_t gain, IADC_CfgOsrHighSpeed_t osr);

  /***************************************************************************//**
   * Initializes the ADC hardware in scan (continuous) mode
   *
   * @param[in] pin The pin number of the ADC input
   ******************************************************************************/
  void init_scan(PinName pin, uint8_t reference);

  /**************************************************************************//**
   * Initializes the DMA hardware
   *
   * @param[in] buffer Pointer to the array where ADC results will be stored
   * @param[in] size Size of the array
   * @param[in] channel Channel to use for transfer
   *
   * @return Status of the DMA init process
   *****************************************************************************/
  sl_status_t init_dma(uint32_t *buffer, uint32_t size);

  /**************************************************************************//**
   * Get characteristics of voltage reference
   *
   * @param[in] reference The voltage reference from 'analog_references'
   * @param[out] sl_adc_reference Value of reference mapped to 'IADC_CfgReference_t'
   * @param[out] sl_adc_vref Voltage of reference
   *
   * @return Status of the DMA init process
   *****************************************************************************/
  bool get_reference_info(uint8_t reference, IADC_CfgReference_t& sl_adc_reference, uint32_t& sl_adc_vref);

  /**************************************************************************//**
   * Allocate the analog bus for ADC0 inputs
   *
   * @param[in] pin The pin number of the ADC input
   *****************************************************************************/
  void allocatePin(PinName pin);

  bool initialized_single;
  bool initialized_scan;
  bool paused_transfer;

  PinName current_adc_ppin;
  PinName current_adc_npin;
  uint8_t current_adc_reference;
  uint8_t current_read_resolution;
  IADC_CfgAnalogGain_t current_gain;
  IADC_CfgOsrHighSpeed_t current_osr;

  LDMA_Descriptor_t ldma_descriptor;
  unsigned int dma_channel;
  unsigned int dma_sequence_number;

  void (*user_onsampling_finished_callback)(void);

  static const IADC_PosInput_t GPIO_to_ADC_ppin_map[64];
  static const IADC_NegInput_t GPIO_to_ADC_npin_map[64];

  SemaphoreHandle_t adc_mutex;
  StaticSemaphore_t adc_mutex_buf;
};
} // namespace arduino

extern arduino::AdcClassMod ADC_MOD;

#endif // __ARDUINO_ADC_MOD_H
