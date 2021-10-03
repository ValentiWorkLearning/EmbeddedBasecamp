#ifndef TIMER_TRACK_MODULE_H
#define TIMER_TRACK_MODULE_H

#include <linux/time.h>

void init_timer_module(void);
void deinit_timer_module(void);

struct tm *timer_handler_get_absolute_time_value(void);

void timer_handler_get_last_used_as_tm(struct tm* tm_fill_up);
int timer_handler_get_seconds_last_used_value(void);

void timer_handler_refresh_last_used_time(void);
#endif