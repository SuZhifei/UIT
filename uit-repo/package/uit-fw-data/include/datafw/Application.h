#pragma once
#include "Type.h"

namespace uit { namespace datafw{

class Application_Internal;
class UIT_API Application
{
public:
	//构建一个Application
	//并自动连接model
	Application();
	~Application();
	Application(const Application &other) = delete;
	void operator = (const Application &other) = delete;

	//运行
	uint32_t run();

	//重写以执行空闲时动作
	virtual void onIdle();

private:
	Application_Internal		*m_internal;
};

}}

