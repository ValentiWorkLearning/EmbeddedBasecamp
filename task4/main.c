#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "gc9a01.h"

int main(int argc, char *argv[])
{
	init_display();
	printf("LCD init ok\n");
	lcd_set_address_window(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

	lcd_fill_screen(COLOR_BLUE);
	lcd_put_char(10, 10, 'A', Font_11x18, COLOR_RED, COLOR_BLACK);
	lcd_update_screen();
	lcd_put_string("hello world!",30,20,Font_11x18,COLOR_RED,COLOR_BLACK);

	sleep(2);

	lcd_deinit();
	return EXIT_SUCCESS;
}