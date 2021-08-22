#include "ControllerApp.h"
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

#define SCREENFW_CONTROLLER_APP_LOG_TAG	"screenfw|controller-app"


POCO_APP_MAIN(uit::screenfw::ScreenControllerApp)

int ScreenControllerApp::main(const std::vector<std::string>& args)
{
#ifndef WIN32	//someip for win32�˳��������������Ҫ��ctrl+c��ֹͣ
	if(m_cmdType == debug || m_cmdType == auto_test)
#endif
		while (1)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	return Application::EXIT_OK;
}

#include <thread>         // std::thread, std::thread::id, std::this_thread::get_id
#include <chrono>         // std::chrono::second

void ScreenControllerApp::initialize(Application & app)
{
	Application::initialize(app);
	if(m_cmdType == ScreenControllerApp::help)
		return;
	ScreenController::connect();

	Log::info(SCREENFW_CONTROLLER_APP_LOG_TAG, "123 test cpu performance for use someip.\r\n");
	
	//HWND handler;
	//handler = FindWindowA(nullptr, "Kanzi");
	//Log::info(SCREENFW_CONTROLLER_APP_LOG_TAG, "het handle{%d}.\r\n", (int)handler);
	//ScreenController::setRect((int)handler, 800, 200, 800, 800, 0);

	//static uint64_t tickCount = uit::getTickCount();
	
	//while(true)
	//{
	//	if(uit::getTickCount() - tickCount >= 2000)
	//	{
	//		static bool vis = true;
	//		HWND handler;
	//		handler = FindWindowA(nullptr, "Kanzi");
	//		Log::info(SCREENFW_CONTROLLER_APP_LOG_TAG, "het handle{%d}.\r\n", (int)handler);
	//		ScreenController::setVisibility((int)handler, vis);
	//		vis = !vis;
	//		tickCount = uit::getTickCount();
	//	}
	//	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	//}

	m_timer.setPeriodicInterval(500);
	m_timer.start(Poco::TimerCallback<ScreenControllerApp>(*this, &ScreenControllerApp::onTimeout_visible));
}

void ScreenControllerApp::onTimeout_visible(Poco::Timer& timer)
{
	static int count = 0;
	static bool vis = true;
	//HWND handler;
	//handler = FindWindowA(nullptr, "Kanzi");
	//Log::info(SCREENFW_CONTROLLER_APP_LOG_TAG, "get handle{%d}.\r\n", (int)handler);
	//ScreenController::setVisibility((int)handler, vis);
	Log::info(SCREENFW_CONTROLLER_APP_LOG_TAG, "onTimeout times(%d), value(%d).\r\n", count, vis);
#if POCO_OS != POCO_OS_WINDOWS_NT
	ScreenController::setVisibility(1001, vis);
#endif
	count++;
	vis = !vis;
}

void ScreenControllerApp::onTimeout_position(Poco::Timer& timer)
{
	static int count = 0;
	static uint32_t x = 0;
	static uint32_t y = 0;
	//HWND handler;
	//handler = FindWindowA(nullptr, "Kanzi");
	//Log::info(SCREENFW_CONTROLLER_APP_LOG_TAG, "get handle{%d}.\r\n", (int)handler);
	//ScreenController::setPosition((int)handler, x, y);
	Log::info(SCREENFW_CONTROLLER_APP_LOG_TAG, "onTimeout times(%d), value(%d, %d).\r\n", count, x, y);
#if POCO_OS != POCO_OS_WINDOWS_NT
	ScreenController::setPosition(1001, x, y);
#endif
	count++;
	x += 20;
	y += 20;
	if(x > 1920) 
		x = 0;
	if(y > 720)
		y = 0;
}

void ScreenControllerApp::onTimeout_size(Poco::Timer& timer)
{
	static int count = 0;
	static uint32_t width = 1920;
	static uint32_t height = 720;
	Log::info(SCREENFW_CONTROLLER_APP_LOG_TAG, "onTimeout times(%d), value(%d, %d).\r\n", count, width, height);
#if POCO_OS != POCO_OS_WINDOWS_NT
	ScreenController::setSize(1001, width, height);
#endif
	count++;
	width -= 20;
	height -= 10;
	if(width < 840) 
		width = 1920;
	if(height < 220)
		height = 720;
}

void ScreenControllerApp::onTimeout_rect(Poco::Timer& timer)
{
	static int count = 0;
	static bool flag = false;
	static uint32_t x = 0;
	static uint32_t y = 0;
	static uint32_t width = 1920;
	static uint32_t height = 720;
	Log::info(SCREENFW_CONTROLLER_APP_LOG_TAG, "onTimeout times(%d), value(%d, %d, %d, %d).\r\n", count, width, height);

#if POCO_OS != POCO_OS_WINDOWS_NT
	ScreenController::setRect(1001, x, y, width, height);
#endif
	count++;
	if(!flag)
	{
		x = 1920/2;
		y = 720/2;
		width = width/2;
		height = height/2;
		flag = true;
	}else
	{
		x = 0;
		y = 0;
		width = 1920;
		height = 720;
		flag = false;
	}	
}

void ScreenControllerApp::onTimeout_clip_rect(Poco::Timer& timer)
{
	static int count = 0;
	static bool flag = false;
	static uint32_t x = 0;
	static uint32_t y = 0;
	static uint32_t width = 1920;
	static uint32_t height = 720;
	Log::info(SCREENFW_CONTROLLER_APP_LOG_TAG, "onTimeout times(%d), value(%d, %d, %d, %d).\r\n", count, width, height);

#if POCO_OS != POCO_OS_WINDOWS_NT
	ScreenController::setClipRect(1001, x, y, width, height);
#endif
	count++;
	if(!flag)
	{
		x = 1920/2;
		y = 720/2;
		width = width/2;
		height = height/2;
		flag = true;
	}else
	{
		x = 0;
		y = 0;
		width = 1920;
		height = 720;
		flag = false;
	}	
}

void ScreenControllerApp::uninitialize()
{
	Application::uninitialize();
}

void ScreenControllerApp::defineOptions(OptionSet & options)
{
	Application::defineOptions(options);
	//shortName�кܶ����ƣ����������fullName�Ŀ�ͷ�������ж��Option shortName���Ƶȡ���������
	options.addOption(Option("help", "", "display help imformation", false).repeatable(false).callback(OptionCallback<ScreenControllerApp>(this, &ScreenControllerApp::handleHelp)));
	options.addOption(Option("debug", "", "debug mode", false).repeatable(false).callback(OptionCallback<ScreenControllerApp>(this, &ScreenControllerApp::handleDebug)));
}

void ScreenControllerApp::handleHelp(const std::string & name, const std::string & value)
{
	showHelp();
}

void ScreenControllerApp::handleDebug(const std::string & name, const std::string & value)
{
	m_cmdType = debug;
	stopOptionsProcessing();
}

void ScreenControllerApp::showHelp()
{
	HelpFormatter hp(options());
	hp.format(std::cout);
	stopOptionsProcessing();
	m_cmdType = help;
}
