#pragma once
#include "uit/core/Def.h"
#include <mutex>

namespace uit {
	namespace screenfw {
		class ScreenControllerStubImp;
		class ScreenListenerStubImp;

		class UIT_API Model final{
		public:
			Model() {};
			bool startup();
			bool stop();

			//controller���ýӿ�
			bool setVisibility(const uint32_t &windowID, const bool &bShow);
			bool setPosition(const uint32_t &windowID, const uint32_t &x, const uint32_t &y, const uint32_t &screenID);
			bool setClipPosition(const uint32_t &windowID, const uint32_t &x, const uint32_t &y, const uint32_t &screenID);
			bool setSize(const uint32_t &windowID, const uint32_t &width, const uint32_t &height);
			bool setClipSize(const uint32_t &windowID, const uint32_t &width, const uint32_t &height);
			bool setRect(const uint32_t &windowID, const uint32_t &x, const uint32_t &y, const uint32_t &width, const uint32_t &height, const uint32_t &screenID);
			bool setClipRect(const uint32_t &windowID, const uint32_t &x, const uint32_t &y, const uint32_t &width, const uint32_t &height, const uint32_t &screenID);
			bool setFullScreen(const uint32_t &windowID, const bool &bFull, const uint32_t &screenID);
			bool screenshot(const std::string &filePath, const uint32_t &screenID);

			//Listener���ýӿ�

		private:
			bool initScreenController(uint32_t timeoutMs);
			bool initScreenListener(uint32_t timeoutMs);

		private:
			std::shared_ptr<ScreenControllerStubImp>	m_screenControllerStubImp;
			std::shared_ptr<ScreenListenerStubImp>		m_screenListenerStubImp;
			std::recursive_mutex                       m_mutex;
		};
	}
}