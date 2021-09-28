TARGET = gc9a01_module

ifneq ($(KERNELRELEASE),)

obj-m := $(TARGET).o
$(TARGET)-objs := gc9a01.o  fonts.o gpio_module.o spi_module.o module_main.o

else

#KERNELDIR := $(BUILD_KERNEL)
KERNELDIR := "/lib/modules/$(shell uname -r)/build"
.PHONY: all clean
all:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) clean
endif