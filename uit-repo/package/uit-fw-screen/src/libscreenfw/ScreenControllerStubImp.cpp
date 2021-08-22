#include "ScreenControllerStubImp.h"
#include "Poco/Foundation.h"
#include "Model.h"

using namespace uit::screenfw;

ScreenControllerStubImp::ScreenControllerStubImp(Model* model)
	:m_model(model){
}

void ScreenControllerStubImp::SetVisibility(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _windowID, bool _bShow, SetVisibilityReply_t _reply){
	(void)_client;
	bool result = m_model->setVisibility(_windowID, _bShow);
	_reply(result);
}

void ScreenControllerStubImp::SetPosition(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _windowID, uint32_t _x, uint32_t _y, uint32_t _screenID, SetPositionReply_t _reply){
	(void)_client;
	bool result = m_model->setPosition(_windowID, _x, _y, _screenID);
	_reply(result);
}

void ScreenControllerStubImp::SetClipPosition(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _windowID, uint32_t _x, uint32_t _y, uint32_t _screenID, SetPositionReply_t _reply){
	(void)_client;
	bool result = m_model->setClipPosition(_windowID, _x, _y, _screenID);
	_reply(result);
}

void ScreenControllerStubImp::SetSize(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _windowID, uint32_t _width, uint32_t _height, SetSizeReply_t _reply){
	(void)_client;
	bool result = m_model->setSize(_windowID, _width, _height);
	_reply(result);
}

void ScreenControllerStubImp::SetClipSize(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _windowID, uint32_t _width, uint32_t _height, SetSizeReply_t _reply){
	(void)_client;
	bool result = m_model->setClipSize(_windowID, _width, _height);
	_reply(result);
}

void ScreenControllerStubImp::SetRect(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _windowID, uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height, uint32_t _screenID, SetRectReply_t _reply){
	(void)_client;
	bool result = m_model->setRect(_windowID, _x, _y, _width, _height, _screenID);
	_reply(result);
}

void ScreenControllerStubImp::SetClipRect(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _windowID, uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height, uint32_t _screenID, SetRectReply_t _reply){
	(void)_client;
	bool result = m_model->setClipRect(_windowID, _x, _y, _width, _height, _screenID);
	_reply(result);
}

void ScreenControllerStubImp::SetFullScreen(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _windowID, bool _bFull, uint32_t _screenID, SetFullScreenReply_t _reply){
	(void)_client;
	bool result = m_model->setFullScreen(_windowID, _bFull, _screenID);
	_reply(result);
}

void ScreenControllerStubImp::Screenshot(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _filePath, uint32_t _screenID, ScreenshotReply_t _reply){
	(void)_client;
	bool result = m_model->screenshot(_filePath, _screenID);
	_reply(result);
}