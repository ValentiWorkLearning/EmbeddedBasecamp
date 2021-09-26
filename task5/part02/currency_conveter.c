#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include "currency_converter_common.h"
#define CONVERSION_BUFFER_SIZE 200

static int conversion_factor = 31;

conversion_result_ready_cb convertion_ready_handler;

module_param( conversion_factor, int, 0 ); 

static char g_converionBuffer[CONVERSION_BUFFER_SIZE] = {};

void set_result_ready_cb(conversion_result_ready_cb callback)
{
    convertion_ready_handler = callback;
}

void set_conversion_factor(int new_factor)
{
    conversion_factor = new_factor;
}

void convert_currency(int to_convert)
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
    
    if(convertion_ready_handler)
        convertion_ready_handler(full,remainder);
}