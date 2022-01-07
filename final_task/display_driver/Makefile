TARGET = gc9a01_module

ifneq ($(KERNELRELEASE),)

obj-m := $(TARGET).o
$(TARGET)-objs := gc9a01.o  fonts.o gpio_module.o spi_module.o module_main.o gc9a01_chardev.o

else

KERNELDIR := $(BUILD_KERNEL)
#KERNELDIR := "/lib/modules/$(shell uname -r)/build"
.PHONY: all clean
all:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) clean
deploy: all
#	scp gc9a01_module.ko pi@192.168.0.197:~/display_driver_test
	scp display_driver_apply_overlay.sh pi@192.168.0.197:~/display_driver_test
	scp debug_read_from_debufs.sh pi@192.168.0.197:~/display_driver_test
	scp spidev_disabled.dts pi@192.168.0.197:~/display_driver_test
endif