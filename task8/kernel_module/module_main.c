#include <linux/module.h>
#include <linux/init.h>

#include "reminders_model.h"
#include "module_common.h"
#include "tests.h"

static int __init reminder_module_init(void)
{
    test1();
    test2();
    test3();
    test4();
    return -1;
}

static void __exit reminder_module_exit(void)
{

}

MODULE_AUTHOR("Valentyn Korniienko");
MODULE_LICENSE("Dual BSD/GPL");

module_init(reminder_module_init);
module_exit(reminder_module_exit);