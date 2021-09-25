#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/pci.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/device/class.h>

#include "currency_converter_common.h"

static conversion_factor_changed_cb on_conversion_factor_changed_cb;

#define LEN_MSG 160
static char buf_msg[LEN_MSG + 1] = "Hello from module!\n";

void set_conversion_changed_cb(conversion_factor_changed_cb callback)
{
    on_conversion_factor_changed_cb = callback;
}

/* sysfs show() method. Calls the show() method corresponding to the individual sysfs file */
static ssize_t curreccy_factor_show(struct class *class, struct class_attribute *attr, char *buf)
{
    strcpy(buf, buf_msg);
    printk("read %ld\n", (long)strlen(buf));
    return strlen(buf);
}

/* sysfs store() method. Calls the store() method corresponding to the individual sysfs file */
static ssize_t curreccy_factor_store(struct class *class, struct class_attribute *attr, const char *buf, size_t count)
{
    printk("write %ld\n", (long)count);
    strncpy(buf_msg, buf, count);
    buf_msg[count] = '\0';

    int32_t conversion_factor = 0;
    kstrtos32(buf_msg,10,&conversion_factor);

    if(conversion_factor <= 0 )
        return count;

    if (on_conversion_factor_changed_cb)
    {
        on_conversion_factor_changed_cb(conversion_factor);
    }
    return count;
}

CLASS_ATTR_RW(curreccy_factor);

static struct class *currency_factor_class;

int sysfs_handler_init(void)
{
    int res;
    currency_factor_class = class_create(THIS_MODULE, "currency-class");
    if (IS_ERR(currency_factor_class))
        printk("bad class create\n");
    res = class_create_file(currency_factor_class, &class_attr_curreccy_factor);
    /* <linux/device.h>
extern int __must_check class_create_file(struct class *class, const struct class_attribute *attr); */
    printk("'curreccy_factor' entry initialized\n");
    return res;
}

void sysfs_handler_cleanup(void)
{
    /* <linux/device.h>
extern void class_remove_file(struct class *class, const struct class_attribute *attr); */
    class_remove_file(currency_factor_class, &class_attr_curreccy_factor);
    class_destroy(currency_factor_class);
    return;
}