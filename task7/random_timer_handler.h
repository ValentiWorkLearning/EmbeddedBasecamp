#ifndef RANDOM_TIMER_H
#define RANDOM_TIMER_H

typedef void (*new_random_value_arrived_cb)(int new_random_value);

void init_random_timer(void);
void deinit_random_timer(void);
void set_new_random_value_arrived_callback(new_random_value_arrived_cb callback);
#endif