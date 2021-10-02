TARGET = task_main_module

ifneq ($(KERNELRELEASE),)

obj-m := $(TARGET).o
$(TARGET)-objs := module_main.o procfs_handler.o

else
BUILD_KERNEL = /home/valenti/Development/Embedded/exercise8/kernel_build

KERNELDIR := $(BUILD_KERNEL)
#KERNELDIR := "/lib/modules/$(shell uname -r)/build"
.PHONY: all clean
all:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) clean
test:
	- sudo insmod task_main_module.ko
	- echo "12345678" > /dev/messenger_0
	- cat /dev/messenger_0
	- echo "Hello world from the character device" > /dev/messenger_0
	- cat /dev/messenger_0
	- cat /proc/
	- sudo rmmod task_main_module
	- dmesg | tail -10
endif