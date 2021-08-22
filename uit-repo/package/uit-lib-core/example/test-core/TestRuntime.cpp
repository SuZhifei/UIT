#include "TestRuntime.h"
#include "core/Runtime.h"

using namespace uit;

void TestRuntime::test()
{
	printf("----------------test runtime-----------------\n");
	printf("home=%s\n", Runtime::getUitHomeDirectory().data());
	printf("lib=%s\n", Runtime::getUitLibDirectory().data());
	printf("etc=%s\n", Runtime::getUitEtcDirectory().data());
	printf("bin=%s\n", Runtime::getUitBinDirectory().data());
	printf("temp=%s\n", Runtime::getUitTempDirectory().data());
	printf("share=%s\n", Runtime::getUitShareDirectory().data());
	printf("resource=%s\n", Runtime::getUitResourceDirectory().data());
	printf("data=%s\n", Runtime::getUitDataDirectory().data());
	printf("----------------test runtime-----------------\n\n");
}
