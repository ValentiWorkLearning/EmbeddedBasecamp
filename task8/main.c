#include "reminders_model.h"
#include <stdio.h>

const char *user1 = "Valenti";
const char* user2 = "Void";
const char* user3 = "Alex";

void test1()
{
    add_new_user(user1);
    add_new_user(user2);
    add_new_user(user3);

    show_all_users();
    remove_user(user2);
    show_all_users();

    add_user_reminder(user1,"Buy some food!",1000);
    add_user_reminder(user1,"Buy some tomaaatos!",100);
    add_user_reminder(user1,"But extre beer",500);
    add_user_reminder(user3,"Buy cucumbers",40);
    add_user_reminder(user3,"Buy sausages",150);
    show_all_reminders();


    show_user_reminders(user1);
    show_user_reminders(user2);
    show_user_reminders(user3);

    printf("Removed notification\n");

    remove_user_notification(user1,3);
    show_user_reminders(user1);

    printf("Show all\n");
    show_all_reminders();
    cleanup_users();
    cleanup_reminders();
}

void test2()
{
    add_new_user(user1);
    add_new_user(user2);
    add_new_user(user3);

    add_new_user(user1);
    add_new_user(user2);
    add_new_user(user3);

    add_new_user(user1);
    add_new_user(user2);
    add_new_user(user3);
    cleanup_users();
}

void test3()
{
    add_new_user(user1);
    add_user_reminder(user1,"Buy some food!",1000);
    add_user_reminder(user1,"Buy some tomaaatos!",100);

    show_all_reminders();

    remove_user_notification(user1,2);
    remove_user_notification(user1,1);

    show_all_reminders();
    cleanup_users();
    cleanup_reminders();
}

void test4()
{
    add_new_user(user1);
    add_user_reminder(user1,"Buy some food!",2);
    add_user_reminder(user1,"Buy some tomaaatos!",1);
    show_all_reminders();

    handle_timer_tick();

    show_all_reminders();

    handle_timer_tick();
    show_all_reminders();

    const int expire_time= 4;
    add_user_reminder(user1,"Buy something new, eventyally!",expire_time);
    for(int i = 0; i< expire_time; ++i)
    {
        show_user_reminders(user1);
        handle_timer_tick();
    }

}
int main()
{
    test1();
    test2();
    test3();
    test4();
    return 0;
}
