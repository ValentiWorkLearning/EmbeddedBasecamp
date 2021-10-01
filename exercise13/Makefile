
ifneq ($(KERNELRELEASE),)

obj-m += hrtimer.o
obj-m += interv.o
obj-m += mytimer.o
obj-m += tick.o

else

#KERNELDIR := $(BUILD_KERNEL)
KERNELDIR := "/lib/modules/$(shell uname -r)/build"
.PHONY: all clean
all:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) clean

endif
