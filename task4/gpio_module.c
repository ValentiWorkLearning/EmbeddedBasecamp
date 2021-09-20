#include "gpio_module.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 64
#define GPIO_DIR "/sys/class/gpio"


static int gpio_get_export_direction_fd(unsigned int gpio)
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
}

int gpio_init_out(unsigned int gpio)
{
	int fd = gpio_get_export_direction_fd(gpio);
	if(fd < 0)
	{
		perror("gpio_init_out");
		return fd;
	}

	write(fd, "out", sizeof("out"));
	close(fd);

	return 0;
}

int gpio_init_in(unsigned int gpio)
{
	int fd = gpio_get_export_direction_fd(gpio);
	if(fd < 0)
	{
		perror("gpio_init_in");
		return fd;
	}

	write(fd, "in", sizeof("in"));
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
int gpio_get_value(unsigned int gpio)
{
	int fd;
	char buf[BUF_SIZE];

	snprintf(buf, sizeof(buf), GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-value");
		return fd;
	}

	char input_port_value = 0;
	lseek(fd, 0, SEEK_SET);
	read(fd, &input_port_value, sizeof(char));

}