#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gc9a01.h"
#include "buttons_module.h"

int counter_value = 0;

void push_counter_up()
{
	++counter_value;
	printf("Updated counter value:%d\n",counter_value);
	update_counter_rect();
}
void push_counter_down()
{
	if (counter_value - 1 >= 0)
		--counter_value;
	printf("Updated counter value:%d\n",counter_value);
	update_counter_rect();
}
void reset_counter()
{
	counter_value = 0;
	printf("Updated counter value:%d\n",counter_value);
	update_counter_rect();
}
void update_counter_rect()
{
}

void draw_current_time()
{
}
void draw_ip_address()
{
}
void draw_counter_value()
{
}

int main(int argc, char *argv[])
{
	init_display();
	init_buttons_module();

	printf("LCD init ok\n");
	lcd_set_address_window(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

	lcd_fill_screen(COLOR_BLUE);
	lcd_put_char(10, 10, 'A', Font_11x18, COLOR_RED, COLOR_BLACK);
	lcd_update_screen();
	lcd_put_string("hello world!", 30, 20, Font_11x18, COLOR_RED,
		       COLOR_BLACK);

	attach_short_click_listener_counter_up(&push_counter_up);
	attach_short_click_listener_counter_down(&push_counter_down);
	attach_short_click_listener_control_button(&reset_counter);
	while (true) {
		process_buttons_module();
		draw_counter_value();
		draw_current_time();
		draw_ip_address();
	}

	deinit_buttons_module();
	lcd_deinit();
	return EXIT_SUCCESS;
}