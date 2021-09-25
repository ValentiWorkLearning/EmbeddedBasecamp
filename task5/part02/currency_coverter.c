#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#define CONVERSION_BUFFER_SIZE 200

static int conversion_factor = 31;
module_param( conversion_factor, int, 0 ); 

static char g_converionBuffer[CONVERSION_BUFFER_SIZE] = {};

static void convert_currency(int to_convert)
{
    const int conversionResult = to_convert * 100 / conversion_factor;
    const int full = conversionResult / 100;
    const int remainder = conversionResult % 100;

    snprintf(g_converionBuffer, CONVERSION_BUFFER_SIZE,"%s:%d\n","UAH" , to_convert);
    printk("%s",g_converionBuffer);

    snprintf(g_converionBuffer, CONVERSION_BUFFER_SIZE,"%s:%d\n","Conversion value" , conversion_factor);
    printk("%s",g_converionBuffer);

    snprintf(g_converionBuffer, CONVERSION_BUFFER_SIZE,"%d.%d\n", full,remainder);
    printk("%s",g_converionBuffer);
    
    return 0;
}