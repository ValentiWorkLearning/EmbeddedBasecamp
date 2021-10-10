#pragma once

#include "gs_lvgl_service.hpp"
#include <atomic>
class Application {
    public:
	Application();

	~Application();

    public:
	void runApplicationLoop();

    private:
	void initGraphicsStack();

    private:
	std::atomic_bool m_applicationRuning;
	std::unique_ptr<Graphics::LvglGraphicsService> m_graphicsService;
};
