TARGET = strings_processor_module

ifneq ($(KERNELRELEASE),)

obj-m := $(TARGET).o
$(TARGET)-objs := strings_processor.o rw.o sysfs_handler.o

else

#KERNELDIR := $(BUILD_KERNEL)
KERNELDIR := "/lib/modules/$(shell uname -r)/build"
.PHONY: all clean
all:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) clean
test: all
	- sudo insmod strings_processor_module.ko

	- sudo sh -c "echo 1 >> /sys/class/conv-class/conv_conf"
	- echo 'hello words 12345 321' > /proc/string_converter/buffer
	- cat /proc/string_converter/buffer

	- sudo sh -c "echo 2 >> /sys/class/conv-class/conv_conf"
	- echo 'hello words 12345 321' > /proc/string_converter/buffer
	- cat /proc/string_converter/buffer

	- sudo sh -c "echo 0 >> /sys/class/conv-class/conv_conf"
	- echo 'hello words 12345 321' > /proc/string_converter/buffer
	- cat /proc/string_converter/buffer

	- sudo rmmod strings_processor_module.ko
endif
