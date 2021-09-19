BUILD_DIR=./bin/
SOURCE=./

displaytestapp: main.c gc9a01.c gpio_module.c spi_module.c fonts.c
	gcc -g -o $(BUILD_DIR)/displaytestapp main.c gc9a01.c gpio_module.c spi_module.c fonts.c -I.
clean:
	$(RM) -rf $(BUILD_DIR)/*
test: displaytestapp
	$(BUILD_DIR)/displaytestapp
