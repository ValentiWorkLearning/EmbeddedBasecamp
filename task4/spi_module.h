#ifndef SPI_MODULE_H
#define SPI_MODULE_H
#include <stdint.h>
#include <stddef.h>

void init_spi_module(void);
void spi_write(uint8_t *tx_buffer, uint16_t len);
void deinit_spi(void);
#endif