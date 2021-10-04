#ifndef NOTIFICATIONS_QUEUE
#define NOTIFICATIONS_QUEUE

#include <linked_list.h>
//#include <linux/list.h>

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
#endif
