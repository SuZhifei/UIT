#include "ScreenService.h"
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/IniFileConfiguration.h>
#include <uit/core/Log.h>
#include <uit/core/Runtime.h>

using namespace uit::screenfw;
using namespace Poco::Util;

#define SCREENFW_SCREENSERVICE_LOG_TAG	"screenfw|screenservice"

int ScreenService::main(const std::vector<std::string>& args)
{
	if (!m_bHelp)
		waitForTerminationRequest();
	return ServerApplication::EXIT_OK;
}

void ScreenService::initialize(Application & app)
{
	//UIT_LOG_USE_SYSTEM_CONFIG
	ServerApplication::initialize(app);
	if (m_bHelp)
		return;
	auto k = uit::getTickCount();
	bool result = m_model.startup();
	if (result)
	{
		Log::info(SCREENFW_SCREENSERVICE_LOG_TAG, "model server startup success, cost [%d] ms.", (int)(uit::getTickCount() - k));
	}
	else
	{
		Log::info(SCREENFW_SCREENSERVICE_LOG_TAG, "model server startup fail");
		terminate();
		return;
	}
}

void ScreenService::uninitialize()
{
	ServerApplication::uninitialize();
	if (!m_bHelp)
	{
		m_model.stop();
		Log::info(SCREENFW_SCREENSERVICE_LOG_TAG, "model server shutdowm.");
	}
}

void ScreenService::defineOptions(OptionSet & options)
{
	Application::defineOptions(options);
	options.addOption(Option("help", "h", "display help imformation", false).repeatable(false).callback(OptionCallback<ScreenService>(this, &ScreenService::handleHelp)));
	options.addOption(Option("dir", "d", "specify data event dir", false, "dir").repeatable(false).callback(OptionCallback<ScreenService>(this, &ScreenService::handleDataEventDir)));
	//options.addOption(Option("socket", "s", "on|off socket server", false, "on|off").repeatable(false).callback(OptionCallback<ScreenService>(this, &ScreenService::handleSocketDebugHelperOnOff)));
}

void ScreenService::handleHelp(const std::string & name, const std::string & value)
{
	m_bHelp = true;
	HelpFormatter hp(options());
	hp.format(std::cout);
	stopOptionsProcessing();
}

void ScreenService::handleDataEventDir(const std::string & name, const std::string & value)
{
	m_optDataEventDir = value;
}
