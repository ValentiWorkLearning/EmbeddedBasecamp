#include "random_timer_handler.h"

#include <linux/timer.h>
#include <linux/types.h>
#include <linux/random.h>
#include <linux/moduleparam.h>

#include "module_common.h"

static new_random_value_arrived_cb new_random_ready;

struct timer_list random_generate_timer;

static uint rand_min = 0;
static uint rand_max = 5;
static uint timer_period = 1;

module_param(rand_min, uint, 0);
module_param(rand_max, uint, 0);
module_param(timer_period, uint, 0);

#define secs_to_jiffies(_secs) msecs_to_jiffies((_secs)*1000)

void set_new_random_value_arrived_callback(new_random_value_arrived_cb callback)
{
	new_random_ready = callback;
}

static void random_timer_callback(struct timer_list *data)
{
	uint rand;
	uint random_value;

	printk(KERN_NOTICE MODULE_TAG "Random timer expired!");

	get_random_bytes(&rand, sizeof(rand));

	random_value = rand % (rand_max - rand_min + 1) + rand_min;

	if (new_random_ready)
		new_random_ready(random_value);

	mod_timer(&random_generate_timer,
		  jiffies + secs_to_jiffies(timer_period));
}

void init_random_timer(void)
{
	timer_setup(&random_generate_timer, random_timer_callback, 0);
	mod_timer(&random_generate_timer,
		  jiffies + secs_to_jiffies(timer_period));
}

void deinit_random_timer(void)
{
	del_timer(&random_generate_timer);
}