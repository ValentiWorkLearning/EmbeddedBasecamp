#ifndef LVGL_EXAMPLE_H
#define LVGL_EXAMPLE_H
#include <utils/MetaUtils.hpp>
#include <lvgl.h>
#include <memory>
#include <reminders_model.h>

namespace Lvgl::Example {

class RemindersViewModel
{
public:
    using Ptr = std::unique_ptr<RemindersViewModel>;
    RemindersViewModel();
    ~RemindersViewModel();
public:

private:
    using TLvglWidget = Meta::PointerWrapper<lv_obj_t,lv_obj_del_async>;
    using TTimerWrapper = Meta::PointerWrapper<lv_timer_t,lv_timer_del>;

    enum class AutoscrollDirection
    {
        Forward,
        Backward
    };

private:
    void createNotification(struct reminder*);
    void slotNotificationIsAboutToBeRemoved(struct reminder*);
    void slotAutoScrollTimerExpired(lv_timer_t* timer);
    void slotNotificationTimerTick(lv_timer_t* timer);
private:
    void createSimpleNotificationsModel();
private:
    size_t m_autoscrollChildrenIndex;
    AutoscrollDirection m_scrollDirection;

    TLvglWidget m_notificationsListRoot;
    TTimerWrapper m_scrollNotificationsTimer;
    TTimerWrapper m_notificationsTimer;

};
void createSimpleWidgetsExample();
}

#endif
