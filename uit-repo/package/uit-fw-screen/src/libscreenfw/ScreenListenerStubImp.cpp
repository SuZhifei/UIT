#include "ScreenListenerStubImp.h"
#include "Model.h"
#include "Poco/Foundation.h"

#if POCO_OS == POCO_OS_WINDOWS_NT
#include <windows.h>
//HWND handler;
#elif POCO_OS == POCO_OS_LINUX
//do the Linux thing
#elif POCO_OS == POCO_OS_QNX
//do the QNX thing
#endif

using namespace uit::screenfw;

ScreenListenerStubImp::ScreenListenerStubImp(Model* model)
	:m_model(model){	
	//m_conf.loadConf();
	//handler = FindWindowA(nullptr, "note");
	//SetWindowPos(handler, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//SetWindowPos(handler, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void ScreenListenerStubImp::notifyVisibility(const uint32_t &_windowID, const bool &_bShow){
#if POCO_OS == POCO_OS_WINDOWS_NT
	fireNotifyVisibilitySelective(_bShow, getReceiver(_windowID));
	// do the Windows thing
#elif POCO_OS == POCO_OS_LINUX
	//do the Linux thing
#elif POCO_OS == POCO_OS_QNX
	//windows之间互斥逻辑:
	if(_bShow){
		static bool flag = false;
		for(auto receiver : m_Receivers){
			if(receiver.second.second == _windowID){
				fireNotifyVisibilitySelective(_bShow, getReceiver(_windowID));
				flag = true;
			}
		}

		if(flag){
			for(auto receiver : m_Receivers){
				if(getWindowLayer(receiver.second.second) == getWindowLayer(_windowID) 
					&&receiver.second.second != _windowID){
					fireNotifyVisibilitySelective(!_bShow, getReceiver(receiver.second.second));
				}
			}
			flag = false;
		}
	}else{
		fireNotifyVisibilitySelective(_bShow, getReceiver(_windowID));
	}
#endif
}

void ScreenListenerStubImp::notifyPosition(const uint32_t &_windowID, const uint32_t &_x, const uint32_t &_y, const uint32_t &_screenID){
	fireNotifyPositionSelective(_x, _y, _screenID, getReceiver(_windowID));
}

void ScreenListenerStubImp::notifyClipPosition(const uint32_t &_windowID, const uint32_t &_x, const uint32_t &_y, const uint32_t &_screenID){
	fireNotifyClipPositionSelective(_x, _y, _screenID, getReceiver(_windowID));
}

void ScreenListenerStubImp::notifySize(const uint32_t &_windowID, const uint32_t &_width, const uint32_t &_height){
	fireNotifySizeSelective(_width, _height, getReceiver(_windowID));
}

void ScreenListenerStubImp::notifyClipSize(const uint32_t &_windowID, const uint32_t &_width, const uint32_t &_height){
	fireNotifyClipSizeSelective(_width, _height, getReceiver(_windowID));
}

void ScreenListenerStubImp::notifyRect(const uint32_t &_windowID, const uint32_t &_x, const uint32_t &_y, const uint32_t &_width, const uint32_t &_height, const uint32_t &_screenID){
	fireNotifyRectSelective(_x, _y, _width, _height, _screenID, getReceiver(_windowID));
}

void ScreenListenerStubImp::notifyClipRect(const uint32_t &_windowID, const uint32_t &_x, const uint32_t &_y, const uint32_t &_width, const uint32_t &_height, const uint32_t &_screenID){
	fireNotifyClipRectSelective(_x, _y, _width, _height, _screenID, getReceiver(_windowID));
}

void ScreenListenerStubImp::notifyFullScreen(const uint32_t &_windowID, const bool &_bFull, const uint32_t &_screenID){
	fireNotifyFullScreenSelective(_bFull, _screenID, getReceiver(_windowID));
}

void ScreenListenerStubImp::notifyScreenshot(const std::string &_filePath, const uint32_t &_screenID){
}

void ScreenListenerStubImp::SayHello(const std::shared_ptr<CommonAPI::ClientId> _client, int32_t _pid, uint32_t _windowID, SayHelloReply_t _reply){
	uint32_t windowID = _windowID;
	m_Receivers[_client->hashCode()] = std::make_pair(_client, windowID);
}

std::shared_ptr<CommonAPI::ClientIdList> ScreenListenerStubImp::getReceiver(const uint32_t windowID) const {
	auto receivers = std::make_shared<CommonAPI::ClientIdList>();
	for (auto listener : m_Receivers) {
		if (listener.second.second == windowID) {
			receivers->insert(listener.second.first);
		}
	}
	return receivers;
}

uint32_t ScreenListenerStubImp::getWindowLayer(const uint32_t& windowID){
	uint32_t num = pow(10, floor(log10(windowID)));
	return (windowID/num) * num;
}
