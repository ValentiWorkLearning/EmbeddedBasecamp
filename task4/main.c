#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gc9a01.h"
#include "buttons_module.h"

#define CONVERSION_BUFFER_SIZE 64

typedef struct drawing_processor
{
	bool counter_rect_invalidated;
}drawing_processor_t;

drawing_processor_t drawing_manager = { .counter_rect_invalidated = true };


int counter_value = 0;

void push_counter_up()
{
	counter_value+=1;
	printf("Updated counter value UP:%d\n",counter_value);
	update_counter_rect();
}
void push_counter_down()
{
	if (counter_value - 1 >= 0)
		counter_value-=1;
	printf("Updated counter value DOWN:%d\n",counter_value);
	update_counter_rect();
}
void reset_counter()
{
	counter_value = 0;
	printf("Updated counter value RESET:%d\n",counter_value);
	update_counter_rect();
}

void update_counter_rect()
{
	drawing_manager.counter_rect_invalidated = true;
}

void draw_current_time()
{
	char conversion_buffer[CONVERSION_BUFFER_SIZE];

	time_t current_system_time = time(NULL);
	struct tm *loc_time = localtime (&current_system_time);
	snprintf(conversion_buffer,sizeof(conversion_buffer),"%d:%d:%d",loc_time.tm_hour,loc_time.tm_min,loc_time.tm_sec);
	
	const int x0_coord = LCD_WIDTH / 4;
	const int y0_counter = LCD_HEIGHT - 40;
	lcd_put_string(conversion_buffer,x0_coord,y0_counter,Font_11x18,COLOR_RED,COLOR_BLACK);
	lcd_update_screen();
}
void draw_ip_address()
{
}

void draw_counter_value()
{
	if(drawing_manager.counter_rect_invalidated)
	{
		char conversion_buffer[CONVERSION_BUFFER_SIZE];

		const int x0_counter = LCD_WIDTH / 2 - Font_16x26.width/2;
		const int y0_counter = LCD_HEIGHT /2 - Font_16x26.height/2;
		snprintf(conversion_buffer, sizeof(conversion_buffer), "%d", counter_value);
		lcd_put_string(conversion_buffer,x0_counter,y0_counter,Font_16x26,COLOR_RED,COLOR_BLACK);
		lcd_update_screen();
		drawing_manager.counter_rect_invalidated = false;
	}
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
		get_current_time();
		draw_counter_value();
		draw_current_time();
		draw_ip_address();
	}

	deinit_buttons_module();
	lcd_deinit();
	return EXIT_SUCCESS;
}