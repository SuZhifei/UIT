#include "TestLog.h"
#include "core/Log.h"
#include "core/Runtime.h"
#include <thread>

using namespace uit;

#define LOG_TEST_TAG	"core|test"

void func()
{
	bool b = false;
	float f = 2.333333f;
	double d = 19.2324343544656577;
	char c = 'a';
	std::string s = "a德赛西威bc";
	Log::debug(LOG_TEST_TAG, "b=%d", b);
	Log::info(LOG_TEST_TAG, "f=%.2f", f);
	Log::warn(LOG_TEST_TAG, "d=%f", d);
	Log::error(LOG_TEST_TAG, "s=%s", s.data());
	Log::fatal(LOG_TEST_TAG, "c=%c", c);
}

void TestLog::test()
{
	printf("----------------test log-----------------\n");
	UIT_LOG_USE_SYSTEM_CONFIG
	//不要把所有的进程的Log::file的path属性都设置为同一路径，否则会导致多进程读写同一log文件而挂死
	bool bb = Log::setFileChannelProperty("path", uit::Runtime::getUitTempDirectory() + "test-log.txt");
//	bool b = Log::configFromFile("e:/uit.log.ini");

	Log::setLevel(Log::console | Log::file, Log::Level::debug);

	for (int i = 0; i != 1; ++i)
	{
		std::thread t(func);
		t.detach();
	}
	
}
