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
extern int sysfs_handler_init(void);
extern void sysfs_handler_cleanup(void);

static int __init currency_converter_init(void)
{
    int err = init_procfs_handler();
    if (err)
        goto error;
    err = sysfs_handler_init();
    if (err)
        goto error;
    return err;
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
