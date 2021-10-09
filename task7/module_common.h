#ifndef MODULE_COMMON_H
#define MODULE_COMMON_H

#include <linux/types.h>

#define MODULE_TAG "==TIME_PROCESS_MOD=="
typedef enum display_time_mode {
	passed_seconds,
	full_time_display,
	display_time_mode_enum_end
} display_time_mode_t;

typedef void (*on_display_mode_changed_cb)(display_time_mode_t);

typedef void (*on_procfs_entry_has_been_used_cb)(void);
typedef void (*on_procfs_entry_request_absolute_time_cb)(void);

#endif