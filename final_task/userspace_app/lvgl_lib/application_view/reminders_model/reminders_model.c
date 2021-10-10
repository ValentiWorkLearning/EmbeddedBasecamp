 #include "reminders_model.h"

 #include <stdbool.h>
 #include <string.h>
 #include <stdlib.h>
 #include <stdio.h>

 struct user;

 typedef struct reminder
 {
     struct list_head list;
     struct list_head user_notify_list;
     const char* notification_text;
     struct user* belongs_to;
     int ellapse_time;

 }reminder_t;


 typedef struct user
 {
     struct list_head list;
     const char* user_name;
     struct list_head notifications_list;
 }user_t;

 LIST_HEAD(users_list);
 LIST_HEAD(reminders_list);
 static notifications_queue_changed callback_queue_changed;
 static notification_queue_enumerator callback_notification_is_about_to_be_removed;

 static user_t* find_user_entry(const char* user_name);

 static bool is_user_exists(const char* requested_name)
 {
     return find_user_entry(requested_name) != NULL;
 }

 static void user_dtor(user_t* destructed_user)
 {
     free(destructed_user->user_name);
     free(destructed_user);
 }
 static void reminder_dtor(reminder_t* destructed_reminder)
 {
     free(destructed_reminder->notification_text);
     free(destructed_reminder);
 }

 static user_t* find_user_entry(const char* user_name)
 {
     struct list_head *ptr;
     user_t *entry;
     list_for_each(ptr,&users_list)
     {
         entry = list_entry(ptr,user_t,list);
         if(strcmp(user_name,entry->user_name) == 0)
             return entry;
     }
     return NULL;
 }

 static void insert_reminder_entry_to_global_list(reminder_t* new_entry)
 {

     struct list_head* ptr;
     struct reminder* entry;
     list_for_each(ptr,&reminders_list)
     {
         entry = list_entry(ptr,struct reminder,list);
         if(entry->ellapse_time > new_entry->ellapse_time)
         {
             list_add_tail(&new_entry->list,ptr);
             return;
         }
     }
     list_add_tail(&new_entry->list,&reminders_list);
 }

 void add_new_user(const char* user_name)
 {
     if(!is_user_exists(user_name))
     {

         user_t* new_user = malloc(sizeof(user_t));
         if(!new_user)
             return;

         const size_t required_memory_for_string = strlen(user_name)+1;
         char* string_buffer = malloc(required_memory_for_string);
         if(!string_buffer){
             free(new_user);
             return;
         }
         strcpy(string_buffer,user_name);

         INIT_LIST_HEAD(&new_user->notifications_list);
         new_user->user_name = string_buffer;
         list_add_tail(&new_user->list,&users_list);

     }
 }


 void add_user_reminder(const char* user_name, const char* reminder_title, int actual_time)
 {
     user_t* p_user_entry = find_user_entry(user_name);
     if(!p_user_entry)
         return;
     reminder_t* new_reminder = malloc(sizeof (reminder_t));
     if(!new_reminder)
         return;
     new_reminder->notification_text = malloc(strlen(reminder_title)+1);
     if(!new_reminder->notification_text)
     {
         free(new_reminder);
         return;
     }
     strcpy(new_reminder->notification_text,reminder_title);

     new_reminder->ellapse_time = actual_time;

     INIT_LIST_HEAD(&new_reminder->list);
     INIT_LIST_HEAD(&new_reminder->user_notify_list);

     list_add_tail(&new_reminder->user_notify_list,&p_user_entry->notifications_list);

     insert_reminder_entry_to_global_list(new_reminder);
     new_reminder->belongs_to = p_user_entry;
 }

 void remove_user(const char* user_name)
 {
     user_t* user_it = NULL;
     struct list_head* pos, *q;
     list_for_each_safe(pos,q,&users_list)
     {
         user_it = list_entry(pos,user_t,list);
         if(strcmp(user_it->user_name,user_name) == 0)
         {
             list_del(pos);
             user_dtor(user_it);
         }
     }
 }

 void remove_user_notification(const char* user_name, int notification_number)
 {
     user_t* p_user = find_user_entry(user_name);
     if(!p_user)
         return;
     int notification_idx = notification_number - 1;

     bool found_notification_in_list = false;
     int loop_counter = 0;
     reminder_t* reminder_it_user = NULL;
     struct list_head* pos_user, *q_user;
     list_for_each_safe(pos_user,q_user,&p_user->notifications_list)
     {
         reminder_it_user = list_entry(pos_user,reminder_t,user_notify_list);
         if(loop_counter == notification_idx)
         {
             found_notification_in_list = true;
             break;
         }
         loop_counter+=1;
     }


     bool found_remove_in_global_reminders = false;

     struct list_head* pos_global, *q_global;
     reminder_t* reminder_it = NULL;
     list_for_each_safe(pos_global,q_global,&reminders_list)
     {
         reminder_it = list_entry(pos_global,reminder_t,list);
         if(reminder_it == reminder_it_user)
         {
             found_remove_in_global_reminders = true;
             break;
         }
     }

     if(found_notification_in_list && found_remove_in_global_reminders)
     {
         reminder_it->belongs_to = NULL;
         list_del(pos_user);
         list_del(pos_global);
         reminder_dtor(reminder_it);
     }

 }

 const char* get_all_reminders(void)
 {
     return NULL;
 }

 const char* get_user_reminders(const char* user_name)
 {
     return NULL;
 }

 void show_all_users()
 {
     user_t* user_it = NULL;
     list_for_each_entry(user_it,&users_list,list)
     {
         printf("UserName: %s \n",user_it->user_name);
     }
 }

 void show_all_reminders(void)
 {
     printf(__FUNCTION__);
     printf("\n");

     reminder_t* reminder_it = NULL;
     list_for_each_entry(reminder_it,&reminders_list,list)
     {
         printf("Remainder: %s, Time: %d \n",reminder_it->notification_text,reminder_it->ellapse_time);
     }
 }

 void show_user_reminders(const char* user_name)
 {
     user_t* p_user = find_user_entry(user_name);
     if(!p_user)
         return;
     reminder_t* reminder_it = NULL;
     printf("Username: %s\n", user_name);

     int counter = 1;
     list_for_each_entry(reminder_it,&p_user->notifications_list,user_notify_list)
     {
         printf("Reminder [%d]: %s, time: %d\n",counter, reminder_it->notification_text,reminder_it->ellapse_time);
         counter+=1;
     }
 }

 void cleanup_users(void)
 {
     user_t* user_it = NULL;
     struct list_head* pos, *q;
     list_for_each_safe(pos,q,&users_list)
     {
         user_it = list_entry(pos,user_t,list);
         list_del(pos);
         user_dtor(user_it);
     }
 }

 void cleanup_reminders(void)
 {
     reminder_t* reminder_it = NULL;
     struct list_head* pos, *q;
     list_for_each_safe(pos,q,&reminders_list)
     {
         reminder_it = list_entry(pos,reminder_t,list);
         list_del(pos);
         reminder_dtor(reminder_it);
     }
 }


 static void handle_reminder_expired(reminder_t* reminder_it)
 {
     if(reminder_it->ellapse_time == 0)
     {
         printf("Notification Alert! %s \n",reminder_it->notification_text);
         list_del(&reminder_it->list);

         user_t* user_it = reminder_it->belongs_to;
         if(user_it)
         {
             reminder_t* reminder_it_user = NULL;
             struct list_head* pos_user, *q_user;
             bool found_notification_in_list = false;
             list_for_each_safe(pos_user,q_user,&user_it->notifications_list)
             {
                 reminder_it_user = list_entry(pos_user,reminder_t,user_notify_list);
                 if(reminder_it_user == reminder_it)
                 {
                     found_notification_in_list = true;
                     break;
                 }
             }
             if(found_notification_in_list)
             {
                 list_del(pos_user);
             }
         }
         if(callback_notification_is_about_to_be_removed)
             callback_notification_is_about_to_be_removed(reminder_it);
         reminder_dtor(reminder_it);
     }
 }

 void handle_timer_tick(void)
 {
     reminder_t* reminder_it = NULL;
     struct list_head* pos, *q;
     list_for_each_safe(pos,q,&reminders_list)
     {
         reminder_it = list_entry(pos,reminder_t,list);
         if(reminder_it)
         {
             reminder_it->ellapse_time-=1;
             if(reminder_it->ellapse_time == 0)
             {
                 handle_reminder_expired(reminder_it);
             }
         }
     }
 }

 void for_each_notification_in_queue(notification_queue_enumerator enumerator)
 {
     reminder_t* reminder_it = NULL;
     list_for_each_entry(reminder_it,&reminders_list,list)
     {
         enumerator(reminder_it);
     }
 }
 const char* get_notification_username(struct reminder*item)
 {
     return item->belongs_to->user_name;
 }
 const char* get_notification_text(struct reminder* item)
 {
     return item->notification_text;
 }
 void set_notification_queue_changed_callback(notifications_queue_changed callback)
 {
    callback_queue_changed = callback;
 }
void set_notification_is_about_to_remove_callback(notification_queue_enumerator callback)
{
    callback_notification_is_about_to_be_removed = callback;
}
 size_t get_notifications_total_count(void)
 {
     size_t notifications_count = 0;
     reminder_t* reminder_it = NULL;
     list_for_each_entry(reminder_it,&reminders_list,list)
     {
         ++notifications_count;
     }
     return notifications_count;
 }
