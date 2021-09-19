#include "gpio_module.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 64
#define GPIO_DIR "/sys/class/gpio"

int gpio_init(unsigned int gpio)
{
	int fd, len;
	char buf[BUF_SIZE];

	fd = open(GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);

	snprintf(buf, sizeof(buf), GPIO_DIR "/gpio%d/direction", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/direction");
		return fd;
	}

	write(fd, "out", sizeof("out"));
	close(fd);

	return 0;
}
int gpio_free(unsigned int gpio)
{
	int fd, len;
	char buf[BUF_SIZE];

	fd = open(GPIO_DIR "/unexport", O_WRONLY);
	if (fd < 0) {
		perror("gpio/unexport");
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);

	return 0;
}
int gpio_set_value(unsigned int gpio, unsigned int value)
{
	int fd;
	char buf[BUF_SIZE];

	snprintf(buf, sizeof(buf), GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-value");
		return fd;
	}

	if (value) {
		write(fd, "1", 2);
	} else {
		write(fd, "0", 2);
	}

	close(fd);
	return 0;
}
