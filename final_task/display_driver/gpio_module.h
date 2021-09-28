#ifndef GPIO_MODULE_H
#define GPIO_MODULE_H

int gpio_module_init_out(unsigned int gpio);
int gpio_module_init_in(unsigned int gpio);
int gpio_module_free(unsigned int gpio);
int gpio_module_set_value(unsigned int gpio, unsigned int value);
int gpio_module_get_value(unsigned int gpio);
#endif