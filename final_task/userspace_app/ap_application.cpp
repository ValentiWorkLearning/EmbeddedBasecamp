#include "ap_application.hpp"

#include "gs_lvgl_service.hpp"
#include "logger/logger_service.hpp"

#include <optional>

Application::Application()
{
    initGraphicsStack();
}

Application::~Application() = default;

void
Application::initGraphicsStack()
{
    m_graphicsService = Graphics::createGraphicsService();
}

void
Application::runApplicationLoop()
{
    while(m_applicationRuning)
        m_graphicsService->executeGlTask();
}
