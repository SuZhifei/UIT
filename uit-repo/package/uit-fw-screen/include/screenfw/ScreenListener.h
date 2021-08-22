#pragma once
#include <vector>
#include "uit/core/Def.h"
#include "uit/core/Event.h"
#include "Poco/Foundation.h"

#if POCO_OS == POCO_OS_QNX
	#include "screen/screen.h"
#endif

namespace v1 {
	namespace uit {
		namespace screenfw {
			template <typename ... _AttributeExtensions>
			class ListenerProxy;
		}
	}
}

namespace uit {
	namespace screenfw {
		class UIT_API ScreenListener final {
		public:
			ScreenListener();
			~ScreenListener();
			ScreenListener(const ScreenListener &other) = delete;
			void operator = (const ScreenListener &other) = delete;

		#if POCO_OS == POCO_OS_WINDOWS_NT
			bool connect(const uint32_t &windowID);
		#elif POCO_OS == POCO_OS_QNX
			bool connect(const uint32_t &windowID, screen_context_t screen_ctx, screen_window_t screen_win);
		#endif

		public:
			//窗口可见通知
			struct VisibleChangedArgs { bool visible; };
			uit::Event<VisibleChangedArgs> VisibleChangedEvent;

			//窗口位置变化通知
			struct PositionChangedArgs { uint32_t x; uint32_t y;  uint32_t screenID; };
			uit::Event<PositionChangedArgs> PositionChangedEvent;

			struct ClipPositionChangedArgs { uint32_t x; uint32_t y;  uint32_t screenID; };
			uit::Event<ClipPositionChangedArgs> ClipPositionChangedEvent;

			//窗口大小变化通知
			struct SizeChangedArgs { uint32_t width; uint32_t height; };
			uit::Event<SizeChangedArgs> SizeChangedEvent;

			struct ClipSizeChangedArgs { uint32_t width; uint32_t height; };
			uit::Event<ClipSizeChangedArgs> ClipSizeChangedEvent;

			//窗口矩形变化通知
			struct RectChangedArgs { uint32_t x; uint32_t y;  uint32_t width; uint32_t height; uint32_t screenID; };
			uit::Event<RectChangedArgs> RectChangedEvent;

			struct ClipRectChangedArgs { uint32_t x; uint32_t y;  uint32_t width; uint32_t height; uint32_t screenID; };
			uit::Event<ClipRectChangedArgs> ClipRectChangedEvent;

			//窗口全屏通知
			struct FullScreenChangedArgs { bool _bFull; uint32_t _screenID; };
			uit::Event<FullScreenChangedArgs> FullScreenChangedEvent;

		private:
			//callback
			void onNotifyVisibilitySelectiveEvent(const bool &_bShow);
			void onNotifyPositionSelectiveEvent(const uint32_t &_x, const uint32_t &_y, const uint32_t &_screenID);
			void onNotifyClipPositionSelectiveEvent(const uint32_t &_x, const uint32_t &_y, const uint32_t &_screenID);
			void onNotifySizeSelectiveEvent(const uint32_t &_width, const uint32_t &_height);
			void onNotifyClipSizeSelectiveEvent(const uint32_t &_width, const uint32_t &_height);
			void onNotifyRectSelectiveEvent(const uint32_t &_x, const uint32_t &_y, const uint32_t &_width, const uint32_t &_height, const uint32_t &_screenID);
			void onNotifyClipRectSelectiveEvent(const uint32_t &_x, const uint32_t &_y, const uint32_t &_width, const uint32_t &_height, const uint32_t &_screenID);
			void onNotifyFullScreenSelectiveEvent(const bool &_bFull, const uint32_t &_screenID);
			void onNotifyscreenshotSelectiveEvent(const std::string &_filePath, const uint32_t &_screenID);

			std::shared_ptr<v1::uit::screenfw::ListenerProxy<>>	m_listenerProxy;

		#if POCO_OS == POCO_OS_WINDOWS_NT
				//std::vector<uint32_t>	m_windowInfo;
				uint32_t	m_windowID;
		#elif POCO_OS == POCO_OS_QNX
				bool attachDisplay(screen_window_t screen_win, screen_display_t screen_dis);	//attach window to display.
				screen_display_t* 	m_displayPtr;
				std::pair<uint32_t, std::pair<screen_context_t, screen_window_t>> m_screenInfo;
		#endif
		};
	}
}