#include "gs_platform_layer.hpp"
#include <logger/logger_service.hpp>

#include <utils/CallbackConnector.hpp>
#include <utils/MetaUtils.hpp>
#include <thread>
#include <chrono>



#ifdef USE_SDL_BACKEND

#include <lvgl/lvgl.h>
#include <lv_drivers/sdl/sdl.h>
#include <lv_drivers/indev/mouse.h>
#include <lv_drivers/indev/keyboard.h>


namespace Graphics {
class PlatformBackend::PlatformConcreteImpl
{
public:

    void platformDependentInit(lv_disp_drv_t* _displayDriver)
    {
        sdl_init();
        _displayDriver->flush_cb = sdl_display_flush;
    }

    void indevPlatformInit()
    {
        static lv_indev_drv_t indevDrv{};

        lv_indev_drv_init( &indevDrv );
        indevDrv.type = LV_INDEV_TYPE_POINTER;
        indevDrv.read_cb = sdl_mouse_read;
        lv_indev_drv_register( &indevDrv );
    }
};

}

#else

#include <memory>
#include <chrono>
#include <thread>
#include <string>
#include <filesystem>
#include <cstddef>
#include <vector>
#include <array>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <cstdio>


namespace  {

constexpr std::string_view kFrameBufferPath = "/dev/gc9_framebuffer_0";

struct FileRaiiGuard {
    FileRaiiGuard(std::string_view filePath):m_fileHandle{}
    {
        if (!std::filesystem::exists(filePath)) {
            throw std::runtime_error(
                        "Bad file path for the FileRaiiGuard");
        }
        m_fileHandle = open(filePath.data(), O_WRONLY);

        if (m_fileHandle == kInvalidFileHandle) {
            throw std::runtime_error(
                        "Failed to open the given file");
        }
    }
    FileRaiiGuard &operator=(const FileRaiiGuard &) = delete;
    FileRaiiGuard(const FileRaiiGuard &) = delete;

    ~FileRaiiGuard()
    {
        if (m_fileHandle != kInvalidFileHandle) {
            close(m_fileHandle);
        }
    }

public:
    void writeToFile(const std::uint8_t *dataSequence,
                     std::size_t dataSequenceSize)
    {
        if(m_fileHandle!=kInvalidFileHandle){
            write(m_fileHandle, dataSequence, dataSequenceSize);
        }
    }

private:
    static constexpr inline std::int32_t kInvalidFileHandle = -1;

private:
    int m_fileHandle;
};

}
namespace Graphics {
class PlatformBackend::PlatformConcreteImpl
        :   public std::enable_shared_from_this<PlatformConcreteImpl>
{
public:

    PlatformConcreteImpl()=default;
    PlatformConcreteImpl&operator=(const PlatformConcreteImpl&) = delete;
    PlatformConcreteImpl(const PlatformConcreteImpl&) = delete;

    void platformDependentInit(lv_disp_drv_t* _displayDriver)
    {
        auto fillDisplayCb = cbc::obtain_connector(
                    [](lv_disp_drv_t* displayDriver, const lv_area_t* fillArea, lv_color_t* colorFill)
        {
            static FileRaiiGuard fileGuard{kFrameBufferPath};
                const size_t dataSize = (displayDriver->ver_res)*(displayDriver->hor_res) * sizeof(lv_color16_t);
                fileGuard.writeToFile(reinterpret_cast<const std::uint8_t*>(colorFill),dataSize);
                lv_disp_flush_ready(displayDriver);
    }
                );
        _displayDriver->flush_cb = fillDisplayCb;
    }

    void indevPlatformInit()
    {

    }
};

}
#endif



namespace Graphics
{


PlatformBackend::PlatformBackend()
    :   m_platformConcreteImpl{std::make_unique<PlatformConcreteImpl>()}
{
    m_isTickThreadRunning.store(false);
};

PlatformBackend::~PlatformBackend()
{
    m_isTickThreadRunning.store(false);
}
void
PlatformBackend::platformDependentInit(lv_disp_drv_t* displayDriver)
{
    m_platformConcreteImpl->platformDependentInit(displayDriver);
}

void
PlatformBackend::initPlatformGfxTimer()
{
    indevPlatformInit();
    initFilesystem();
}

void
PlatformBackend::indevPlatformInit()
{
    m_platformConcreteImpl->indevPlatformInit();

    auto memoryMonitorTask = cbc::obtain_connector(
                [this]( lv_timer_t* _param )
            {
                    memoryMonitor( _param );
            }
            );

/*    lv_timer_create(
                memoryMonitorTask
                ,   10
                ,   nullptr
                ); */
}

void
PlatformBackend::initFilesystem()
{
}

void
PlatformBackend::memoryMonitor(lv_timer_t* _param)
{
    static_cast<void>( _param );

    lv_mem_monitor_t moninor{};
    lv_mem_monitor( &moninor );

    printf("Used: %d, %d" "'%'" "fragmentation : %d, biggest free : %d\n",
           static_cast<std::uint32_t>(moninor.total_size - moninor.free_size)
           , static_cast<std::uint32_t>(moninor.used_pct)
           , static_cast<std::uint32_t>(moninor.frag_pct)
           , static_cast<std::uint32_t>(moninor.free_biggest_size)
           );
}

void
PlatformBackend::executeLvTaskHandler()
{

    if(!m_isTickThreadRunning)
    {
        m_isTickThreadRunning = true;
        setvbuf(stdout, NULL, _IONBF, 0);

        auto m_tickThread = std::thread(
                    [this]
        {

            while(m_isTickThreadRunning){
                lv_tick_inc( LvglNotificationTime );
                std::this_thread::sleep_for(
                            std::chrono::milliseconds( LvglNotificationTime )
                            );
            }
        }
        );
        m_tickThread.detach();
    }
    lv_task_handler();
    std::this_thread::sleep_for(
                std::chrono::milliseconds( LvglNotificationTime )
                );
}





}
