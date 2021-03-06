#include "gc9a01.h"
#include "gpio_module.h"
#include "spi_module.h"
#include <linux/types.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/slab.h>

#define GPIO_PIN_RESET LCD_PIN_RESET
#define GPIO_PIN_DC LCD_PIN_DC
#define DATA_SIZE 1000

static uint16_t *frame_buffer;

// clang-format off
#define COMMANDS_SIZE 328
#define COMMANDS_TRANSACTIONS_COUNT 59
static const uint8_t Commands[COMMANDS_SIZE] = 
{
    /***Cmd****Argc****delay****argv*****************************/
        0xFE,   0,      0
    ,   0xEF,   0,      0
    ,   0xEB,   1,      0,      0x14

    ,   0xFE,   0,      0
    ,   0xEF,   0,      0
    ,   0xEB,   1,      0,      0x14
    ,   0xFE,   0,      0
    ,   0xEF,   0,      0
    ,   0xEB,   1,      0,      0x14

    ,   0xFE,   0,      0
    ,   0xEF,   0,      0

    ,   0x84,   1,      0,      0x40
    ,   0x85,   1,      0,      0xF1
    ,   0x86,   1,      0,      0x98
    ,   0x87,   1,      0,      0x28
    ,   0x88,   1,      0,      0x0A
    ,   0x89,   1,      0,      0x21

    ,   0x8A,   1,      0,      0x00
    ,   0x8B,   1,      0,      0x80
    ,   0x8C,   1,      0,      0x01
    ,   0x8D,   1,      0,      0x00
    ,   0x8E,   1,      0,      0xDF
    ,   0x8F,   1,      0,      0x52

    ,   0xB6,   1,      0,      0x20

    ,   0x36,   1,      0,      0x48

    ,   0x3A,   1,      0,      0x05

    ,   0x90,   4,      0,      0x08,0x08,0x08,0x08

    ,   0xBD,   1,      0,      0x06
    ,   0xA6,   1,      0,      0x74
    ,   0xBF,   1,      0,      0x1C
    ,   0xA7,   1,      0,      0x45
    ,   0xA9,   1,      0,      0xBB
    ,   0xB8,   1,      0,      0x63
    ,   0xE8,   1,      0,      0x34

    ,   0xFF,   3,      0,      0x60, 0x01,0x04

    ,   0x74,   7,      0,      0x10,0x75,0x80,0x00,0x00,0x4E,0x00
    ,   0xC3,   5,      0,      0x14,0xC4,0x14,0xC9,0x25

    ,   0xBE,   1,      0,      0x11
    ,   0xBC,   1,      0,      0x00
    ,   0xE1,   2,      0,      0x10,0x0e

    ,   0xDF,   3,      0,      0x21,0x0c,0x02

    ,   0xED,   2,      0,      0x1B,0x0B

    ,   0xAE,   5,      0,      0x77,0xCB,0x02,0xCD,0x63

    ,   0x70,   9,      0,      0x07,0x09,0x04,0x0e,0x0f, 0x09,0x07,0x08,0x03
    ,   0xf0,   6,      0,      0x45,0x09,0x08,0x08,0x26,0x2a
    ,   0xf1,   6,      0,      0x43,0x70,0x72,0x36,0x37,0x6f
    ,   0xf2,   6,      0,      0x45,0x09,0x08,0x08,0x26,0x2a
    ,   0xf3,   6,      0,      0x43,0x70,0x72,0x36,0x37,0x6f

    ,   0x62,   12,     0,      0x18,0x0d,0x71,0xED,0x70,0x70,0x18,0x0F,0x71,0xEF,0x70,0x70
    ,   0x63,   12,     0,      0x18,0x11,0x71,0xf1,0x70,0x70,0x18,0x13,0x71,0xf3,0x70,0x70
    ,   0x64,   7,      0,      0x28,0x29,0xF1,0x01,0xf1,0x00,0x07
    ,   0x66,   10,     0,      0x3c,0x00,0xCD,0x67,0x45,0x45,0x10,0x00,0x00,0x00
    ,   0x67,   10,     0,      0x00,0x3c,0x00,0x00,0x00,0x01,0x54,0x10,0x32,0x98
    ,   0x74,   7,      0,      0x10,0x85,0x80,0x00,0x00,0x4e,0x00

    ,   0x98,   2,      0,      0x3e,0x07

    ,   0x35,   0,      0
    ,   0x21,   0,      0

    ,   0x11,   0,      120
    ,   0x29,   0,      120
};
// clang-format on

static void init_display_internal(void);

int init_display(int spi_module_index, int chip_select)
{
	gpio_module_init_out(GPIO_PIN_RESET);
	gpio_module_init_out(GPIO_PIN_DC);
	lcd_reset();
	if (init_spi_wrapper(spi_module_index,chip_select)) {
		printk(KERN_ERR
		       "Failed to initialize display. SPI module initialization failed\n");
		return -1;
	}
	frame_buffer =
		kmalloc(LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t), GFP_KERNEL);
	if (frame_buffer == NULL) {
		printk(KERN_ERR "Failed to allocate display framebuffer\n");
		return -1;
	}
	init_display_internal();
	return 0;
}

