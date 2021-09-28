#include "gpio_module.h"
#include <linux/gpio.h>
// https://www.kernel.org/doc/html/v4.19/driver-api/gpio/consumer.html#using-gpios
// https://01.org/linuxgraphics/gfx-docs/drm/driver-api/gpio/driver.html
// https://android.googlesource.com/kernel/msm/+/android-msm-flo-3.4-kitkat-mr0/drivers/watchdog/wm831x_wdt.c
// https://lwn.net/Articles/532714/
int gpio_module_init_out(unsigned int gpio)
{
	return 0;
}

int gpio_module_init_in(unsigned int gpio)
{
	return 0;
}

int gpio_module_free(unsigned int gpio)
{
	return 0;
}
int gpio_module_set_value(unsigned int gpio, unsigned int value)
{
	return 0;
}
int gpio_module_get_value(unsigned int gpio)
{
	return 0;
}