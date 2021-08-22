#include "ListenerApp.h"
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/StringTokenizer.h>
#include <uit/core/Log.h>
#include <uit/core/Runtime.h>
#include <cassert>
#include <thread>

using namespace uit::screenfw;
using namespace Poco;
using namespace Poco::Util;

#define DATAFW_CONTROLLER_APP_LOG_TAG	"screenfw|listener-app"

int ListenerApp::main(const std::vector<std::string>& args)
{
#ifndef WIN32	//someip for win32
	if (m_cmdType != ListenerApp::help && m_cmdType != ListenerApp::post_event)
#endif
		while (1)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

	return Application::EXIT_OK;
}

void ListenerApp::initialize(Application & app)
{
	Application::initialize(app);
	if(m_cmdType == ListenerApp::help)
		return;

	uint64_t k = uit::getTickCount();
	bool bSuccess = false;
	while (!bSuccess)
	{
		bSuccess = true;
		//bSuccess = m_listener.connect(3001, 0);
	}
	printf("view connected, cost [%d] %s.\n", (int)(uit::getTickCount() - k), bSuccess ? "success" : "fail");
}

void ListenerApp::uninitialize()
{
	Application::uninitialize();
}

void ListenerApp::defineOptions(OptionSet & options)
{
	Application::defineOptions(options);
	//shortName�кܶ����ƣ����������fullName�Ŀ�ͷ�������ж��Option shortName���Ƶȡ���������
	options.addOption(Option("help", "", "display help imformation", false).repeatable(false).callback(OptionCallback<ListenerApp>(this, &ListenerApp::handleHelp)));
	options.addOption(Option("debug", "", "display help imformation", false).repeatable(false).callback(OptionCallback<ListenerApp>(this, &ListenerApp::handleDebug)));
	options.addOption(Option("post", "", "post event", false, "path#argname#argvalue#argname#argvalue...").repeatable(false).callback(OptionCallback<ListenerApp>(this, &ListenerApp::handlePostEvent)));
	
}

void ListenerApp::handleHelp(const std::string & name, const std::string & value)
{
	showHelp();
}

void ListenerApp::handleDebug(const std::string & name, const std::string & value)
{
	m_cmdType = debug;
	stopOptionsProcessing();
}

void ListenerApp::handlePostEvent(const std::string & name, const std::string & value)
{
	m_cmdType = post_event;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 3 || m_values.size() % 2 == 0)
		showHelp();
	stopOptionsProcessing();
}

void ListenerApp::showHelp()
{
	HelpFormatter hp(options());
	hp.format(std::cout);
	stopOptionsProcessing();
	m_cmdType = help;
}