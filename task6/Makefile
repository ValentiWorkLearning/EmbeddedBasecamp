TARGET = task_main_module

ifneq ($(KERNELRELEASE),)

obj-m := $(TARGET).o
$(TARGET)-objs := module_main.o procfs_handler.o sysfs_handler.o

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
#scp -P 8022 task_main_module.ko root@localhost:

	- sudo insmod task_main_module.ko
	- cat /proc/messenger_dir/message_buffer_total_size
	- cat /proc/messenger_dir/message_buffer_remaining
	- cat /proc/messenger_dir/total_messages_proceeded
	- echo "12345678" > /dev/messenger_0
	- cat /proc/messenger_dir/message_buffer_total_size
	- cat /proc/messenger_dir/message_buffer_remaining
	- cat /proc/messenger_dir/total_messages_proceeded
	- cat /dev/messenger_0
	- echo "Hello world from the character device" > /dev/messenger_0
	- cat /proc/messenger_dir/message_buffer_total_size
	- cat /proc/messenger_dir/message_buffer_remaining
	- cat /proc/messenger_dir/total_messages_proceeded
	- cat /dev/messenger_0
	- echo "Testing sequence for the buffer cleanup" > /dev/messenger_0
	- cat /proc/messenger_dir/message_buffer_remaining
	- cat /sys/class/messenger-mod-sysfs/clear_buffer
	- echo 1 > /sys/class/messenger-mod-sysfs/clear_buffer
	- cat /proc/messenger_dir/message_buffer_remaining
	- sudo rmmod task_main_module
	- dmesg | tail -10
endif