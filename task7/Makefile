TARGET = task_timer_module

ifneq ($(KERNELRELEASE),)

obj-m := $(TARGET).o
$(TARGET)-objs := module_main.o procfs_handler.o sysfs_handler.o timer_track_module.o random_timer_handler.o

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
	sh test_timer_module.sh
deploy: all
	scp -P 8022 task_timer_module.ko root@localhost:
	scp -P 8022 test_timer_module.sh root@localhost:
endif