#
# sysfs examples
#

ifneq ($(KERNELRELEASE),)

obj-m += xxx.o
obj-m += xxm.o
obj-m += xxe.o

else

#KERNELDIR := $(BUILD_KERNEL)
KERNELDIR := "/lib/modules/$(shell uname -r)/build"
.PHONY: all clean
all:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) clean

endif
