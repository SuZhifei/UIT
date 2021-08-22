#pragma once
#include <v1/uit/screenfw/ControllerStubDefault.hpp>

using namespace v1::uit::screenfw;

namespace uit {
	namespace screenfw {
		
		class Model;
		class ScreenControllerStubImp : public ControllerStubDefault
		{
		public:
			ScreenControllerStubImp(Model* model) ;

		private:
			//remote call
			virtual void SetVisibility(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _windowID, bool _bShow, SetVisibilityReply_t _reply) override;
			virtual void SetPosition(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _windowID, uint32_t _x, uint32_t _y, uint32_t _screenID, SetPositionReply_t _reply) override;
			virtual void SetClipPosition(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _windowID, uint32_t _x, uint32_t _y, uint32_t _screenID, SetPositionReply_t _reply) override;
			virtual void SetSize(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _windowID, uint32_t _width, uint32_t _height, SetSizeReply_t _reply) override;
			virtual void SetClipSize(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _windowID, uint32_t _width, uint32_t _height, SetSizeReply_t _reply) override;
			virtual void SetRect(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _windowID, uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height, uint32_t _screenID, SetRectReply_t _reply) override;
			virtual void SetClipRect(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _windowID, uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height, uint32_t _screenID, SetRectReply_t _reply) override;
			virtual void SetFullScreen(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _windowID, bool _bFull, uint32_t _screenID, SetFullScreenReply_t _reply) override;
			virtual void Screenshot(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _filePath, uint32_t _screenID, ScreenshotReply_t _reply) override;

		private:
			Model* m_model;
		};
	}
}