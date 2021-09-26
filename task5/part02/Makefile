TARGET = currency_convertor_module

ifneq ($(KERNELRELEASE),)

obj-m := $(TARGET).o
$(TARGET)-objs := currency_conveter.o  module_main.o procfs_handler.o sysfs_handler.o

else

#KERNELDIR := $(BUILD_KERNEL)
KERNELDIR := "/lib/modules/$(shell uname -r)/build"
.PHONY: all clean
all:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) clean
test:
	- sudo insmod currency_convertor_module.ko

	- echo 12345 > /proc/conversion_value/value
	- cat /proc/conversion_value/value
	- sudo sh -c "echo 43 >> /sys/class/currency-class/currency_factor"
	- cat /proc/conversion_value/value

	- dmesg | tail -10

	- sudo rmmod currency_convertor_module
endif