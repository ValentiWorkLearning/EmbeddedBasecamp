#include "spi_module.h"
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

static int fd;


static const char *device = "/dev/spidev0.0";
static uint8_t mode = 0;
static uint8_t bits = 8;
static uint32_t speed = 5000000;
static uint16_t delay = 0;

void init_spi_module(){
    fd = open(device, O_RDWR);
	if (fd < 0)
		printf("can't open device\n");

	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		printf("can't set spi mode\n");
	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		printf("can't get spi mode\n");

	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		printf("can't set bits per word\n");
	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		printf("can't get bits per word\n");

	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		printf("can't set max speed hz\n");
	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		printf("can't get max speed hz\n");
}

void spi_write(uint8_t *tx_buffer, uint16_t len){
    	int ret;

	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx_buffer,
		.rx_buf = (unsigned long)NULL,
		.len = len,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		printf("can't send spi message\n");
}

void deinit_spi(){
    close(fd);
}