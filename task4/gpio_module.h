#ifndef GPIO_MODULE_H
#define GPIO_MODULE_H

int gpio_init_out(unsigned int gpio);
int gpio_init_in(unsigned int gpio);
int gpio_free(unsigned int gpio);
int gpio_set_value(unsigned int gpio, unsigned int value);
int gpio_get_value(unsigned int gpio);
#endif