#include "module_common.h"
#include "currency_converter_common.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("valentyn Korniienko <valentyn.korniienko1@nure.ua>");
MODULE_DESCRIPTION("Sysfs/procfs currency converter");
MODULE_VERSION("0.1");

extern int init_procfs_handler(void);
extern void procfs_handler_deinit(void);
extern char* get_proc_output_buffer(void);
extern int get_procfs_output_buffer_size(void);

extern int sysfs_handler_init(void);
extern void sysfs_handler_cleanup(void);


extern void register_procfs_data_arrived_callback(conversion_value_changed_cb);
extern void set_conversion_changed_cb(conversion_factor_changed_cb callback);

extern void set_result_ready_cb(conversion_result_ready_cb);
extern void convert_currency(int to_convert);
extern void set_conversion_factor(int new_factor);


static int last_converted_value_request = 0;

static void conversion_factor_changed_handler(int new_factor)
{
    set_conversion_factor(new_factor);
    convert_currency(last_converted_value_request);
}

static void conversion_result_ready(int full, int remainder )
{
    printk("conversion_result_ready %d, %d\n", full,remainder);
    char* procfs_message_buffer = get_proc_output_buffer();
    int procfs_message_buffer_size = get_procfs_output_buffer_size();

    snprintf(procfs_message_buffer, procfs_message_buffer_size,"%d.%d\n", full,remainder);
}
static void handle_procfs_data_arrived(char* new_value_to_convert)
{
    printk(KERN_DEBUG "handle_procfs_data_arrived");
    int32_t new_value = 0;
    kstrtos32(new_value_to_convert,10,&new_value);
    convert_currency(new_value);
    last_converted_value_request = new_value;
}

static int __init currency_converter_init(void)
{
    int err = init_procfs_handler();
    if (err)
        goto error;
    err = sysfs_handler_init();
    if (err)
        goto error;

    set_conversion_changed_cb(&conversion_factor_changed_handler);
    set_result_ready_cb(&conversion_result_ready);
    register_procfs_data_arrived_callback(&handle_procfs_data_arrived);
    return 0;
error:
    sysfs_handler_cleanup();
    procfs_handler_deinit();
    return err;
}

static void __exit currency_converter_exit(void)
{
    sysfs_handler_cleanup();
    procfs_handler_deinit();
}

module_init(currency_converter_init);
module_exit(currency_converter_exit);