static void lcd_write_command(uint8_t cmd)
{
	gpio_module_set_value(GPIO_PIN_DC, 0);
	spi_wrapper_write(&cmd, sizeof(cmd));
}

static void lcd_write_data(const uint8_t *buff, size_t buff_size)
{
	gpio_module_set_value(LCD_PIN_DC, 1);
	spi_wrapper_write(buff, buff_size);
}

static void init_display_internal(void)
{
	size_t CommandCount = COMMANDS_TRANSACTIONS_COUNT;
	const uint8_t *pBuffer = Commands;
	while (CommandCount--) {
		const uint8_t *Command = pBuffer++;
		const uint8_t NumArgs = *pBuffer++;
		const uint8_t Delay = *pBuffer++;

		lcd_write_command(*Command);
		if (NumArgs) {
			lcd_write_data(pBuffer++, NumArgs);
			pBuffer += (NumArgs - 1);
		}
		if (Delay)
			mdelay(Delay);
	}
}

void lcd_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	lcd_write_command(0x2a);
	{
		uint8_t data[] = { (x0 >> 8) & 0xFF, x0 & 0xFF,
				   (x1 >> 8) & 0xFF, x1 & 0xFF };
		lcd_write_data(data, sizeof(data));
	}

	lcd_write_command(0x2b);
	{
		uint8_t data[] = { (y0 >> 8) & 0xFF, y0 & 0xFF,
				   (y1 >> 8) & 0xFF, y1 & 0xFF };
		lcd_write_data(data, sizeof(data));
	}

	lcd_write_command(0x2C);
}

void lcd_update_screen(void)
{
	lcd_write_data((uint8_t *)frame_buffer,
		       sizeof(uint16_t) * LCD_WIDTH * LCD_HEIGHT);
}

void lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
	if ((x >= LCD_WIDTH) || (y >= LCD_HEIGHT)) {
		return;
	}

	frame_buffer[x + LCD_WIDTH * y] = (color >> 8) | (color << 8);
	lcd_update_screen();
}
void lcd_fill_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
			uint16_t color)
{
	uint16_t i;
	uint16_t j;

	if ((x >= LCD_WIDTH) || (y >= LCD_HEIGHT)) {
		return;
	}

	if ((x + w - 1) > LCD_WIDTH) {
		w = LCD_WIDTH - x;
	}

	if ((y + h - 1) > LCD_HEIGHT) {
		h = LCD_HEIGHT - y;
	}

	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++) {
			frame_buffer[(x + LCD_WIDTH * y) + (i + LCD_WIDTH * j)] =
				(color >> 8) | (color << 8);
		}
	}
	lcd_update_screen();
}

void lcd_fill_screen(uint16_t color)
{
	lcd_fill_rectangle(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1, color);
}

void lcd_put_char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color,
		  uint16_t bgcolor)
{
	uint32_t i, b, j;

	for (i = 0; i < font.height; i++) {
		b = font.data[(ch - 32) * font.height + i];
		for (j = 0; j < font.width; j++) {
			if ((b << j) & 0x8000) {
				frame_buffer[(x + LCD_WIDTH * y) +
					     (j + LCD_WIDTH * i)] =
					(color >> 8) | (color << 8);
			} else {
				frame_buffer[(x + LCD_WIDTH * y) +
					     (j + LCD_WIDTH * i)] =
					(bgcolor >> 8) | (bgcolor << 8);
			}
		}
	}
}

void lcd_put_string(const char *string, uint16_t x, uint16_t y, FontDef font,
		    uint16_t color, uint16_t bgcolor)
{
	int length_of_string = strlen(string);
	uint16_t local_x = x;
	uint16_t local_y = y;

	int i;
	for (i = 0; i < length_of_string; ++i) {
		bool move_to_next_line = local_x + font.width > LCD_WIDTH;
		bool stop_drawing = local_y + font.height > LCD_HEIGHT;
		if (stop_drawing)
			return;

		if (move_to_next_line) {
			local_y += font.height;
			local_x = x;
		}
		lcd_put_char(local_x, local_y, string[i], font, color, bgcolor);
		local_x += font.width;
	}
}

void lcd_reset(void)
{
	gpio_module_set_value(GPIO_PIN_RESET, 0);
	mdelay(5000);
	gpio_module_set_value(GPIO_PIN_RESET, 1);
}

void lcd_deinit(void)
{
	if (frame_buffer)
		kfree(frame_buffer);
	deinit_spi_wrapper();
	gpio_module_free(GPIO_PIN_RESET);
	gpio_module_free(GPIO_PIN_DC);
}


u8 *get_framebuffer_raw_ptr(void)
{
	return (u8*)frame_buffer;
}
u32 get_framebuffer_size(void)
{
	return LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t);
}
