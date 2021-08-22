#include "DataService.h"
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/IniFileConfiguration.h>
#include <uit/core/Log.h>
#include <uit/core/Runtime.h>

using namespace uit::datafw;
using namespace Poco::Util;

#define DATAFW_DATASERVICE_LOG_TAG	"datafw|dataservice"
#define DEFAULT_DATA_EVENT_DIR		uit::Runtime::getUitEtcDirectory() + "data-event"
#define CONFIGURE_FILE_PATH			uit::Runtime::getUitEtcDirectory() + "uit.svc.data.conf"

int DataService::main(const std::vector<std::string>& args)
{
	if (!m_bHelp)
		waitForTerminationRequest();
	return ServerApplication::EXIT_OK;
}

void DataService::initialize(Application & app)
{
	UIT_LOG_USE_SYSTEM_CONFIG
	ServerApplication::initialize(app);
	if (m_bHelp)
		return;

	bool bEnableSocketDebugHelper = false;
	std::string sDataEventDir;
	Poco::AutoPtr<IniFileConfiguration> ini;
	std::string iniPath = CONFIGURE_FILE_PATH;
	try {
		//从配置文件解析配置
		ini = new IniFileConfiguration(iniPath);
		if (m_optEnableSocketDebugHelper.empty())
		{
			try {
				bEnableSocketDebugHelper = ini->getBool("model.EnableSocketDebugHelper", false);
			}
			catch (...) {
				bEnableSocketDebugHelper = false;
			}
		}
		else
		{
			bEnableSocketDebugHelper = m_optEnableSocketDebugHelper == "on" ? true : false;
		}

		if (m_optDataEventDir.empty())
		{
			try {
				sDataEventDir = ini->getString("model.DataEventDir", DEFAULT_DATA_EVENT_DIR);
			}
			catch (...) {
				sDataEventDir.clear();
			}
		}
		else
		{
			sDataEventDir = m_optDataEventDir;
		}

	}
	catch (...) {
		Log::info(DATAFW_DATASERVICE_LOG_TAG, "open [%s] fail, use default config.", iniPath.data());
	}

	if (sDataEventDir.empty())	sDataEventDir = DEFAULT_DATA_EVENT_DIR;
	Log::info(DATAFW_DATASERVICE_LOG_TAG, "data event dir [%s].", sDataEventDir.data());
	Log::info(DATAFW_DATASERVICE_LOG_TAG, "socket debug helper [%s].", bEnableSocketDebugHelper ? "on" : "off");
	auto k = uit::getTickCount();
	if (m_model.configDataEventDir(sDataEventDir))
	{
		Log::info(DATAFW_DATASERVICE_LOG_TAG, "config data event success, cost [%d] ms.", (int)(uit::getTickCount() - k));
	}
	else
	{
		Log::error(DATAFW_DATASERVICE_LOG_TAG, "config data event fail.");
		terminate();
		return;
	}

	k = uit::getTickCount();
	m_model.enableSocketDebugHelper(bEnableSocketDebugHelper);
	bool b = m_model.startup();
	if (b)
	{
		Log::info(DATAFW_DATASERVICE_LOG_TAG, "model server startup success, cost [%d] ms.", (int)(uit::getTickCount() - k));
	}
	else
	{
		Log::info(DATAFW_DATASERVICE_LOG_TAG, "model server startup fail");
		terminate();
		return;
	}
}

void DataService::uninitialize()
{
	ServerApplication::uninitialize();
	if (!m_bHelp)
	{
		m_model.stop();
		Log::info(DATAFW_DATASERVICE_LOG_TAG, "model server shutdowm.");
	}
}

void DataService::defineOptions(OptionSet & options)
{
	Application::defineOptions(options);
	options.addOption(Option("help", "h", "display help imformation", false).repeatable(false).callback(OptionCallback<DataService>(this, &DataService::handleHelp)));
	options.addOption(Option("dir", "d", "specify data event dir", false, "dir").repeatable(false).callback(OptionCallback<DataService>(this, &DataService::handleDataEventDir)));
	options.addOption(Option("socket", "s", "on|off socket server", false, "on|off").repeatable(false).callback(OptionCallback<DataService>(this, &DataService::handleSocketDebugHelperOnOff)));

}

void DataService::handleHelp(const std::string & name, const std::string & value)
{
	m_bHelp = true;
	HelpFormatter hp(options());
	hp.format(std::cout);
	stopOptionsProcessing();
}

void DataService::handleDataEventDir(const std::string & name, const std::string & value)
{
	m_optDataEventDir = value;
}

void DataService::handleSocketDebugHelperOnOff(const std::string & name, const std::string & value)
{
	m_optEnableSocketDebugHelper = value;
}
