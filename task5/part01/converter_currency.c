#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Valentyn Korniienko");
MODULE_DESCRIPTION("A simple currency convertor Linux module.");
MODULE_VERSION("0.01");

#define CONVERSION_BUFFER_SIZE 200

static int uahToEuro = 31;
static int uahAmount = 100;

static char g_converionBuffer[CONVERSION_BUFFER_SIZE] = {};

static int __init converter_currency_init(void)
{
    const int conversionResult = uahAmount * 100 / uahToEuro;
    const int full = conversionResult / 100;
    const int remainder = conversionResult % 100;

    snprintf(g_converionBuffer, CONVERSION_BUFFER_SIZE,"%s:%d\n","UAH" , uahAmount);
    printk("%s",g_converionBuffer);

    snprintf(g_converionBuffer, CONVERSION_BUFFER_SIZE,"%s:%d\n","Conversion value" , uahToEuro);
    printk("%s",g_converionBuffer);

    snprintf(g_converionBuffer, CONVERSION_BUFFER_SIZE,"%d.%d\n", full,remainder);
    printk("%s",g_converionBuffer);
    //printk(KERN_INFO "Hello!!!\n");
    return 0;
}

static void __exit converter_currency_exit(void)
{
    printk(KERN_INFO "Module unloaded\n");
}

module_init(converter_currency_init);
module_exit(converter_currency_exit);