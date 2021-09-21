#ifndef BUTTONS_MODULE
#define BUTTONS_MODULE
#include "gpio_module.h"

#define COUNTER_UP_PIN 21
#define COUNTER_DOWN_PIN 20
#define CONTROL_BUTTON_PIN 16

#define BUTTONS_COUNT 3

void init_buttons_module();
void process_buttons_module();
void deinit_buttons_module();

typedef void (*button_callback_t)(void);

void attach_short_click_listener_counter_up(
	button_callback_t short_click_listener);
void attach_short_click_listener_counter_down(
	button_callback_t short_click_listener);

void attach_short_click_listener_control_button(
	button_callback_t short_control_button);

void attach_long_click_listener(button_callback_t long_click_listener);

#endif