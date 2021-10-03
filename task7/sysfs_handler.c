#include "procfs_handler.h"

#include <linux/version.h>
#include <linux/init.h>
#include <linux/device/class.h>

#define PROCFS_BUF_LEN 10

static on_display_mode_changed_cb on_received_mode_change_request_cb = NULL;

static char proc_buf[PROCFS_BUF_LEN] = "";

static struct class *display_mode_class;

void set_mode_change_request_callback(on_display_mode_changed_cb callback)
{
	on_received_mode_change_request_cb = callback;
}

static ssize_t display_mode_show(struct class *class,
				 struct class_attribute *attr, char *buf)
{
	printk(KERN_ERR MODULE_TAG
	       "Can't read from the displaying mode procfs entry!");
	return -EIO;
}

static ssize_t display_mode_store(struct class *class,
				  struct class_attribute *attr, const char *buf,
				  size_t count)
{
	int32_t clear_buf_int;

	if (count >= PROCFS_BUF_LEN)
		count = PROCFS_BUF_LEN - 1;

	printk("write %ld\n", (long)count);
	strncpy(proc_buf, buf, count);
	proc_buf[count] = '\0';

	int display_option_value = 0;
	kstrtos32(proc_buf, 10, &display_option_value);

	display_time_mode_t displaying_mode = display_time_mode_enum_end;

	switch (display_option_value) {
	case full_time_display:
		displaying_mode = full_time_display;
		break;
	case passed_seconds:
		displaying_mode = passed_seconds;
		break;
	default:
		break;
	}
	if (displaying_mode != display_time_mode_enum_end) {
		if (on_received_mode_change_request_cb)
			on_received_mode_change_request_cb(displaying_mode);
	}
	return count;
}
CLASS_ATTR_RW(display_mode);

int sysfs_handler_init(void)
{
	int res;
	display_mode_class = class_create(THIS_MODULE, "timer-mod-sys");
	if (IS_ERR(display_mode_class))
		printk("bad class create\n");
	res = class_create_file(display_mode_class, &class_attr_display_mode);
	printk("'display_mode' entry initialized\n");
	return res;
}

void sysfs_handler_cleanup(void)
{
	class_remove_file(display_mode_class, &class_attr_display_mode);
	class_destroy(display_mode_class);
	return;
}
