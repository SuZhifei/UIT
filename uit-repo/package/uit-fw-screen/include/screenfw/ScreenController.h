#pragma once
#include "uit/core/Def.h"

namespace uit {
	namespace screenfw {

		class UIT_API ScreenController final{
		public:
			ScreenController() = delete;
			~ScreenController() = delete;
			ScreenController(const ScreenController &other) = delete;
			void operator = (const ScreenController &other) = delete;

			static bool connect();
			static bool disconnect();
			static bool setVisibility(const uint32_t &windowID, const bool &bShow);
			static bool setPosition(const uint32_t &windowID, const uint32_t &x, const uint32_t &y, const uint32_t &screenID = 0);
			static bool setClipPosition(const uint32_t &windowID, const uint32_t &x, const uint32_t &y, const uint32_t &screenID = 0);
			static bool setSize(const uint32_t &windowID, const uint32_t &width, const uint32_t &height);
			static bool setClipSize(const uint32_t &windowID, const uint32_t &width, const uint32_t &height);
			static bool setRect(const uint32_t &windowID, const uint32_t &x, const uint32_t &y, const uint32_t &width, const uint32_t &height, const uint32_t &screenID = 0);
			static bool setClipRect(const uint32_t &windowID, const uint32_t &x, const uint32_t &y, const uint32_t &width, const uint32_t &height, const uint32_t &screenID = 0);
			static bool setFullScreen(const uint32_t &windowID, const bool &bFull, const uint32_t &screenID = 0);
			//以下暂未实现:
			static bool screenshot(const std::string &filePath, const uint32_t &screenID =0);
		};
	}
}
