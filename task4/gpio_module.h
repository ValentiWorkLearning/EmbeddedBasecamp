#ifndef GPIO_MODULE_H
#define GPIO_MODULE_H

int gpio_init(unsigned int gpio);
int gpio_free(unsigned int gpio);
int gpio_set_value(unsigned int gpio, unsigned int value);
#endif