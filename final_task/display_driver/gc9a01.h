#ifndef GC9A01_DRIVER_H
#define GC9A01_DRIVER_H
#include <linux/types.h>
#include "fonts.h"

#define COLOR_BLACK 0x0000
#define COLOR_BLUE 0x001F
#define COLOR_RED 0xF800
#define COLOR_GREEN 0x07E0
#define COLOR_CYAN 0x07FF
#define COLOR_MAGENTA 0xF81F
#define COLOR_YELLOW 0xFFE0
#define COLOR_WHITE 0xFFFF
#define COLOR_COLOR565(r, g, b)                                                \
	(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

#define LCD_PIN_RESET 27
#define LCD_PIN_DC 22

#define LCD_WIDTH 240
#define LCD_HEIGHT 240

int init_display(int spi_module_index);
void lcd_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void lcd_update_screen(void);
void lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void lcd_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
			uint16_t color);
void lcd_fill_screen(uint16_t color);
void lcd_put_char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color,
		  uint16_t bgcolor);
void lcd_put_string(const char *string, uint16_t x, uint16_t y, FontDef font,
		    uint16_t color, uint16_t bgcolor);
void lcd_reset(void);
void lcd_deinit(void);
#endif
