#ifndef PROCFS_HANDLER_H
#define PROCFS_HANDLER_H
#include "module_common.h"

int init_procfs_handler(void);
void cleanup_procfs_handler(void);

void set_procfs_entry_has_been_used_cb(
	on_procfs_entry_has_been_used_cb callback);
void set_procfs_entry_request_absolute_current_time(
	on_procfs_entry_request_absolute_time_cb callback);

size_t procfs_get_last_used_total_buffer_size(void);
void procfs_set_last_used_message_size(size_t last_used_message_size);
char *procfs_get_last_used_buffer(void);

size_t procfs_get_absolute_time_buffer_size(void);
void procfs_set_absoulute_timer_message_size(size_t last_used_message_size);
char *procfs_get_absolute_time_buffer(void);

#endif