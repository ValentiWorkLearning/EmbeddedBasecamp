#ifndef SPI_MODULE_H
#define SPI_MODULE_H
#include <linux/types.h>

int init_spi_wrapper(int spi_bus_index);
int spi_wrapper_write(uint8_t *tx_buffer, size_t len);
void deinit_spi_wrapper(void);
#endif