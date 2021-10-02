BUILD_KERNEL = /home/valenti/Development/Embedded/exercise8/kernel_build

KERNELDIR := $(BUILD_KERNEL)
#KERNELDIR := "/lib/modules/$(shell uname -r)/build"

PWD = $(shell pwd)

TARGET1 = lab1_interrupt
TARGET2 = mod_tasklet
TARGET3 = mod_workqueue
TARGET4 = mod_ser

obj-m	:=	$(TARGET1).o $(TARGET2).o $(TARGET3).o $(TARGET4).o

all: default clean 

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	@rm -f *.o .*.cmd .*.flags *.mod.c *.order
	@rm -f .*.*.cmd *.symvers *~ *.*~
	@rm -fR .tmp*
	@rm -rf .tmp_versions

disclean: clean
	@rm -f *.ko
