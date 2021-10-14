obj-m += ds3231_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

tree:
	dtc -I dts -O dtb -o ds3231_device.dtbo ds3231_device.dts
	sudo cp ds3231_device.dtbo /boot/overlays/

test:
	sudo dmesg -C
	sudo insmod ds3231_device.ko
	sudo rmmod ds3231_device.ko
	dmesg
