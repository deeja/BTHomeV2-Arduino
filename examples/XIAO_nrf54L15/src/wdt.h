#pragma once

/*
  Wrapper for watchdog timer
 */

#include <zephyr/logging/log.h>
#include <zephyr/drivers/watchdog.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize watchdog timer
 *
 * @param minWindow Lower limit of watchdog feed timeout in milliseconds
 * @param maxWindow Upper limit of watchdog feed timeout in milliseconds
 * @param options configuration options (see WDT_OPT)
 * 
 * @retval channel id if initialization was successfull, negative value otherwise
 */
extern int init_wdt(int32_t minWindow, int32_t maxWindow, int32_t options);
/**
 * @brief Feed watchdog timer
 * 
 * @param wdt_channel_id id returned by init_wdt
 */
extern void feed_wdt(int32_t wdt_channel_id);

#ifdef __cplusplus
}
#endif
