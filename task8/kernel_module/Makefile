TARGET = task_notifications_timer

ifneq ($(KERNELRELEASE),)

obj-m := $(TARGET).o
$(TARGET)-objs := module_main.o reminders_model.o

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
	scp -P 8022 task_notifications_timer.ko root@localhost:
	scp -P 8022 task_notifications_timer.sh root@localhost:
endif