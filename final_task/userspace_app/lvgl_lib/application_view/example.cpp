#include "example.h"
#include <lvgl.h>
#include <reminders_model.h>
#include <utils/CallbackConnector.hpp>
#include <algorithm>


static const char *kMessageboxButtons[] ={"OK",""};


namespace Lvgl::Example {

static void scrollEventCallback(lv_event_t * e)
{
    lv_obj_t * cont = lv_event_get_target(e);

    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);
    lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2;

    lv_coord_t r = lv_obj_get_height(cont) * 7 / 10;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_cnt(cont);
    for(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = lv_obj_get_child(cont, i);
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);

        lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;

        lv_coord_t diff_y = child_y_center - cont_y_center;
        diff_y = LV_ABS(diff_y);

        /*Get the x of diff_y on a circle.*/
        lv_coord_t x;
        /*If diff_y is out of the circle use the last point of the circle (the radius)*/
        if(diff_y >= r) {
            x = r;
        } else {
            /*Use Pythagoras theorem to get x from radius and y*/
            uint32_t x_sqr = r * r - diff_y * diff_y;
            lv_sqrt_res_t res;
            lv_sqrt(x_sqr, &res, 0x8000);   /*Use lvgl's built in sqrt root function*/
            x = r - res.i;
        }

        /*Translate the item by the calculated X coordinate*/
        lv_obj_set_style_translate_x(child, x, 0);

        /*Use some opacity with larger translations*/
        lv_opa_t opa = lv_map(x, 0, r, LV_OPA_TRANSP, LV_OPA_COVER);
        lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);
    }
}

static void messageBoxDefaultCallback(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_current_target(e);
    lv_msgbox_close(obj);
}

static void createMessageboxForReminder(struct reminder* pReminder)
{

    const char* reminderUsername = get_notification_username(pReminder);
    const char* reminderText = get_notification_text(pReminder);

    lv_obj_t * reminderMessagebox = lv_msgbox_create(nullptr, reminderUsername, reminderText, kMessageboxButtons, true);
    lv_obj_add_event_cb(reminderMessagebox, messageBoxDefaultCallback, LV_EVENT_CLICKED, nullptr);
    lv_obj_center(reminderMessagebox);
}
static void onNotificationButtonClicked(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * pButton = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        struct reminder* pReminder =  reinterpret_cast<struct reminder*>(lv_obj_get_user_data(pButton));
        createMessageboxForReminder(pReminder);
    }
}


