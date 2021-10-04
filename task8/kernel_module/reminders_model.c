#include "reminders_model.h"

#include <linux/types.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/fs.h>
#include "module_common.h"

typedef struct app_user
{
    struct list_head list;
    const char* user_name;
    struct list_head notifications_list;
}app_user_t;

typedef struct reminder
{
    struct list_head list;
    struct list_head user_notify_list;
    const char* notification_text;
    struct app_user* belongs_to;
    int ellapse_time;

}reminder_t;

LIST_HEAD(users_list);
LIST_HEAD(reminders_list);

static app_user_t* find_user_entry(const char* user_name);

static bool is_user_exists(const char* requested_name)
{
    return find_user_entry(requested_name) != NULL;
}

static void user_dtor(app_user_t* destructed_user)
{
    kfree(destructed_user->user_name);
    kfree(destructed_user);
}
static void reminder_dtor(reminder_t* destructed_reminder)
{
    kfree(destructed_reminder->notification_text);
    kfree(destructed_reminder);
}

static app_user_t* find_user_entry(const char* user_name)
{
    struct list_head *ptr;
    app_user_t *entry;
    list_for_each(ptr,&users_list)
    {
        entry = list_entry(ptr,app_user_t,list);
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

        app_user_t* new_user = kmalloc(sizeof(app_user_t),GFP_KERNEL);
        if(!new_user)
            return;

        const size_t required_memory_for_string = strlen(user_name)+1;
        char* string_buffer = kmalloc(required_memory_for_string,GFP_KERNEL);
        if(!string_buffer){
            kfree(new_user);
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
    app_user_t* p_user_entry = find_user_entry(user_name);
    if(!p_user_entry)
        return;
    reminder_t* new_reminder = kmalloc(sizeof (reminder_t),GFP_KERNEL);
    if(!new_reminder)
        return;
    new_reminder->notification_text = kmalloc(strlen(reminder_title)+1,GFP_KERNEL);
    if(!new_reminder->notification_text)
    {
        kfree(new_reminder);
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
    app_user_t* user_it = NULL;
    struct list_head* pos, *q;
    list_for_each_safe(pos,q,&users_list)
    {
        user_it = list_entry(pos,app_user_t,list);
        if(strcmp(user_it->user_name,user_name) == 0)
        {
            list_del(pos);
            user_dtor(user_it);
        }
    }
}

void remove_user_notification(const char* user_name, int notification_number)
{
    app_user_t* p_user = find_user_entry(user_name);
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
    app_user_t* user_it = NULL;
    list_for_each_entry(user_it,&users_list,list)
    {
        printk(KERN_INFO MODULE_TAG"UserName: %s \n",user_it->user_name);
    }
}

void show_all_reminders(void)
{
    printk(KERN_INFO MODULE_TAG "Show all reminders: \n");

    reminder_t* reminder_it = NULL;
    list_for_each_entry(reminder_it,&reminders_list,list)
    {
        printk(KERN_INFO MODULE_TAG"Remainder: %s, Time: %d \n",reminder_it->notification_text,reminder_it->ellapse_time);
    }
}

void show_user_reminders(const char* user_name)
{
    app_user_t* p_user = find_user_entry(user_name);
    if(!p_user)
        return;
    reminder_t* reminder_it = NULL;
    printk(KERN_INFO MODULE_TAG"Username: %s\n", user_name);

    int counter = 1;
    list_for_each_entry(reminder_it,&p_user->notifications_list,user_notify_list)
    {
        printk(KERN_INFO MODULE_TAG"Reminder [%d]: %s, time: %d\n",counter, reminder_it->notification_text,reminder_it->ellapse_time);
        counter+=1;
    }
}

void cleanup_users(void)
{
    app_user_t* user_it = NULL;
    struct list_head* pos, *q;
    list_for_each_safe(pos,q,&users_list)
    {
        user_it = list_entry(pos,app_user_t,list);
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
        printk(KERN_INFO MODULE_TAG"Notification Alert! %s \n",reminder_it->notification_text);
        list_del(&reminder_it->list);

        app_user_t* user_it = reminder_it->belongs_to;
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
