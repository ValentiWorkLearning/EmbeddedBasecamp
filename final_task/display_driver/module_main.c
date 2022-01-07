
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include "gc9a01.h"
#include "gc9a01_chardev.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Valentyn Korniienko <valentyn.korniienko1@nure.ua>");
MODULE_DESCRIPTION("GC9A01 display driver");
MODULE_VERSION("0.1");

static int spi_bus_index = 0;
static int chip_select = 0;

module_param(spi_bus_index, int, S_IRUGO);
module_param(chip_select, int, S_IRUGO);

static display_chardev_descriptor display_functions = {
	.p_display_updater = &lcd_update_screen,
	.p_fb_getter = &get_framebuffer_raw_ptr,
	.p_fb_size_getter = &get_framebuffer_size
};

static int __init display_module_init(void)
{
	if (init_display(spi_bus_index,chip_select)) {
		printk(KERN_INFO "Display initizization error \n");
		return -1;
	}
	printk(KERN_INFO "LCD init ok\n");
	lcd_set_address_window(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

	lcd_fill_screen(COLOR_BLUE);
	lcd_fill_rectangle(20, 20, 50,50, COLOR_GREEN);
	lcd_fill_rectangle(40, 40, 80, 80, COLOR_RED);
	lcd_put_string("Test KERNEL mode", 60, 60, Font_11x18, COLOR_RED,
		       COLOR_BLACK);
	lcd_update_screen();

	init_display_chardevice(&display_functions);
	return 0;
}

static void __exit display_module_exit(void)
{
	deinit_display_chardevice();
	lcd_deinit();
}

module_init(display_module_init);
module_exit(display_module_exit);
