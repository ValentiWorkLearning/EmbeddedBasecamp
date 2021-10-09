#include "gc9a01_chardev.h"
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/fs.h>

static display_chardev_descriptor *display_handler;

#define MIN_MINOR_NUMBER 0
#define MAX_DEVICE_COUNT 1
#define DEV_SYSFS_ENTRY_TITLE "gc9a01_chdev"
#define MODULE_TITLE "gc9a01_driver"

static struct cdev hcdev;
static struct class *devclass;

static int device_major_number = 0;

//https://stackoverflow.com/questions/11846594/how-can-i-programmatically-set-permissions-on-my-char-device

static char *display_devnode(struct device *dev, umode_t *mode)
{
	if (!mode)
		return NULL;

	*mode = 0666;

	printk(KERN_INFO "Display devnode called with: %d\n", dev->devt);
	return NULL;
};

static ssize_t display_chardev_read(struct file *file, char *buf, size_t count,
				    loff_t *ppos)
{
	return -EIO;
}

static ssize_t display_chardev_write(struct file *filep, const char *buffer,
				     size_t len, loff_t *offset)
{
	int ret;
	printk(KERN_INFO "Display: write to file %s\n",
	       filep->f_path.dentry->d_iname);
	printk(KERN_INFO "Called chardev write with len : %ld", len);
	if (len > display_handler->p_fb_size_getter())
		return -EIO;
	ret = copy_from_user(display_handler->p_fb_getter(), buffer, len);
	if (ret) {
		printk(KERN_ERR "GC9A01: copy_from_user failed: %d\n", ret);
		return -EFAULT;
	}
	printk(KERN_INFO "GC9A01: %ld bytes written\n", len);

	display_handler->p_display_updater();
	return len;
}

struct file_operations display_device_file_operations = {
	.read = display_chardev_read,
	.write = display_chardev_write,
	.owner = THIS_MODULE
};

int init_display_chardevice(const display_chardev_descriptor *display_functions)
{
	display_handler = display_functions;

	int ret;
	dev_t device;
	ret = alloc_chrdev_region(&device, MIN_MINOR_NUMBER, MAX_DEVICE_COUNT,
				  MODULE_TITLE);

	device_major_number = MAJOR(device);
	if (ret < 0) {
		printk(KERN_ERR "=== Failed to allocate chardevice region \n");
		goto err;
	}
	cdev_init(&hcdev, &display_device_file_operations);

	ret = cdev_add(&hcdev, MKDEV(device_major_number, MIN_MINOR_NUMBER),
		       MAX_DEVICE_COUNT);
	if (ret < 0) {
		unregister_chrdev_region(MKDEV(device_major_number,
					       MIN_MINOR_NUMBER),
					 MAX_DEVICE_COUNT);
		printk(KERN_ERR "=== Failed to add char device! \n");
		goto err_allocate_chardev_region_failed;
	}

	devclass = class_create(THIS_MODULE, DEV_SYSFS_ENTRY_TITLE);

	devclass->devnode = display_devnode;
	device_create(devclass, NULL, device, NULL, "%s_%d", "framebuffer",
		      MIN_MINOR_NUMBER);

	printk(KERN_INFO "======== module installed %d:[%d-%d] ===========\n",
	       MAJOR(device), MIN_MINOR_NUMBER, MINOR(device));

err_allocate_chardev_region_failed:
    unregister_chrdev_region(MKDEV(device_major_number, MIN_MINOR_NUMBER),
				 MAX_DEVICE_COUNT);
err:
    return 0;


    return ret;
}

void deinit_display_chardevice(void)
{
	dev_t dev;
	dev = MKDEV(device_major_number, MIN_MINOR_NUMBER);
	device_destroy(devclass, dev);

	class_destroy(devclass);
	cdev_del(&hcdev);
	unregister_chrdev_region(MKDEV(device_major_number, MIN_MINOR_NUMBER),
				 MAX_DEVICE_COUNT);
}