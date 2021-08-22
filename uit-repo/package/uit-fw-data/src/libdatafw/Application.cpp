#include "datafw/Application.h"
#include <uit/core/Log.h>
#include "datafw/Controller.h"
#include "datafw/Application.h"
#include "Share.h"

using namespace uit::datafw;

static bool g_bInstanced = false;
Application::Application()
{
	if (g_bInstanced)
	{
		Log::error(DATAFW_CONTROLLER_LOG_TAG, "can't create two applications in one process.");
		std::exit(1);
	}

	if (!Controller::connect(DATAFW_CONTROLLER_CONNECT_TIMEOUT))
		std::exit(1);
	g_bInstanced = true;
}

Application::~Application()
{
	Controller::disconnect();
}

uint32_t Application::run()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		onIdle();
	}
	return 0;
}

void Application::onIdle()
{
}