RemindersViewModel::RemindersViewModel():m_autoscrollChildrenIndex{},m_scrollDirection{AutoscrollDirection::Forward}
{
    createSimpleNotificationsModel();

    m_notificationsListRoot.reset(lv_obj_create(lv_scr_act()));
    auto cont = m_notificationsListRoot.get();

    lv_obj_set_size(cont, 240, 240);
    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_add_event_cb(cont, scrollEventCallback, LV_EVENT_SCROLL, nullptr);
    lv_obj_set_style_radius(cont, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_clip_corner(cont, true, 0);
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(cont, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);

    auto enumNotifications = cbc::obtain_connector<0>(this, &RemindersViewModel::createNotification);
    for_each_notification_in_queue(enumNotifications);


    lv_event_send(cont, LV_EVENT_SCROLL, nullptr);

    /*Be sure the fist button is in the middle*/
    lv_obj_scroll_to_view(lv_obj_get_child(cont, 0), LV_ANIM_OFF);

    auto autoscrollTimerCallback = cbc::obtain_connector<0>(this,&RemindersViewModel::slotAutoScrollTimerExpired);
    auto notificationTimerCallback = cbc::obtain_connector<1>(this,&RemindersViewModel::slotNotificationTimerTick);

    auto notificationIsAboutToBeRemovedSlot = cbc::obtain_connector(this,&RemindersViewModel::slotNotificationIsAboutToBeRemoved);
    set_notification_is_about_to_remove_callback(notificationIsAboutToBeRemovedSlot);

    constexpr auto kTimerTimeout = 1500;
    m_scrollNotificationsTimer.reset(lv_timer_create(autoscrollTimerCallback, kTimerTimeout,nullptr));

    constexpr auto kNotificationsTimeout = 1000;
    m_notificationsTimer.reset(lv_timer_create(notificationTimerCallback, kNotificationsTimeout,nullptr));
}

void RemindersViewModel::createSimpleNotificationsModel()
{
    constexpr auto valenti = "Valenti";
    constexpr auto voidUser = "Void";
    constexpr auto alex = "Alex";


    add_new_user(valenti);
    add_new_user(voidUser);
    add_new_user(alex);
    add_new_user(valenti);
    add_new_user(voidUser);
    add_new_user(alex);

    add_user_reminder(valenti,"Buy some food!",15);
    add_user_reminder(valenti,"Buy some tomaaatos!",20);
    add_user_reminder(valenti,"But extre beer",25);
    add_user_reminder(voidUser,"Physical activity",30);
    add_user_reminder(alex,"Buy sausages",35);
    add_user_reminder(alex,"Check mobile for new messages",40);

}

void RemindersViewModel::createNotification(struct reminder* pReminder)
{
    auto listContainer = m_notificationsListRoot.get();
    lv_obj_t * notificationButton = lv_btn_create(listContainer);
    lv_obj_set_size(notificationButton, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_add_event_cb(notificationButton, onNotificationButtonClicked, LV_EVENT_ALL, nullptr);
    lv_obj_set_user_data(notificationButton,pReminder);
    lv_obj_t * label = lv_label_create(notificationButton);
    lv_label_set_text(label, get_notification_username(pReminder));
}

void RemindersViewModel::slotNotificationIsAboutToBeRemoved(struct reminder* pReminder)
{
    auto listContainer = m_notificationsListRoot.get();
    size_t numChildren{lv_obj_get_child_cnt(listContainer)};
    for(uint32_t childIt{}; childIt < numChildren; ++childIt)
    {
        lv_obj_t* pChildButton = lv_obj_get_child(listContainer,childIt);
        if(pChildButton)
        {
            struct reminder* pReminderButton =  reinterpret_cast<struct reminder*>(lv_obj_get_user_data(pChildButton));
            if(pReminder == pReminderButton){
                lv_obj_del(pChildButton);
                showExpriedNotificationMessageBox(pReminder);
                checkIsListEmpty();
                return;
            }
        }
    }
}

void RemindersViewModel::slotAutoScrollTimerExpired(lv_timer_t *timer)
{
    lv_obj_t* view = m_notificationsListRoot.get();

    const bool isEmpty = is_notifications_list_empty();
    if(isEmpty)
        return;

    size_t notificationsCount = get_notifications_total_count();
    const bool shouldScroll = notificationsCount > 1 && m_autoscrollChildrenIndex < notificationsCount;
    if(!shouldScroll)
        return;
    lv_obj_scroll_to_view(lv_obj_get_child(view, m_autoscrollChildrenIndex), LV_ANIM_ON);
    if(m_scrollDirection == AutoscrollDirection::Forward)
    {
        ++m_autoscrollChildrenIndex;
        if(m_autoscrollChildrenIndex == notificationsCount){
            m_scrollDirection = AutoscrollDirection::Backward;
            --m_autoscrollChildrenIndex;
        }
    }
    else{
        if(m_autoscrollChildrenIndex == 0)
            m_scrollDirection = AutoscrollDirection::Forward;
        else
            --m_autoscrollChildrenIndex;
    }
}

void RemindersViewModel::showExpriedNotificationMessageBox(struct reminder* pReminder)
{

    const char* reminderText = get_notification_text(pReminder);

    lv_obj_t* messagebox = lv_msgbox_create(nullptr, "!REMINDER!", reminderText, kMessageboxButtons, true);
    lv_obj_add_event_cb(messagebox, messageBoxDefaultCallback, LV_EVENT_CLICKED, nullptr);
    lv_obj_center(messagebox);
}
void RemindersViewModel::slotNotificationTimerTick(lv_timer_t* timer)
{
    handle_timer_tick();
}

void RemindersViewModel::checkIsListEmpty()
{
    const bool isEmpty = is_notifications_list_empty();
    if(isEmpty)
    {
        m_emptyListLabel.reset(lv_label_create(lv_scr_act()));
        lv_label_set_text(m_emptyListLabel.get(), "Notification list is empty");
        lv_obj_align(m_emptyListLabel.get(),LV_ALIGN_CENTER,0,0);
    }
    else{
        m_emptyListLabel.reset();
    }
}
RemindersViewModel::~RemindersViewModel()
{
    cleanup_users();
    cleanup_reminders();

}
}
