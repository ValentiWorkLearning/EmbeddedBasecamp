TARGET = task_main_module

ifneq ($(KERNELRELEASE),)

obj-m := $(TARGET).o
#$(TARGET)-objs := task_main_module.o

else
BUILD_KERNEL = /home/valenti/Development/Embedded/exercise8/kernel_build

#KERNELDIR := $(BUILD_KERNEL)
KERNELDIR := "/lib/modules/$(shell uname -r)/build"
.PHONY: all clean
all:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) clean
test:
	- sudo insmod task_main_module.ko
	- echo "12345678" > /dev/messenger_0
	- cat /dev/messenger_0
	- sudo rmmod task_main_module
endif