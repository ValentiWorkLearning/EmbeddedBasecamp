#ifndef GC9A01_CHARDEV_H
#define GC9A01_CHARDEV_H
#include <linux/types.h>

typedef u8* (*get_framebuffer_ptr_cb)(void);
typedef size_t (*get_framebuffer_size_cb)(void);
typedef void (*push_display_update_cb)(void);

typedef struct {
	push_display_update_cb p_display_updater;
	get_framebuffer_size_cb p_fb_size_getter;
	get_framebuffer_ptr_cb p_fb_getter;

} display_chardev_descriptor;

int init_display_chardevice(
	const display_chardev_descriptor *display_functions_holder);
void deinit_display_chardevice(void);

#endif