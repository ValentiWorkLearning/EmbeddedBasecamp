
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

#include "gc9a01.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Valentyn Korniienko <valentyn.korniienko1@nure.ua>");
MODULE_DESCRIPTION("GC9A01 display driver");
MODULE_VERSION("0.1");

static int spi_bus_index = 0;
module_param(spi_bus_index, int, S_IRUGO);

static int __init display_module_init(void)
{
	init_display(spi_bus_index);
	return 0;
}

static void __exit display_module_exit(void)
{
	lcd_deinit();
}

module_init(display_module_init);
module_exit(display_module_exit);
