#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>

MODULE_LICENSE("GPL");

unsigned long timer_interval_ns = 1e6;
static struct hrtimer hr_timer;

enum hrtimer_restart timer_callback( struct hrtimer *timer_for_restart )
{
  	ktime_t currtime , interval;
  	currtime  = ktime_get();
  	interval = ktime_set(0,timer_interval_ns); 
  	hrtimer_forward(timer_for_restart, currtime , interval);
	// set_pin_value(PIO_G,9,(cnt++ & 1)); //Toggle LED 
	return HRTIMER_RESTART;
}

static int __init timer_init(void) {
	ktime_t timer_to_set = ktime_set( 0, timer_interval_ns );
	hrtimer_init( &hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
	hr_timer.function = &timer_callback;
 	hrtimer_start( &hr_timer, timer_to_set, HRTIMER_MODE_REL );
	return 0;
}

static void __exit timer_exit(void) {
	int ret;
  	ret = hrtimer_cancel( &hr_timer );
  	if (ret) printk("The timer was still in use...\n"); //  Becasuse the timer will be active till the hrtimer_cancel call
  	printk("HR Timer module uninstalling\n");
	
}

module_init(timer_init);
module_exit(timer_exit);