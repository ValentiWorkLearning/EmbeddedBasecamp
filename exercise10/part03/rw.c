
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("valentyn Korniienko <valentyn.korniienko1@nure.ua>");
MODULE_DESCRIPTION("Procfs strings converter");
MODULE_VERSION("0.1");

#include "convertor_mode_enum.h"

#define MODULE_TAG      "strings_converter"
#define PROC_DIRECTORY  "string_converter"
#define PROC_FILENAME   "buffer"
#define BUFFER_SIZE     1024

ConversionMode_t active_conversion_mode = NoneConversion;

static char *proc_buffer_in;
static char *proc_buffer_out;

static size_t proc_msg_length;
static size_t proc_msg_read_pos;

static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file;

static ssize_t procfs_read_callback(struct file *file_p, char __user *buffer, size_t length, loff_t *offset);
static ssize_t procfs_write_callback(struct file *file_p, const char __user *buffer, size_t length, loff_t *offset);


extern int init_converter_module(void);
extern void deinit_converter_module(void);

extern void reverse_string_words(const char* incoming_string, char* output_buffer);
extern void to_upper_string(const char* incoming_string,char* output_buffer);
extern void copy_string_to_output(const char* incoming_string,char* output_buffer);

extern int init_sysfs_handler(void);
extern void sysfs_handler_cleanup(void);
extern void register_on_conversion_mode_changed_cb(conversion_mode_changed_cb_t callback);

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

static void handle_conversion_mode_changed(ConversionMode_t conversion_mode)
{
    active_conversion_mode = conversion_mode;
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

    left = copy_to_user(buffer, &proc_buffer_out[proc_msg_read_pos], length);

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

    left = copy_from_user(proc_buffer_in, buffer, msg_length);

    proc_msg_length = msg_length - left;
    proc_msg_read_pos = 0;

    if (left)
        printk(KERN_ERR MODULE_TAG "failed to write %zu from %zu chars\n", left, msg_length);
    else
        printk(KERN_NOTICE MODULE_TAG "written %zu chars\n", msg_length);

    switch (active_conversion_mode)
    {
    case NoneConversion:
        copy_string_to_output(proc_buffer_in,proc_buffer_out);
        break;
    case Flip:
        reverse_string_words(proc_buffer_in,proc_buffer_out);
        break;
    case Uppercase:
        to_upper_string(proc_buffer_in,proc_buffer_out);
        break;
    default:
        copy_string_to_output(proc_buffer_in,proc_buffer_out);
        break;
    }

    return length;
}


static int __init string_processor_init(void)
{
    int err;

    err = create_buffer(&proc_buffer_in);
    if (err)
        goto error;

    err = create_buffer(&proc_buffer_out);
    if (err)
        goto error;

    err = crate_proc_entry();
    if (err)
        goto error;

    init_converter_module();
    init_sysfs_handler();
    register_on_conversion_mode_changed_cb(&handle_conversion_mode_changed);
    printk(KERN_NOTICE MODULE_TAG "loaded\n");
    return 0;

error:
    printk(KERN_ERR MODULE_TAG "failed to load\n");
    remove_procfs_entries();
    cleanup_buffer(proc_buffer_in);
    cleanup_buffer(proc_buffer_out);
    deinit_converter_module();
    sysfs_handler_cleanup();
    return err;
}


static void __exit string_processor_exit(void)
{
    remove_procfs_entries();
    cleanup_buffer(proc_buffer_in);
    cleanup_buffer(proc_buffer_out);
    deinit_converter_module();
    sysfs_handler_cleanup();
    printk(KERN_NOTICE MODULE_TAG "exited\n");
}


module_init(string_processor_init);
module_exit(string_processor_exit);
