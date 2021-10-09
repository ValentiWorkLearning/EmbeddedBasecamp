#ifndef SYSFS_HANDLER_H
#define SYSFS_HANDLER_H
#include "module_common.h"

int sysfs_handler_init(void);
void sysfs_handler_cleanup(void);
void set_mode_change_request_callback(on_display_mode_changed_cb callback);

#endif