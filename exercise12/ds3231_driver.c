#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "ds3231_registers.h"

static char *param1 = "N";
module_param(param1,charp,0660);

struct ds3231_data {
	struct i2c_client *drv_client;
	char sec;
	char min;
	char hour;
	int temp;
};

static struct ds3231_data ds_data;


static int ds_read_data(void)
{

	struct i2c_client *drv_client = ds_data.drv_client;

	if (drv_client == 0)
		return -ENODEV;

	ds_data.sec = i2c_smbus_read_byte_data(drv_client, REG_SEC);
	ds_data.min = i2c_smbus_read_byte_data(drv_client, REG_MIN);
	ds_data.hour = i2c_smbus_read_byte_data(drv_client, REG_HOUR);
	ds_data.temp = i2c_smbus_read_byte_data(drv_client, REG_TEMP_H);

	dev_info(&drv_client->dev, "read data\n");
	dev_info(&drv_client->dev, "sec: %02x\n", ds_data.sec);
	dev_info(&drv_client->dev, "min: %02x\n", ds_data.min);
	dev_info(&drv_client->dev, "hour: %02x\n", ds_data.hour);
	dev_info(&drv_client->dev, "temp: %d\n", ds_data.temp);

	return 0;
}


static ssize_t ds_time_show(struct class *class,
			    struct class_attribute *attr, char *buf)
{
	ds_read_data();

	sprintf(buf, "%02x:%02x:%02x\n", ds_data.hour, ds_data.min, ds_data.sec);
	return strlen(buf);
}


static ssize_t ds_temp_show(struct class *class,
			    struct class_attribute *attr, char *buf)
{
	ds_read_data();

	if (param1[0] == 'C') {
		sprintf(buf, "%d %s\n", ds_data.temp, param1);
	} 
	else if (param1[0] == 'F') {
		sprintf(buf, "%d %s\n", (ds_data.temp * 18 + 320)/10, param1);
	}
	else {
		sprintf(buf, "--\n");
	}

	return strlen(buf);
}


static int ds3231_probe(struct i2c_client *drv_client, const struct i2c_device_id *id)
{

	int reg = 0;
	struct device_node *node;
	
	dev_info(&drv_client->dev,
		"i2c client address is 0x%X\n", drv_client->addr);
	dev_info(&drv_client->dev, "i2c driver probed\n");

	ds_data.drv_client = drv_client;
	ds_read_data();

	node =	drv_client->dev.of_node;
	of_property_read_u32(node, "reg", &reg);
	of_property_read_string(node, "param1", (const char**) &param1);
	dev_info(&drv_client->dev, "reg = %d\n", reg);
	dev_info(&drv_client->dev, "param1 = %s\n", param1);

	return 0;
}


static int ds3231_remove(struct i2c_client *drv_client)
{

	ds_data.drv_client = 0;
	dev_info(&drv_client->dev, "i2c driver removed\n");
	
	return 0;
}


static const struct i2c_device_id ds3231_idtable [] = {
    { "my_ds3231", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, ds3231_idtable);

static struct i2c_driver ds3231_i2c_driver = {
    .driver = {
   	 .name = "my_ds3231",
    },

    .probe = ds3231_probe,
    .remove = ds3231_remove,
    .id_table = ds3231_idtable,
};

CLASS_ATTR_RO(ds_time);
CLASS_ATTR_RO(ds_temp);

static struct class *attr_class;


static int ds3231_init(void)
{
	int ret;

	ret = i2c_add_driver(&ds3231_i2c_driver);
	if (ret) {
		printk(KERN_ERR "ds3231: failed to add new i2c driver: %d\n", ret);
		return ret;
	}
	printk(KERN_INFO "ds3231: i2c driver created\n");

	printk(KERN_INFO "ds3231: PARAM = %s\n", param1);


	attr_class = class_create(THIS_MODULE, "ds3231");
	if (IS_ERR(attr_class)) {
		ret = PTR_ERR(attr_class);
		printk(KERN_ERR "ds3231: failed to create sysfs class: %d\n", ret);
		return ret;
	}
	printk(KERN_INFO "ds3231: sysfs class created\n");

	ret = class_create_file(attr_class, &class_attr_ds_time);
	if (ret) {
		printk(KERN_ERR "ds3231: failed to create sysfs class attribute ds_time: %d\n", ret);
		return ret;
	}

	ret = class_create_file(attr_class, &class_attr_ds_temp);
	if (ret) {
		printk(KERN_ERR "ds3231: failed to create sysfs class attribute ds_temp: %d\n", ret);
		return ret;
	}
		
	printk(KERN_INFO "ds3231: sysfs class attributes created\n");
	printk(KERN_INFO "ds3231: module loaded\n");

	return 0;
}


static void ds3231_exit(void)
{

	if (attr_class) {
		class_remove_file(attr_class, &class_attr_ds_time);
		class_remove_file(attr_class, &class_attr_ds_temp);
		printk(KERN_INFO "ds3231: sysfs class attributes removed\n");

		class_destroy(attr_class);
		printk(KERN_INFO "ds3231: sysfs class destroyed\n");
	}

	i2c_del_driver(&ds3231_i2c_driver);
	printk(KERN_INFO "ds3231: i2c driver deleted\n");
	printk(KERN_INFO "ds3231: module exited\n");
}


module_init(ds3231_init);
module_exit(ds3231_exit);

MODULE_AUTHOR("Maksym.Lipchanskyi <maxl@meta.ua>");
MODULE_DESCRIPTION("ds3231 Demo");
MODULE_LICENSE("GPL");
