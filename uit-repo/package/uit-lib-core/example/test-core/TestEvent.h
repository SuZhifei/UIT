#pragma once
#include "TestBase.h"
#include "core/Event.h"

class EventSender
{
public:
	struct TimeoutArgs { int a; float b; std::string c; };
	uit::Event<TimeoutArgs> Timeout;

};

class TestEvent : public TestBase
{
public:
	void test();

private:
	void onTimeout(const EventSender::TimeoutArgs &args);
};