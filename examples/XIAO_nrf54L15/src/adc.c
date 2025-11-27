
/*
  Wrapper for ADC
 */

#include "adc.h"

LOG_MODULE_REGISTER(SolarSensor54l15AnalogIO, LOG_LEVEL_INF);

#define DT_SPEC_AND_COMMA(node_id, prop, idx) ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
 	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, DT_SPEC_AND_COMMA)
};

/**
 * 
 */
int adcSetup(uint8_t channel)
{
	int err;

	LOG_INF("channel %d gain %d", channel, adc_channels[channel].channel_cfg.gain);

 	/* Configure channels individually prior to sampling. */
 	if (!adc_is_ready_dt(&adc_channels[channel])) {
  		LOG_ERR("ADC controller device %s not ready\n", adc_channels[channel].dev->name);
  		return -1;
 	}

 	err = adc_channel_setup_dt(&adc_channels[channel]);
 	if (err < 0) {
  		LOG_ERR("Could not setup channel #%d (%d)\n", channel, err);
  		return -1;
 	}

	return 0;
}

int adcRead(int32_t *pValMV, int32_t *pRaw, uint8_t channel)
{
	int err;
	uint16_t buf;
	
	struct adc_sequence sequence = {
		.buffer = &buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(buf),
	};

 	(void)adc_sequence_init_dt(&adc_channels[channel], &sequence);

 	err = adc_read_dt(&adc_channels[channel], &sequence);
 	if (err < 0) {
  		LOG_ERR("Could not read (%d)\n", err);
  		return err;
 	}	

 	/*
  	 * If using differential mode, the 16 bit value
     * in the ADC sample buffer should be a signed 2's
     * complement value.
  	*/
	int32_t raw;
 	if (adc_channels[channel].channel_cfg.differential) {
  		raw = (int32_t)((int16_t)buf);
 	} else {
  		raw = (int32_t)buf;
 	}
	if (pRaw != NULL) {
		*pRaw = raw;
	}
	uint32_t tmp = raw;
	if (pValMV != NULL) {
		err = adc_raw_to_millivolts_dt(&adc_channels[channel], &raw);
		/* conversion to mV may not be supported, skip if not */
		if (err < 0) {
			LOG_ERR(" value in mV not available\n");
			return err;
		}
		LOG_INF("channel %d voltage = %" PRId32 " mV raw = %d\n", channel, raw, tmp);
		*pValMV = raw;
	} else {
		LOG_INF("channel %d raw = %d\n", channel, raw);
	}
	return 0;
}
