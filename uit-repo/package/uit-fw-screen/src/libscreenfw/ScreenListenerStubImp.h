#pragma once
#include "v1/uit/screenfw/ListenerStubDefault.hpp"
//#include "ScreenConfig.h"

using namespace v1::uit::screenfw;

namespace uit {
	namespace screenfw {
		class Model;
		class ScreenListenerStubImp : public ListenerStubDefault{
		public:
			ScreenListenerStubImp(Model* model);

			void notifyVisibility(const uint32_t &_windowID, const bool &_bShow);
			void notifyPosition(const uint32_t &_windowID, const uint32_t &_x, const uint32_t &_y, const uint32_t &_screenID);
			void notifyClipPosition(const uint32_t &_windowID, const uint32_t &_x, const uint32_t &_y, const uint32_t &_screenID);
			void notifySize(const uint32_t &_windowID, const uint32_t &_width, const uint32_t &_height);
			void notifyClipSize(const uint32_t &_windowID, const uint32_t &_width, const uint32_t &_height);
			void notifyRect(const uint32_t &_windowID, const uint32_t &_x, const uint32_t &_y, const uint32_t &_width, const uint32_t &_height, const uint32_t &_screenID);
			void notifyClipRect(const uint32_t &_windowID, const uint32_t &_x, const uint32_t &_y, const uint32_t &_width, const uint32_t &_height, const uint32_t &_screenID);
			void notifyFullScreen(const uint32_t &_windowID, const bool &_bFull, const uint32_t &_screenID);
			void notifyScreenshot(const std::string &_filePath, const uint32_t &_screenID);

		private:
			//remote call
			virtual void SayHello(const std::shared_ptr<CommonAPI::ClientId> _client, int32_t _pid, uint32_t _windowID, SayHelloReply_t _reply) override;

			//get the Receiver for which client to receive.
			std::shared_ptr<CommonAPI::ClientIdList> getReceiver(const uint32_t windowID) const;

			//get the layer which window attached.
			uint32_t getWindowLayer(const uint32_t& windowID);

		private:
			//ScreenConfig 	m_conf;
			Model* 			m_model;
			std::map<size_t, std::pair<std::shared_ptr<CommonAPI::ClientId>, uint32_t>>		m_Receivers;
		};
	}
}