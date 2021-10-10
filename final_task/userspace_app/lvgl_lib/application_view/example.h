#ifndef LVGL_EXAMPLE_H
#define LVGL_EXAMPLE_H
#include <utils/MetaUtils.hpp>
#include <lvgl.h>
#include <memory>
#include <reminders_model.h>
#include <optional>

namespace Lvgl::Example {

class RemindersViewModel
{
public:
    using Ptr = std::unique_ptr<RemindersViewModel>;
    RemindersViewModel();
    ~RemindersViewModel();

    RemindersViewModel&operator=(const RemindersViewModel&) = delete;
    RemindersViewModel(const RemindersViewModel&) = delete;
public:
    void scrollToPrevNotification();
    void scrollToNextNotification();
    bool isScrollAllowed()const;
private:
    using TLvglWidget = Meta::PointerWrapper<lv_obj_t,lv_obj_del_async>;
    using TTimerWrapper = Meta::PointerWrapper<lv_timer_t,lv_timer_del>;
    using TMessageBox = Meta::PointerWrapper<lv_obj_t,lv_msgbox_close>;
    using TGroupWrapper = Meta::PointerWrapper<lv_group_t,lv_group_del>;
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
    void showExpriedNotificationMessageBox(struct reminder* pReminder);
private:
    void createSimpleNotificationsModel();
    void checkIsListEmpty();
    void assignInputDeviceToGroup();

    void deactivateAutoscroll();
private:
    size_t m_autoscrollChildrenIndex;
    std::optional<AutoscrollDirection> m_scrollDirection;

    TLvglWidget m_notificationsListRoot;
    TLvglWidget m_emptyListLabel;
    TTimerWrapper m_scrollNotificationsTimer;
    TTimerWrapper m_notificationsTimer;
    TGroupWrapper m_objectsGroup;

};
void createSimpleWidgetsExample();
}

#endif
