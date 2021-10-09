#include <linux/version.h>
#include <linux/init.h>
#include <linux/device/class.h>

#include "module_common.h"


#define PROCFS_BUF_LEN 10
#define CLEAR_BUF_ARG 1

static clear_message_buffer_cb on_received_clear_request_cb = NULL;

static char proc_buf[PROCFS_BUF_LEN] = "";

static struct class * clear_buffer_class;

void set_on_clear_requested_callback(clear_message_buffer_cb callback)
{
    on_received_clear_request_cb=callback;
}

static ssize_t clear_buffer_show(struct class *class, struct class_attribute *attr, char *buf)
{
    printk(KERN_ERR MODULE_TAG "Can't read from the clear buffer procfs entry!");
    return -EIO;
}

static ssize_t clear_buffer_store(struct class *class, struct class_attribute *attr, const char *buf, size_t count)
{
    int32_t clear_buf_int;

    if(count >= PROCFS_BUF_LEN)
        count = PROCFS_BUF_LEN - 1;

    printk("write %ld\n", (long)count);
    strncpy(proc_buf, buf, count);
    proc_buf[count] = '\0';

    clear_buf_int = 0;
    kstrtos32(proc_buf,10,&clear_buf_int);
    if(clear_buf_int == CLEAR_BUF_ARG )
    {
        if(on_received_clear_request_cb)
            on_received_clear_request_cb();
    }
    return count;
}
CLASS_ATTR_RW(clear_buffer);


int sysfs_handler_init(void)
{
    int res;
    clear_buffer_class = class_create(THIS_MODULE, "messenger-mod-sysfs");
    if (IS_ERR(clear_buffer_class))
        printk("bad class create\n");
    res = class_create_file(clear_buffer_class, &class_attr_clear_buffer);
    printk("'clear_buffer' entry initialized\n");
    return res;
}

void sysfs_handler_cleanup(void)
{
    class_remove_file(clear_buffer_class, &class_attr_clear_buffer);
    class_destroy(clear_buffer_class);
    return;
}
