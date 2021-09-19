BUILD_DIR=./bin/
SOURCE=./

dislaytestapp: main.c gc9a01.c gpio_module.c spi_module.c fonts.c
	gcc -o $(BUILD_DIR)/dislaytestapp main.c gc9a01.c gpio_module.c spi_module.c fonts.c -I.
clean:
	$(RM) -rf $(BUILD_DIR)/*