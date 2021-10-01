/*******************************************************************************
* Copyright (c) 2015 Song Yang @ ittraining
* 
* All rights reserved.
* This program is free to use, but the ban on selling behavior.
* Modify the program must keep all the original text description.
*
* Email: onionys@ittraining.com.tw
* Blog : http://blog.ittraining.com.tw
*******************************************************************************/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ITtraining.com.tw");
MODULE_DESCRIPTION("A timer example.");

struct timer_list my_timer;

/*
 * TIMER FUNCTION
 * */

static void timer_function(struct timer_list* data){
	printk("Time up");
	// modify the timer for next time
	mod_timer(&my_timer, jiffies + HZ / 2);
}

/*
 * INIT MODULE
 * */
int init_module(void)
{
	printk("Hello My Timer\n");
    int ret;
	//  -- initialize the timer
    timer_setup(&my_timer,timer_function,0);

    ret = mod_timer(&my_timer,jiffies + HZ);
	if (ret)
		pr_err("%s: Timer firing failed\n", __func__);

	// -- TIMER START
	add_timer(&my_timer);
	printk("END: init_module() \n");
	return 0;
}

/*
 * CLEANUP MODULE
 * */
void cleanup_module(void)
{
	del_timer(&my_timer);
	printk("Goodbye\n");
}
