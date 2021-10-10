#ifndef NOTIFICATIONS_QUEUE
#define NOTIFICATIONS_QUEUE


#include <linked_list.h>

#ifdef __cplusplus
extern "C" {
#endif
void add_new_user(const char* user_name);
void add_user_reminder(const char* user_name, const char* reminder_title, int actual_time);

void remove_user(const char* user_name);
void remove_user_notification(const char* user_name, int notification_number);

const char* get_all_reminders(void);
const char* get_user_reminders(const char* user_name);

void show_all_users(void);
void show_all_reminders(void);
void show_user_reminders(const char* user_name);

void cleanup_users(void);
void cleanup_reminders(void);
void handle_timer_tick(void);

struct reminder;

typedef void(*notification_queue_enumerator)(struct reminder*);
typedef void(*notifications_queue_changed)(void);

void for_each_notification_in_queue(notification_queue_enumerator);

const char* get_notification_username(struct reminder*);
const char* get_notification_text(struct reminder*);

size_t get_notifications_total_count(void);

void set_notification_queue_changed_callback(notifications_queue_changed callback);
void set_notification_is_about_to_remove_callback(notification_queue_enumerator);

bool is_notifications_list_empty(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
