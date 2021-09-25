obj-m += converter_currency.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
	sudo dmesg -C
	sudo insmod converter_currency.ko uahToEuro=31 uahAmount=200
	sudo rmmod converter_currency.ko
	dmesg
