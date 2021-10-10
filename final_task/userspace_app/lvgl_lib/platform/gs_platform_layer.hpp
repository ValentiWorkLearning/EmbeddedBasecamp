#pragma once

#include <memory>

#include <lvgl.h>

#include <thread>
#include <chrono>
#include <atomic>

namespace Graphics
{

class PlatformBackend
{

public:

    PlatformBackend();
    ~PlatformBackend();

    void platformDependentInit( lv_disp_drv_t* displayDriver );

    void initPlatformGfxTimer();

    void executeLvTaskHandler();

private:

    static constexpr std::uint32_t LvglNotificationTime = 15;

private:

    void indevPlatformInit();
    void initFilesystem();
    void memoryMonitor(lv_timer_t* _param);

private:
    class PlatformConcreteImpl;
    std::unique_ptr<PlatformConcreteImpl> m_platformConcreteImpl;
    std::atomic_bool m_tickStop = false;
    std::thread m_tickThread;
};

} // namespace Graphics
