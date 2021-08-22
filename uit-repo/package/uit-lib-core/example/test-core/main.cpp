#include "TestBase.h"
#include "TestRuntime.h"
#include "TestLog.h"
#include "TestEvent.h"
#include <thread>

template <class T>
class TestHandle
{
public:
	TestHandle<T>() { m_pInter = new T(); }
	~TestHandle() { delete m_pInter; }

	T * operator ->() { return (T *)m_pInter; }

private:
	TestBase *m_pInter;
};

int main(int argc, char *argv[])
{
	try {
		TestHandle<TestRuntime> test;
		test->test();
		TestHandle<TestEvent> test1;
		test1->test();
		TestHandle<TestLog> test2;
		test2->test();
	}
	catch (...)
	{
		printf("catch an exception.\r\n");
	}

	while (1)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	return 0;
}