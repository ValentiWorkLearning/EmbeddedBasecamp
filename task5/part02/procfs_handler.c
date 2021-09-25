#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include "currency_converter_common.h"
#include "module_common.h"

#define PROC_DIRECTORY  "conversion_value"
#define PROC_FILENAME   "value"

#define BUFFER_SIZE     40
static char *proc_buffer;
static size_t proc_msg_length;
static size_t proc_msg_read_pos;

static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file;
static conversion_value_changed_cb value_changed_cb;

static ssize_t procfs_read_callback(struct file *file_p, char __user *buffer, size_t length, loff_t *offset);
static ssize_t procfs_write_callback(struct file *file_p, const char __user *buffer, size_t length, loff_t *offset);

static struct proc_ops proc_fops = {
    .proc_read  = procfs_read_callback,
    .proc_write = procfs_write_callback,
};

static int create_buffer(char** p_buffer)
{
    *p_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (NULL == *p_buffer)
        return -ENOMEM;
    proc_msg_length = 0;

    return 0;
}

static void cleanup_buffer(char* p_buffer)
{
    if (p_buffer) {
        kfree(p_buffer);
        p_buffer = NULL;
    }
    proc_msg_length = 0;
}



static int crate_proc_entry(void)
{
    proc_dir = proc_mkdir(PROC_DIRECTORY, NULL);
    if (NULL == proc_dir)
        return -EFAULT;

    proc_file = proc_create(PROC_FILENAME, S_IFREG | S_IRUGO | S_IWUGO, proc_dir, &proc_fops);
    if (NULL == proc_file)
        return -EFAULT;

    return 0;
}

static void remove_procfs_entries(void)
{
    if (proc_file)
    {
        remove_proc_entry(PROC_FILENAME, proc_dir);
        proc_file = NULL;
    }
    if (proc_dir)
    {
        remove_proc_entry(PROC_DIRECTORY, NULL);
        proc_dir = NULL;
    }
}

static ssize_t procfs_read_callback(struct file *file_p, char __user *buffer, size_t length, loff_t *offset)
{
    size_t left;

    if (length > (proc_msg_length - proc_msg_read_pos))
        length = (proc_msg_length - proc_msg_read_pos);

    left = copy_to_user(buffer, &proc_buffer[proc_msg_read_pos], length);

    proc_msg_read_pos += length - left;

    if (left)
        printk(KERN_ERR MODULE_TAG "failed to read %zu from %zu chars\n", left, length);
    else
        printk(KERN_NOTICE MODULE_TAG "read %zu chars\n", length);

    return length - left;
}

static ssize_t procfs_write_callback(struct file *file_p, const char __user *buffer, size_t length, loff_t *offset)
{
    size_t msg_length;
    size_t left;

    if (length > BUFFER_SIZE)
    {
        printk(KERN_WARNING MODULE_TAG "reduse message length from %zu to %u chars\n", length, BUFFER_SIZE);
        msg_length = BUFFER_SIZE;
    }
    else
        msg_length = length;

    left = copy_from_user(proc_buffer, buffer, msg_length);

    proc_msg_length = msg_length - left;
    proc_msg_read_pos = 0;

    if (left)
        printk(KERN_ERR MODULE_TAG "failed to write %zu from %zu chars\n", left, msg_length);
    else
        printk(KERN_NOTICE MODULE_TAG "written %zu chars\n", msg_length);


    return length;
}


int init_procfs_handler(void)
{
        int err;

    err = create_buffer(&proc_buffer);
    if (err)
        goto error;

    err = crate_proc_entry();
    if (err)
        goto error;

    printk(KERN_NOTICE MODULE_TAG "procfs entry created\n");
    return 0;

    error:
    printk(KERN_ERR MODULE_TAG "procfs entry failed to create\n");
    remove_procfs_entries();
    cleanup_buffer(proc_buffer);
    return err;
}

void procfs_handler_deinit(void)
{
    remove_procfs_entries();
    cleanup_buffer(proc_buffer);
    printk(KERN_NOTICE MODULE_TAG "procfs entry exited\n");
}

void register_procfs_data_arrived_callback(conversion_value_changed_cb callback)
{
    value_changed_cb = callback;
}