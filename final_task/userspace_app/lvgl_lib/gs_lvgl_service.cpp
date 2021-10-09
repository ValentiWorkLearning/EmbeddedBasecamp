#include "gs_lvgl_service.hpp"

#include "platform/gs_platform_layer.hpp"

#include <lvgl.h>
#include <lv_conf.h>

#include "utils/CallbackConnector.hpp"
#include "logger/logger_service.hpp"

#include <array>
#include <memory>

#include "example.h"

namespace Graphics
{

LvglGraphicsService::LvglGraphicsService(
        )   :   m_pGraphicsServiceImpl{ std::make_unique<GSLvglServiceImpl>() }
{
}

class LvglGraphicsService::GSLvglServiceImpl
{

public:

    GSLvglServiceImpl(
    )   :   m_glDisplay{ nullptr }
        ,   m_glDisplayDriver{}
        ,   m_pPlatformBackend{}
    {
        initLvglLogger();
        initDisplayDriver();
        initMainWindow();
    }

public:

    void executeGlTask()
    {
        m_pPlatformBackend.executeLvTaskHandler();
    }
private:

    void initLvglLogger()
    {
        auto lvglLoggerCallback = cbc::obtain_connector(
            []( const char* buffer )
            {

                LOG_DEBUG_ENDL( buffer);

            }
        );
#if LV_USE_LOG == 1
        lv_log_register_print_cb( lvglLoggerCallback );
#endif
    }

    void initDisplayDriver()
    {
        lv_init();

        lv_disp_draw_buf_init(
                        &displayBuffer
                    ,   &dispFrameBufFirst
                    ,   nullptr
                    ,   DispHorRes
                );

        lv_disp_drv_init( &m_glDisplayDriver );
        m_glDisplayDriver.draw_buf = &displayBuffer;

        auto monitorCallback = cbc::obtain_connector(
            []( lv_disp_drv_t * disp_drv, uint32_t time, uint32_t px )
            {
                LOG_DEBUG("Refresh time:");
                LOG_DEBUG_ENDL( time );
                LOG_DEBUG("Refreshed pixels:");
                LOG_DEBUG_ENDL( px );
            }
        );

        m_glDisplayDriver.monitor_cb = monitorCallback;
        m_pPlatformBackend.platformDependentInit( &m_glDisplayDriver );

        m_glDisplay.reset( lv_disp_drv_register( &m_glDisplayDriver ) );

        m_pPlatformBackend.initPlatformGfxTimer();
    }

    void initMainWindow()
    {
        // TODO create the lvlg task for ellaped event processing
        //lv_ex_tileview_1();
        Lvgl::Example::createSimpleWidgetsExample();
    }

private:

    static constexpr size_t DispHorRes = LV_HOR_RES_MAX*10;

    using TColorBuf = std::array<lv_color_t,DispHorRes>;

    static lv_disp_draw_buf_t displayBuffer;
    static TColorBuf dispFrameBufFirst;

private:

    Meta::PointerWrapper<lv_disp_t,lv_disp_remove> m_glDisplay;
    lv_disp_drv_t m_glDisplayDriver;
    Graphics::PlatformBackend m_pPlatformBackend;
};

lv_disp_draw_buf_t LvglGraphicsService::GSLvglServiceImpl::displayBuffer{};

LvglGraphicsService::GSLvglServiceImpl::TColorBuf
LvglGraphicsService::GSLvglServiceImpl::dispFrameBufFirst{};

LvglGraphicsService::~LvglGraphicsService() = default;


void LvglGraphicsService::executeGlTask()
{
    m_pGraphicsServiceImpl->executeGlTask();
}

std::unique_ptr<LvglGraphicsService>
createGraphicsService()
{
    return std::make_unique<LvglGraphicsService>();
}

};
