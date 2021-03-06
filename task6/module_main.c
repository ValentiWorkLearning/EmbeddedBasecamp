#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/fs.h>

#include "module_common.h"
//https://olegkutkov.me/2018/03/14/simple-linux-character-device-driver/

static int memory_buffer_size = 1024;
module_param(memory_buffer_size, int, S_IRUGO);

#define MIN_MINOR_NUMBER 0
#define MAX_DEVICE_COUNT 1

#define MODULE_TITLE "my_messenger_chdev"

static unsigned char *device_memory_buffer = NULL;
static ssize_t chardev_buffer_rw_holder = 0;

static size_t total_messages_count = 0;


// procfs section
extern int initialize_procfs_handler(void);
extern void cleanup_procfs_entries(void);

extern void update_buffer_total_size(size_t current_buffer_size);
extern void update_remaining_buffer_size(size_t remainig_buffer_size);
extern void update_total_messages_count(size_t total_messages_count);


//sysfs section

extern int sysfs_handler_init(void);
extern void sysfs_handler_cleanup(void);
extern void set_on_clear_requested_callback(clear_message_buffer_cb callback);


// character device section
static struct cdev hcdev;
static struct class *devclass;

static int device_major_number = 0;

static ssize_t messenger_chardev_read(struct file *file, char *buf,
				      size_t count, loff_t *ppos)
{
	int ret;

	printk(KERN_INFO "Messenger: read from file %s\n",
	       file->f_path.dentry->d_iname);
	printk(KERN_INFO "Current chardev rw holder: %d",
	       chardev_buffer_rw_holder);

	if (count > chardev_buffer_rw_holder)
		count = chardev_buffer_rw_holder;
	if (*ppos != 0) {
		printk(KERN_INFO "=== read return : 0\n"); // EOF
		return 0;
	}

	ret = copy_to_user(buf, device_memory_buffer, count);
	if (ret) {
		printk(KERN_ERR "Messenger: copy_to_user failed: %d\n", ret);
		return -EFAULT;
	}
	chardev_buffer_rw_holder = 0; /* eof for cat */

	printk(KERN_INFO "Messenger: %zu bytes read\n", count);

	update_remaining_buffer_size(memory_buffer_size);

	return count;
}

static ssize_t messenger_chardev_write(struct file *filep, const char *buffer,
				       size_t len, loff_t *offset)
{
	int ret;

	printk(KERN_INFO "Messenger: write to file %s\n",
	       filep->f_path.dentry->d_iname);
	printk(KERN_INFO "Called chardev write with len : %d", len);

	chardev_buffer_rw_holder = len;
	if (chardev_buffer_rw_holder > memory_buffer_size)
		chardev_buffer_rw_holder = memory_buffer_size;

	ret = copy_from_user(device_memory_buffer, buffer,
			     chardev_buffer_rw_holder);
	if (ret) {
		printk(KERN_ERR "Messenger: copy_from_user failed: %d\n", ret);
		return -EFAULT;
	}

	printk(KERN_INFO "Messenger: %d bytes written\n",
	       chardev_buffer_rw_holder);

	update_remaining_buffer_size(memory_buffer_size - len);
	total_messages_count += 1;
	update_total_messages_count(total_messages_count);

	return chardev_buffer_rw_holder;
}


void cleanup_message_buffer(void)
{
	chardev_buffer_rw_holder = 0;
	memset(device_memory_buffer,0,memory_buffer_size * sizeof(*device_memory_buffer));
	update_remaining_buffer_size(memory_buffer_size);
}

struct file_operations chardev_file_operations = {
	.read = messenger_chardev_read,
	.write = messenger_chardev_write,
	.owner = THIS_MODULE
};

//https://stackoverflow.com/questions/11846594/how-can-i-programmatically-set-permissions-on-my-char-device

static char *messenger_devnode(struct device *dev, umode_t *mode)
{
	if (!mode)
		return NULL;

	*mode = 0666;

	printk(KERN_INFO "Messenger called with: %d\n", dev->devt);
	return NULL;
};

// character device section

static int __init chardev_init(void)
{
	int ret;
	dev_t device;
	ret = alloc_chrdev_region(&device, MIN_MINOR_NUMBER, MAX_DEVICE_COUNT,
				  MODULE_TITLE);

	device_major_number = MAJOR(device);
	if (ret < 0) {
		printk(KERN_ERR "=== Failed to allocate chardevice region \n");
		goto err;
	}
	cdev_init(&hcdev, &chardev_file_operations);

	ret = cdev_add(&hcdev, MKDEV(device_major_number, MIN_MINOR_NUMBER),
		       MAX_DEVICE_COUNT);
	if (ret < 0) {
		unregister_chrdev_region(MKDEV(device_major_number,
					       MIN_MINOR_NUMBER),
					 MAX_DEVICE_COUNT);
		printk(KERN_ERR "=== Failed to add char device! \n");
		goto err;
	}

	devclass = class_create(THIS_MODULE, MODULE_TITLE);

	devclass->devnode = messenger_devnode;
	device_memory_buffer = kmalloc(
		memory_buffer_size * sizeof(*device_memory_buffer), GFP_KERNEL);

	device_create(devclass, NULL, device, NULL, "%s_%d", "messenger",
		      MIN_MINOR_NUMBER);

	printk(KERN_INFO "======== module installed %d:[%d-%d] ===========\n",
	       MAJOR(device), MIN_MINOR_NUMBER, MINOR(device));

	initialize_procfs_handler();
	sysfs_handler_init();

	update_buffer_total_size(memory_buffer_size);
	update_remaining_buffer_size(memory_buffer_size);
	update_total_messages_count(total_messages_count);

	set_on_clear_requested_callback(cleanup_message_buffer);
	return 0;
err:
	if (device_memory_buffer)
		kfree(device_memory_buffer);
	return ret;
}

static void __exit chardev_exit(void)
{
	dev_t dev;
	dev = MKDEV(device_major_number, MIN_MINOR_NUMBER);
	device_destroy(devclass, dev);

	class_destroy(devclass);
	cdev_del(&hcdev);
	unregister_chrdev_region(MKDEV(device_major_number, MIN_MINOR_NUMBER),
				 MAX_DEVICE_COUNT);

	sysfs_handler_cleanup();
	cleanup_procfs_entries();

	if (device_memory_buffer)
		kfree(device_memory_buffer);

	printk(KERN_INFO "=============module removed==========");
}

MODULE_AUTHOR("Valentyn Korniienko");
MODULE_LICENSE("Dual BSD/GPL");

module_init(chardev_init);
module_exit(chardev_exit);