
/*
  Wrapper for watchdog timer
 */

#include "wdt.h"

LOG_MODULE_REGISTER(SolarSensor54l15wdt, LOG_LEVEL_INF);

static const struct device *const wdt = DEVICE_DT_GET(DT_ALIAS(watchdog0));

int init_wdt(int32_t minWindow, int32_t maxWindow, int32_t options)
{
	if (!device_is_ready(wdt)) {
		LOG_ERR("%s: device not ready.\n", wdt->name);
		return -1;
	}	
	struct wdt_timeout_cfg wdt_config = {
		/* Reset SoC when watchdog timer expires. */
		.flags = WDT_FLAG_RESET_SOC,
		/* Expire watchdog after max window */
		.window.min = minWindow,
		.window.max = maxWindow,
	};	
	int wdt_channel_id = wdt_install_timeout(wdt, &wdt_config);
	if (wdt_channel_id < 0) {
		LOG_ERR("Watchdog install error\n");
		return -2;
	}
	int err = wdt_setup(wdt, options);
	if (err < 0) {
		LOG_ERR("Watchdog setup error\n");
		return -3;
	}
    return wdt_channel_id;
}

void feed_wdt(int32_t wdt_channel_id)
{
	wdt_feed(wdt, wdt_channel_id);
}