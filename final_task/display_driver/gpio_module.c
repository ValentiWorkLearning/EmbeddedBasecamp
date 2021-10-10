#include "gpio_module.h"
#include <linux/gpio.h>
// https://www.kernel.org/doc/html/v4.19/driver-api/gpio/consumer.html#using-gpios
// https://01.org/linuxgraphics/gfx-docs/drm/driver-api/gpio/driver.html
// https://android.googlesource.com/kernel/msm/+/android-msm-flo-3.4-kitkat-mr0/drivers/watchdog/wm831x_wdt.c
// https://lwn.net/Articles/532714/

#define FORMAT_BUFFER_SIZE 64
static char format_buffer[FORMAT_BUFFER_SIZE];

int gpio_module_init_out(unsigned int gpio)
{
	snprintf(format_buffer, FORMAT_BUFFER_SIZE, "GPIO_%d", gpio);
	gpio_request(gpio, format_buffer);

	return gpio_direction_output(gpio, 0);
}

int gpio_module_init_in(unsigned int gpio)
{
	snprintf(format_buffer, FORMAT_BUFFER_SIZE, "GPIO_%d", gpio);
	gpio_request(gpio, format_buffer);

	return gpio_direction_input(gpio);
}

void gpio_module_free(unsigned int gpio)
{
	gpio_free(gpio);
}
void gpio_module_set_value(unsigned int gpio, unsigned int value)
{
	gpio_set_value(gpio, value);
}
int gpio_module_get_value(unsigned int gpio)
{
	return gpio_get_value(gpio);
}