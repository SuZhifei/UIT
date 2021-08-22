#include "screenfw/ScreenController.h"
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Process.h>
#include <Poco/Thread.h>
#include <v1/uit/screenfw/ControllerProxy.hpp>
#include "ServiceHelper.h"
#define SCREENFW_CONTROLLER_LOG_TAG		"screenfw|controller"

using namespace uit::screenfw;
using namespace v1::uit::screenfw;

static std::shared_ptr<ControllerProxy<>> g_controllerProxy;

bool ScreenController::connect(){
	static bool flag = false;
	if (!flag) {
		initEnv();
		Log::info(SCREENFW_CONTROLLER_LOG_TAG, "COMMONAPI_CONFIG=%s", getenv("COMMONAPI_CONFIG"));
		Log::info(SCREENFW_CONTROLLER_LOG_TAG, "VSOMEIP_CONFIGURATION=%s", getenv("VSOMEIP_CONFIGURATION"));
		flag = true;
	}

	if (g_controllerProxy)
		return true;
	g_controllerProxy = buildProxy<ControllerProxy>("uit.screenfw.controller");
	if (!waitProxy(g_controllerProxy)) {
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "waitProxy failed.\r\n");
		return false;
	}
	else {
		Log::info(SCREENFW_CONTROLLER_LOG_TAG, "waitProxy success.\r\n");
	}

	return g_controllerProxy != nullptr;
}

bool ScreenController::disconnect(){
	g_controllerProxy.reset();
	return true;
}

bool ScreenController::setVisibility(const uint32_t &windowID, const bool &bShow){
	if (g_controllerProxy == nullptr){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "controller[%s] has not connected to service when call [%s].", uit::getProcessName(Poco::Process::id()).data(), __FUNCTION__);
		std::quick_exit(1);
	}

	CommonAPI::CallStatus callStatus;
	bool result = false;

	g_controllerProxy->SetVisibility(windowID, bShow, callStatus, result);
	if (callStatus != CommonAPI::CallStatus::SUCCESS){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "setVisibility(windowID[%u]) failed.", windowID);
	}

	return result;
}

bool ScreenController::setPosition(const uint32_t &windowID, const uint32_t &x, const uint32_t &y, const uint32_t &screenID){
	if (g_controllerProxy == nullptr){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "controller[%s] has not connected to service when call [%s].", uit::getProcessName(Poco::Process::id()).data(), __FUNCTION__);
		std::quick_exit(1);
	}

	CommonAPI::CallStatus callStatus;
	bool result = false;

	g_controllerProxy->SetPosition(windowID, x, y, screenID, callStatus, result);
	if (callStatus != CommonAPI::CallStatus::SUCCESS){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "setPosition(screenID[%u], windowID[%u]) failed.", screenID, windowID);
	}

	return result;
}

bool ScreenController::setClipPosition(const uint32_t &windowID, const uint32_t &x, const uint32_t &y, const uint32_t &screenID){
	if (g_controllerProxy == nullptr){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "controller[%s] has not connected to service when call [%s].", uit::getProcessName(Poco::Process::id()).data(), __FUNCTION__);
		std::quick_exit(1);
	}

	CommonAPI::CallStatus callStatus;
	bool result = false;

	g_controllerProxy->SetClipPosition(windowID, x, y, screenID, callStatus, result);
	if (callStatus != CommonAPI::CallStatus::SUCCESS){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "setClipPosition(screenID[%u], windowID[%u]) failed.", screenID, windowID);
	}

	return result;
}

bool ScreenController::setSize(const uint32_t &windowID, const uint32_t &width, const uint32_t &height){
	if (g_controllerProxy == nullptr){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "controller[%s] has not connected to service when call [%s].", uit::getProcessName(Poco::Process::id()).data(), __FUNCTION__);
		std::quick_exit(1);
	}

	CommonAPI::CallStatus callStatus;
	bool result = false;

	g_controllerProxy->SetSize(windowID, width, height, callStatus, result);
	if (callStatus != CommonAPI::CallStatus::SUCCESS){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "setSize(windowID[%u]) failed.", windowID);
	}

	return result;
}

bool ScreenController::setClipSize(const uint32_t &windowID, const uint32_t &width, const uint32_t &height){
	if (g_controllerProxy == nullptr){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "controller[%s] has not connected to service when call [%s].", uit::getProcessName(Poco::Process::id()).data(), __FUNCTION__);
		std::quick_exit(1);
	}

	CommonAPI::CallStatus callStatus;
	bool result = false;

	g_controllerProxy->SetClipSize(windowID, width, height, callStatus, result);
	if (callStatus != CommonAPI::CallStatus::SUCCESS){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "setClipSize(windowID[%u]) failed.", windowID);
	}

	return result;
}

bool ScreenController::setRect(const uint32_t &windowID, const uint32_t &x, const uint32_t &y, const uint32_t &width, const uint32_t &height, const uint32_t &screenID){
	if (g_controllerProxy == nullptr){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "controller[%s] has not connected to service when call [%s].", uit::getProcessName(Poco::Process::id()).data(), __FUNCTION__);
		std::quick_exit(1);
	}

	CommonAPI::CallStatus callStatus;
	bool result = false;

	g_controllerProxy->SetRect(windowID, x, y, width, height, screenID, callStatus, result);
	if (callStatus != CommonAPI::CallStatus::SUCCESS){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "setRect(screenID[%u], windowID[%u]) failed.", screenID, windowID);
	}

	return result;
}

bool ScreenController::setClipRect(const uint32_t &windowID, const uint32_t &x, const uint32_t &y, const uint32_t &width, const uint32_t &height, const uint32_t &screenID){
	if (g_controllerProxy == nullptr){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "controller[%s] has not connected to service when call [%s].", uit::getProcessName(Poco::Process::id()).data(), __FUNCTION__);
		std::quick_exit(1);
	}

	CommonAPI::CallStatus callStatus;
	bool result = false;

	g_controllerProxy->SetClipRect(windowID, x, y, width, height, screenID, callStatus, result);
	if (callStatus != CommonAPI::CallStatus::SUCCESS){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "setClipRect(screenID[%u], windowID[%u]) failed.", screenID, windowID);
	}

	return result;
}

bool ScreenController::setFullScreen(const uint32_t &windowID, const bool &bFull, const uint32_t &screenID){
	if (g_controllerProxy == nullptr){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "controller[%s] has not connected to service when call [%s].", uit::getProcessName(Poco::Process::id()).data(), __FUNCTION__);
		std::quick_exit(1);
	}

	CommonAPI::CallStatus callStatus;
	bool result = false;

	g_controllerProxy->SetFullScreen(windowID, bFull, screenID, callStatus, result);
	if (callStatus != CommonAPI::CallStatus::SUCCESS){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "setFullScreen(screenID[%u], windowID[%u]) failed.", screenID, windowID);
	}

	return result;
}

bool ScreenController::screenshot(const std::string &filePath, const uint32_t &screenID){
	if (g_controllerProxy == nullptr){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "controller[%s] has not connected to service when call [%s].", uit::getProcessName(Poco::Process::id()).data(), __FUNCTION__);
		std::quick_exit(1);
	}

	CommonAPI::CallStatus callStatus;
	bool result = false;

	g_controllerProxy->Screenshot(filePath, screenID, callStatus, result);
	if (callStatus != CommonAPI::CallStatus::SUCCESS){
		Log::error(SCREENFW_CONTROLLER_LOG_TAG, "screenshot(screenID[%u], filePath[%s]) failed.", screenID, filePath.data());
	}

	return result;
}