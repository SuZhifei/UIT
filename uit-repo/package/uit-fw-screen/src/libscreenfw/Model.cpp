#include "Model.h"
#include "ServiceHelper.h"
#include "ScreenControllerStubImp.h"
#include "ScreenListenerStubImp.h"

#define SCREENFW_MODEL_LOG_TAG		"screenfw|model"

using namespace uit::screenfw;

bool Model::startup(){
	Log::info(SCREENFW_MODEL_LOG_TAG, "startup.\r\n");
	bool ret = initScreenController(5 * 1000);
	ret &= initScreenListener(5 * 1000);
	return ret;
}

bool Model::stop(){
	return true;
}

bool Model::setVisibility(const uint32_t &windowID, const bool &bShow){
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	m_screenListenerStubImp->notifyVisibility(windowID, bShow);
	return true;
}

bool Model::setPosition(const uint32_t &windowID, const uint32_t &x, const uint32_t &y, const uint32_t &screenID){
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	m_screenListenerStubImp->notifyPosition(windowID, x, y, screenID);
	return true;
}

bool Model::setClipPosition(const uint32_t &windowID, const uint32_t &x, const uint32_t &y, const uint32_t &screenID){
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	m_screenListenerStubImp->notifyClipPosition(windowID, x, y, screenID);
	return true;
}

bool Model::setSize(const uint32_t &windowID, const uint32_t &width, const uint32_t &height){
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	m_screenListenerStubImp->notifySize(windowID, width, height);
	return true;
}

bool Model::setClipSize(const uint32_t &windowID, const uint32_t &width, const uint32_t &height){
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	m_screenListenerStubImp->notifyClipSize(windowID, width, height);
	return true;
}

bool Model::setRect(const uint32_t &windowID, const uint32_t &x, const uint32_t &y, const uint32_t &width, const uint32_t &height, const uint32_t &screenID){
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	m_screenListenerStubImp->notifyRect(windowID, x, y, width, height, screenID);
	return true;
}

bool Model::setClipRect(const uint32_t &windowID, const uint32_t &x, const uint32_t &y, const uint32_t &width, const uint32_t &height, const uint32_t &screenID){
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	m_screenListenerStubImp->notifyClipRect(windowID, x, y, width, height, screenID);
	return true;
}

bool Model::setFullScreen(const uint32_t &windowID, const bool &bFull, const uint32_t &screenID){
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	m_screenListenerStubImp->notifyFullScreen(windowID, bFull, screenID);
	return true;
}

bool Model::screenshot(const std::string &filePath, const uint32_t &screenID){
	std::lock_guard<std::recursive_mutex> lock(m_mutex);
	m_screenListenerStubImp->notifyScreenshot(filePath, screenID);
	return true;
}

bool Model::initScreenController(uint32_t timeoutMs){
	//��һЩ��ʼ���Ķ�������register֮ǰ�����Ա�֤����ע�����Щ�Ѿ���ʼ�����ͷ�����ʱ������©����֮ǰ�и�©���ǣ�screenListenerImp��m_model�����õ��¿��ܵ���ʱΪ�յ������
	m_screenControllerStubImp = std::make_shared<ScreenControllerStubImp>(this);
	bool ret = registerService<ScreenControllerStubImp>(m_screenControllerStubImp, "uit.screenfw.controller");
	if (!ret){
		Log::error(SCREENFW_MODEL_LOG_TAG, "init ScreenController timeout for [%d] ms, maybe stub dll(so) not found. check env COMMONAPI_CONFIG.\r\n", timeoutMs);
	}
	return ret;
}

bool Model::initScreenListener(uint32_t timeoutMs){
	m_screenListenerStubImp = std::make_shared<ScreenListenerStubImp>(this); 
	bool ret = registerService<ScreenListenerStubImp>(m_screenListenerStubImp, "uit.screenfw.listener");
	if (!ret){
		Log::error(SCREENFW_MODEL_LOG_TAG, "init ScreenListener timeout for [%d] ms, maybe stub dll(so) not found. check env COMMONAPI_CONFIG.\r\n", timeoutMs);
	}
	return ret;
}

