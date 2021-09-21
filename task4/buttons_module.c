#include "buttons_module.h"
#include <stddef.h>
#include <stdio.h>

#define SHORT_CLICK_THERSHOLD 1000
#define LONG_CLICK_THERSHOLD 6000

typedef enum button_state {
	kPressed = 0,
	kReleased = 1,
	None = -1
} button_state_t;

typedef struct button_descriptor {
	int pin_id;
	int press_count;
	int long_press_count;
	button_state_t button_state;
	button_callback_t short_click_handler;
	button_callback_t long_click_handler;
} button_descriptor_t;

static button_descriptor_t buttons[BUTTONS_COUNT] = {
	{ .pin_id = COUNTER_UP_PIN,
	  .press_count = 0,
	  .button_state = None,
	  .short_click_handler = NULL,
	  .long_click_handler = NULL },
	{ .pin_id = COUNTER_DOWN_PIN,
	  .press_count = 0,
	  .button_state = None,
	  .short_click_handler = NULL,
	  .long_click_handler = NULL },
	{ .pin_id = CONTROL_BUTTON_PIN,
	  .press_count = 0,
	  .button_state = None,
	  .short_click_handler = NULL,
	  .long_click_handler = NULL },
};

static button_descriptor_t *find_by_id(int button_id)
{
	for (int i = 0; i < BUTTONS_COUNT; ++i) {
		if (buttons[i].pin_id == button_id)
			return &buttons[i];
	}
	return NULL;
}

static void process_pin_event(int pin_event, int pin_number)
{
	for (int i = 0; i < BUTTONS_COUNT; ++i) {
		if (buttons[i].pin_id == pin_number) {
			if (pin_event == kPressed) {
				buttons[i].press_count += 1;
				buttons[i].button_state = kPressed;
			} else if (pin_event == kReleased) {
				if (buttons[i].button_state == kPressed) {
					int counter_press =
						buttons[i].press_count;
					if ((counter_press > 0) &&
					    (counter_press <
					     SHORT_CLICK_THERSHOLD)) {
						if (buttons[i]
							    .short_click_handler)
							buttons[i]
								.short_click_handler();
					} else if (counter_press >
						   LONG_CLICK_THERSHOLD) {
						if (buttons[i]
							    .long_click_handler)
							buttons[i]
								.long_click_handler();
					}
				}

				buttons[i].button_state = kReleased;
				buttons[i].press_count = 0;
			}
		}
	}
}

void init_buttons_module()
{
	gpio_init_in(COUNTER_UP_PIN);
	gpio_init_in(COUNTER_DOWN_PIN);
	gpio_init_in(CONTROL_BUTTON_PIN);
}
void process_buttons_module()
{
	int counter_pin_up = gpio_get_value(COUNTER_UP_PIN);
	process_pin_event(counter_pin_up, COUNTER_UP_PIN);

	int counter_down_pin = gpio_get_value(COUNTER_DOWN_PIN);
	process_pin_event(counter_down_pin, COUNTER_DOWN_PIN);

	int control_pin = gpio_get_value(CONTROL_BUTTON_PIN);
	process_pin_event(control_pin, CONTROL_BUTTON_PIN);
}
void deinit_buttons_module()
{
	gpio_free(COUNTER_UP_PIN);
	gpio_free(COUNTER_DOWN_PIN);
	gpio_free(CONTROL_BUTTON_PIN);
}

void attach_short_click_listener_counter_up(
	button_callback_t short_click_listener)
{
	button_descriptor_t *p_button = find_by_id(COUNTER_UP_PIN);
	if (p_button)
		p_button->short_click_handler = short_click_listener;
}

void attach_short_click_listener_counter_down(
	button_callback_t short_click_listener)
{
	button_descriptor_t *p_button = find_by_id(COUNTER_DOWN_PIN);
	if (p_button)
		p_button->short_click_handler = short_click_listener;
}

void attach_short_click_listener_control_button(
	button_callback_t short_control_button)
{
	button_descriptor_t *p_button = find_by_id(CONTROL_BUTTON_PIN);
	if (p_button)
		p_button->short_click_handler = short_control_button;
}

void attach_long_click_listener(button_callback_t long_click_listener)
{
	button_descriptor_t *p_button = find_by_id(CONTROL_BUTTON_PIN);
	if (p_button)
		p_button->long_click_handler = long_click_listener;
}