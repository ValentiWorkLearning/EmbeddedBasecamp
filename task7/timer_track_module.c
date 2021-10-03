#include "timer_track_module.h"
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/types.h>
#include <linux/sched.h>

static struct tm current_time;
static uint32_t timestamp_measure_begin;

// https://elixir.bootlin.com/linux/v5.14.9/source/drivers/net/ethernet/broadcom/bnxt/bnxt.c#L8105
struct tm *timer_handler_get_absolute_time_value()
{
	time64_t now = ktime_get_real_seconds();
	time64_to_tm(now, 0, &current_time);
	return &current_time;
}

int timer_handler_get_seconds_last_used_value(void)
{
	uint32_t current_jiffies = jiffies;
	int seconds_since_last_use =
		(current_jiffies - timestamp_measure_begin) / HZ;

	return seconds_since_last_use;
}

void timer_handler_get_last_used_as_tm(struct tm* tm_fill_up)
{
	time64_t diff_seconds = timer_handler_get_seconds_last_used_value();
	time64_to_tm(diff_seconds,0,tm_fill_up);
}

void init_timer_module(void)
{
	timestamp_measure_begin = jiffies;
}

void timer_handler_refresh_last_used_time(void)
{
	timestamp_measure_begin = jiffies;
}

void deinit_timer_module(void)
{
}
