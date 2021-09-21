BUILD_DIR=./bin/
SOURCE=./

gethostip:
	cd $(BUILD_DIR)
	hostname -I > host.txt

displaytestapp: gethostip main.c gc9a01.c gpio_module.c spi_module.c fonts.c buttons_module.c
	gcc -g -o $(BUILD_DIR)/displaytestapp main.c gc9a01.c gpio_module.c spi_module.c fonts.c buttons_module.c -I.
clean:
	$(RM) -rf $(BUILD_DIR)/*
test: displaytestapp
	$(BUILD_DIR)/displaytestapp
