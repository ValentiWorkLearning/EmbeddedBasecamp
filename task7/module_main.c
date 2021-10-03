#include <linux/module.h>
#include <linux/device.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/fs.h>

#include "module_common.h"

#include "sysfs_handler.h"
#include "procfs_handler.h"
#include "timer_track_module.h"

static display_time_mode_t current_displaying_mode = passed_seconds;

static void format_procfs_passed_seconds(void)
{
	char *procfs_last_used_buffer = procfs_get_last_used_buffer();
	size_t total_buffer_size = procfs_get_last_used_total_buffer_size();
	uint32_t seconds_since_last_use =
		timer_handler_get_seconds_last_used_value();
	snprintf(procfs_last_used_buffer, total_buffer_size,
		 "Seconds since last procfs use: %d \n",
		 seconds_since_last_use);
	procfs_set_last_used_message_size(strlen(procfs_last_used_buffer));
}

static void format_procfs_passed_fulldate(void)
{
	char *procfs_last_used_buffer = procfs_get_last_used_buffer();
	size_t total_buffer_size = procfs_get_last_used_total_buffer_size();
	
    struct tm fill_up;
    timer_handler_get_last_used_as_tm(&fill_up);

	snprintf(procfs_last_used_buffer, total_buffer_size,
		 "Full date last procfs use: %u:%u:%u \n", fill_up.tm_hour, fill_up.tm_min,
		 fill_up.tm_sec);
	procfs_set_last_used_message_size(strlen(procfs_last_used_buffer));
}

void procfs_entry_has_been_used_handler(void)
{
	switch (current_displaying_mode) {
	case passed_seconds:
		format_procfs_passed_seconds();
		break;
	case full_time_display:
		format_procfs_passed_fulldate();
		break;
	default:
		break;
	}
	timer_handler_refresh_last_used_time();
}

void procfs_entry_request_absolut_time_handler(void)
{
	struct tm *absoulte_time = timer_handler_get_absolute_time_value();
	char *absolute_time_value_buffer = procfs_get_absolute_time_buffer();
	size_t abslute_time_buffer_size =
		procfs_get_absolute_time_buffer_size();

	snprintf(absolute_time_value_buffer, abslute_time_buffer_size,
		 "Absolute time: %u:%u:%u \n", absoulte_time->tm_hour,
		 absoulte_time->tm_min, absoulte_time->tm_sec);
	procfs_set_absoulute_timer_message_size(
		strlen(absolute_time_value_buffer));
}


void displaying_mode_changed_handler(display_time_mode_t new_mode)
{
    current_displaying_mode = new_mode;
}
static int __init time_track_module_init(void)
{
	sysfs_handler_init();
	init_procfs_handler();
	init_timer_module();

    set_mode_change_request_callback(displaying_mode_changed_handler);

	set_procfs_entry_has_been_used_cb(procfs_entry_has_been_used_handler);
	set_procfs_entry_request_absolute_current_time(
		procfs_entry_request_absolut_time_handler);
	return 0;
}

static void __exit time_track_module_exit(void)
{
	cleanup_procfs_handler();
	sysfs_handler_cleanup();
	deinit_timer_module();
}

MODULE_AUTHOR("Valentyn Korniienko");
MODULE_LICENSE("Dual BSD/GPL");

module_init(time_track_module_init);
module_exit(time_track_module_exit);