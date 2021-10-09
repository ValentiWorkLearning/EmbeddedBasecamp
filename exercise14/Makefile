BUILD_DIR=./bin/
SOURCE=./


memtest_app:userspace_mem_test.c
	gcc -g -o $(BUILD_DIR)/memtest_app userspace_mem_test.c -I.
clean:
	$(RM) -rf $(BUILD_DIR)/*
test: memtest_app
	$(BUILD_DIR)/memtest_app
