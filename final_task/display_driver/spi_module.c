#include "spi_module.h"
#include <linux/spi/spi.h>

// https://elixir.bootlin.com/linux/v5.15-rc3/source/include/linux/spi/spi.h#L763
static struct spi_controller *spi_module_block;

static struct spi_board_info gc9a01_spi_board_info = {
	.modalias = "gc9a01_spi_driver",
	.max_speed_hz = 400000,
	.bus_num = 0,
	.chip_select = 0,
	.mode = SPI_MODE_0
};
static struct spi_device *spi_device_master;

int init_spi_wrapper(int spi_bus_index)
{
	gc9a01_spi_board_info.bus_num = spi_bus_index;

	spi_module_block = spi_busnum_to_master(spi_bus_index);
	if (!spi_module_block) {
		printk(KERN_ERR
		       "Failed to get spi_busnum_to_master from the current board\n");
		goto error;
	}

	spi_device_master =
		spi_new_device(spi_module_block, &gc9a01_spi_board_info);
	if (!spi_device_master) {
		printk(KERN_ERR "FAILED to create spi_device_master.\n");
		return -ENODEV;
	}

	return 0;
error:
	return -ENODEV;
}

// https://wiki.t-firefly.com/en/AIO-3288C/driver_spi.html
//https://android.googlesource.com/kernel/msm/+/android-msm-flo-3.4-kitkat-mr0/drivers/staging/iio/dds/ad9910.c
int spi_wrapper_write(uint8_t *tx_buffer, uint16_t len)
{
	struct spi_transfer spi_transaction_item = { .tx_buf = tx_buffer,
						     .len = len };
	struct spi_message spi_message;
	spi_message_init(&spi_message);
	spi_message_add_tail(&spi_transaction_item, &spi_message);
	return spi_sync(spi_device_master, &spi_message);
}

void deinit_spi_wrapper(void)
{
	spi_unregister_device(spi_device_master);
	printk(KERN_INFO "Unregistred spi driver.\n");
}