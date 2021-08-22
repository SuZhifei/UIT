#include "screenfw/ScreenListener.h"
#include "v1/uit/screenfw/ListenerProxy.hpp"
#include "ServiceHelper.h"
#include <Poco/Process.h>

#define SCREENFW_LISTENER_LOG_TAG		"screenfw|listener"

using namespace uit::screenfw;
using namespace v1::uit::screenfw;

ScreenListener::ScreenListener(){
}

ScreenListener::~ScreenListener(){
	#if POCO_OS == POCO_OS_WINDOWS_NT
		// do the Windows thing
	#elif POCO_OS == POCO_OS_QNX
		free(m_displayPtr);
		//kanzi内部已经做了free，这里就不再重复动作:
		//screen_destroy_window(m_screenInfo.second.second);
		//screen_destroy_context(m_screenInfo.second.first); 
	#endif
}

#if POCO_OS == POCO_OS_WINDOWS_NT
	struct ALLMONITORINFO {
		HMONITOR hMonitor;
		RECT			rect;
		bool				isPrimary;
	};

	int CALLBACK MonitorEnumProc(__in  HMONITOR hMonitor, __in  HDC hdcMonitor, __in  LPRECT lprcMonitor, __in  LPARAM dwData) {
		std::vector<ALLMONITORINFO>& infoArray = *reinterpret_cast<std::vector<ALLMONITORINFO>* >(dwData);

		ALLMONITORINFO monitorInfo;
		monitorInfo.hMonitor = hMonitor;
		monitorInfo.rect = *lprcMonitor;

		HMONITOR priMonitor = MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY);
		if (priMonitor == hMonitor)
			monitorInfo.isPrimary = true;
		else
			monitorInfo.isPrimary = false;

		infoArray.push_back(monitorInfo);
		return true;
	}

	bool getDisplayInfo(std::vector<ALLMONITORINFO> &displayInfo) {
		displayInfo.clear();
		displayInfo.reserve(GetSystemMetrics(SM_CMONITORS));
		EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(&displayInfo));
		return !displayInfo.size() == 0;
	}

	bool ScreenListener::connect(const uint32_t &windowID){
		if (!windowID) {
			return false;
		}
		m_windowID = windowID;
		//m_windowInfo.push_back(windowID);

		//connect to service and regist monitoring func
		if (m_listenerProxy)
			return true;
		int pid = Poco::Process::id();
		m_listenerProxy = buildProxy<ListenerProxy>("uit.screenfw.listener");
		if (!waitProxy(m_listenerProxy)){
			Log::error(SCREENFW_LISTENER_LOG_TAG, "waitProxy failed.\r\n");
			return false;
		}
		else{
			Log::info(SCREENFW_LISTENER_LOG_TAG, "waitProxy success.\r\n");
			m_listenerProxy->getNotifyVisibilitySelectiveEvent().subscribe([&](const bool &_bShow) {onNotifyVisibilitySelectiveEvent(_bShow); });
			m_listenerProxy->getNotifyPositionSelectiveEvent().subscribe([&](const uint32_t &_x, const uint32_t &_y, const uint32_t &_screenID) {onNotifyPositionSelectiveEvent(_x, _y, _screenID); });
			m_listenerProxy->getNotifyClipPositionSelectiveEvent().subscribe([&](const uint32_t &_x, const uint32_t &_y, const uint32_t &_screenID) {onNotifyClipPositionSelectiveEvent(_x, _y, _screenID); });
			m_listenerProxy->getNotifySizeSelectiveEvent().subscribe([&](const uint32_t &_width, const uint32_t &_height) {onNotifySizeSelectiveEvent(_width, _height); });
			m_listenerProxy->getNotifyClipSizeSelectiveEvent().subscribe([&](const uint32_t &_width, const uint32_t &_height) {onNotifyClipSizeSelectiveEvent(_width, _height); });
			m_listenerProxy->getNotifyRectSelectiveEvent().subscribe([&](const uint32_t &_x, const uint32_t &_y, const uint32_t &_width, const uint32_t &_height, const uint32_t &_screenID) {onNotifyRectSelectiveEvent(_x, _y, _width, _height, _screenID); });
			m_listenerProxy->getNotifyClipRectSelectiveEvent().subscribe([&](const uint32_t &_x, const uint32_t &_y, const uint32_t &_width, const uint32_t &_height, const uint32_t &_screenID) {onNotifyClipRectSelectiveEvent(_x, _y, _width, _height, _screenID); });
			m_listenerProxy->getNotifyFullScreenSelectiveEvent().subscribe([&](const bool &_bFull, const uint32_t &_screenID) {onNotifyFullScreenSelectiveEvent(_bFull, _screenID); });
			m_listenerProxy->getNotifyscreenshotSelectiveEvent().subscribe([&](const std::string &_filePath, const uint32_t &_screenID) {onNotifyscreenshotSelectiveEvent(_filePath, _screenID); });
		}
		try{
			CommonAPI::CallStatus callStatus;
			m_listenerProxy->SayHello(pid, windowID, callStatus);
		}
		catch (...){
			Log::error(SCREENFW_LISTENER_LOG_TAG, "connect failed.\r\n");
		}

		return m_listenerProxy != nullptr;
	}
#elif POCO_OS == POCO_OS_QNX
	bool ScreenListener::connect(const uint32_t &windowID, screen_context_t screen_ctx, screen_window_t screen_win){
		//get screen handler from GUI such as kanzi.
		if(!screen_ctx || !screen_win){
			Log::error(SCREENFW_LISTENER_LOG_TAG, "connect failed, Please check the parameters.\r\n");
			return false;
		}
		m_screenInfo.first = windowID;
		m_screenInfo.second.first = screen_ctx;
		m_screenInfo.second.second = screen_win;

		int zorder = m_screenInfo.first;
		screen_set_window_property_iv(m_screenInfo.second.second, SCREEN_PROPERTY_ZORDER, &zorder);

		//retrieving an array of display pointers.
		int display_count{0};
		screen_get_context_property_iv(screen_ctx, SCREEN_PROPERTY_DISPLAY_COUNT, &display_count);
		Log::info(SCREENFW_LISTENER_LOG_TAG, "get display count %d\r\n", display_count);

		m_displayPtr = static_cast<screen_display_t*>(calloc(display_count, sizeof(screen_display_t)));
		screen_get_context_property_pv(screen_ctx, SCREEN_PROPERTY_DISPLAYS, (void **)m_displayPtr);

		//connect to service and regist monitoring func
		if (m_listenerProxy)
			return true;
		int pid = Poco::Process::id();
		m_listenerProxy = buildProxy<ListenerProxy>("uit.screenfw.listener");
		if (!waitProxy(m_listenerProxy)) {
			Log::error(SCREENFW_LISTENER_LOG_TAG, "waitProxy failed.\r\n");
			return false;
		}
		else {
			Log::info(SCREENFW_LISTENER_LOG_TAG, "waitProxy success.\r\n");
			m_listenerProxy->getNotifyVisibilitySelectiveEvent().subscribe([&](const bool &_bShow) {onNotifyVisibilitySelectiveEvent(_bShow); });
			m_listenerProxy->getNotifyPositionSelectiveEvent().subscribe([&](const uint32_t &_x, const uint32_t &_y, const uint32_t &_screenID) {onNotifyPositionSelectiveEvent(_x, _y, _screenID); });
			m_listenerProxy->getNotifyClipPositionSelectiveEvent().subscribe([&](const uint32_t &_x, const uint32_t &_y, const uint32_t &_screenID) {onNotifyClipPositionSelectiveEvent(_x, _y, _screenID); });
			m_listenerProxy->getNotifySizeSelectiveEvent().subscribe([&](const uint32_t &_width, const uint32_t &_height) {onNotifySizeSelectiveEvent(_width, _height); });
			m_listenerProxy->getNotifyClipSizeSelectiveEvent().subscribe([&](const uint32_t &_width, const uint32_t &_height) {onNotifyClipSizeSelectiveEvent(_width, _height); });
			m_listenerProxy->getNotifyRectSelectiveEvent().subscribe([&](const uint32_t &_x, const uint32_t &_y, const uint32_t &_width, const uint32_t &_height, const uint32_t &_screenID) {onNotifyRectSelectiveEvent(_x, _y, _width, _height, _screenID); });
			m_listenerProxy->getNotifyClipRectSelectiveEvent().subscribe([&](const uint32_t &_x, const uint32_t &_y, const uint32_t &_width, const uint32_t &_height, const uint32_t &_screenID) {onNotifyClipRectSelectiveEvent(_x, _y, _width, _height, _screenID); });
			m_listenerProxy->getNotifyFullScreenSelectiveEvent().subscribe([&](const bool &_bFull, const uint32_t &_screenID) {onNotifyFullScreenSelectiveEvent(_bFull, _screenID); });
			m_listenerProxy->getNotifyscreenshotSelectiveEvent().subscribe([&](const std::string &_filePath, const uint32_t &_screenID) {onNotifyscreenshotSelectiveEvent(_filePath, _screenID); });
		}
		try {
			CommonAPI::CallStatus callStatus;
			m_listenerProxy->SayHello(pid, windowID, callStatus);
		}
		catch (...) {
			Log::error(SCREENFW_LISTENER_LOG_TAG, "connect failed.\r\n");
		}

		return m_listenerProxy != nullptr;
	}

	bool ScreenListener::attachDisplay(screen_window_t screen_win, screen_display_t screen_dis){
		if(screen_set_window_property_pv(screen_win, SCREEN_PROPERTY_DISPLAY, (void **)&screen_dis)){
			Log::error(SCREENFW_LISTENER_LOG_TAG, "attach window[%d] to screen[%d] failed, please check parameter _screenID.\r\n");
			return false;
		}
		return true;
	}

#endif

void ScreenListener::onNotifyVisibilitySelectiveEvent(const bool &_bShow){
#if POCO_OS == POCO_OS_WINDOWS_NT
	int vis = _bShow ? SW_SHOW : SW_HIDE;
	ShowWindow((HWND)m_windowID, vis);
#elif POCO_OS == POCO_OS_QNX
	int vis = _bShow;
	screen_set_window_property_iv(m_screenInfo.second.second, SCREEN_PROPERTY_VISIBLE, &vis);
	//screen_flush_context(m_screenInfo.second.first, SCREEN_WAIT_IDLE);
#endif
	VisibleChangedEvent.dispatch({ _bShow });
}

void ScreenListener::onNotifyPositionSelectiveEvent(const uint32_t &_x, const uint32_t &_y, const uint32_t &_screenID){
#if POCO_OS == POCO_OS_WINDOWS_NT
	std::vector<ALLMONITORINFO> displayInfo;
	getDisplayInfo(displayInfo);
	if (_screenID >= displayInfo.size()) {
		return;
	}
	RECT rect;
	GetWindowRect((HWND)m_windowID, &rect);
	MoveWindow((HWND)m_windowID, displayInfo[_screenID].rect.left + _x, _y, rect.right - rect.left, rect.bottom - rect.top, false);
#elif POCO_OS == POCO_OS_QNX
	int pos[2] = { _x, _y };
	screen_set_window_property_iv(m_screenInfo.second.second, SCREEN_PROPERTY_POSITION, pos);
	attachDisplay(m_screenInfo.second.second, m_displayPtr[_screenID]);
	//screen_flush_context(m_screenInfo.second.first, SCREEN_WAIT_IDLE);
#endif
	PositionChangedEvent.dispatch({_x, _y, _screenID});
}

void ScreenListener::onNotifyClipPositionSelectiveEvent(const uint32_t &_x, const uint32_t &_y, const uint32_t &_screenID){
#if POCO_OS == POCO_OS_WINDOWS_NT
	std::vector<ALLMONITORINFO> displayInfo;
	getDisplayInfo(displayInfo);
	if (_screenID >= displayInfo.size()) {
		return;
	}
	RECT rect;
	GetWindowRect((HWND)m_windowID, &rect);
	MoveWindow((HWND)m_windowID, displayInfo[_screenID].rect.left + _x, _y, rect.right - rect.left, rect.bottom - rect.top, false);
#elif POCO_OS == POCO_OS_QNX
	int pos[2] = { _x, _y };
	screen_set_window_property_iv(m_screenInfo.second.second, SCREEN_PROPERTY_CLIP_POSITION, pos);
	attachDisplay(m_screenInfo.second.second, m_displayPtr[_screenID]);
	//screen_flush_context(m_screenInfo.second.first, SCREEN_WAIT_IDLE);
#endif
	ClipPositionChangedEvent.dispatch({ _x, _y, _screenID });
}

void ScreenListener::onNotifySizeSelectiveEvent(const uint32_t &_width, const uint32_t &_height){
#if POCO_OS == POCO_OS_WINDOWS_NT
	RECT rect;
	GetWindowRect((HWND)m_windowID, &rect);
	MoveWindow((HWND)m_windowID, rect.left, rect.top, _width, _height, false);
#elif POCO_OS == POCO_OS_QNX
	int size[2] = { _width, _height };
	screen_set_window_property_iv(m_screenInfo.second.second, SCREEN_PROPERTY_SIZE, size);
	//screen_flush_context(m_screenInfo.second.first, SCREEN_WAIT_IDLE);
#endif
	SizeChangedEvent.dispatch({ _width, _height });
}

void ScreenListener::onNotifyClipSizeSelectiveEvent(const uint32_t &_width, const uint32_t &_height){
#if POCO_OS == POCO_OS_WINDOWS_NT
	RECT rect;
	GetWindowRect((HWND)m_windowID, &rect);
	MoveWindow((HWND)m_windowID, rect.left, rect.top, _width, _height, false);
#elif POCO_OS == POCO_OS_QNX
	int size[2] = { _width, _height };
	screen_set_window_property_iv(m_screenInfo.second.second, SCREEN_PROPERTY_CLIP_SIZE, size);
	//screen_flush_context(m_screenInfo.second.first, SCREEN_WAIT_IDLE);
#endif
	ClipSizeChangedEvent.dispatch({ _width, _height });
}

void ScreenListener::onNotifyRectSelectiveEvent(const uint32_t &_x, const uint32_t &_y, const uint32_t &_width, const uint32_t &_height, const uint32_t &_screenID){
#if POCO_OS == POCO_OS_WINDOWS_NT
	std::vector<ALLMONITORINFO> displayInfo;
	getDisplayInfo(displayInfo);
	if (_screenID >= displayInfo.size()) {
		return;
	}
	MoveWindow((HWND)m_windowID, displayInfo[_screenID].rect.left + _x, _y, _width, _height, false);
#elif POCO_OS == POCO_OS_QNX
	int pos[2] = { _x, _y };
	int size[2] = { _width, _height };
	screen_set_window_property_iv(m_screenInfo.second.second, SCREEN_PROPERTY_POSITION, pos);
	screen_set_window_property_iv(m_screenInfo.second.second, SCREEN_PROPERTY_SIZE, size);
	attachDisplay(m_screenInfo.second.second, m_displayPtr[_screenID]);
	//screen_flush_context(m_screenInfo.second.first, SCREEN_WAIT_IDLE);
#endif
	RectChangedEvent.dispatch({ _x, _y, _width, _height,  _screenID });
}

void ScreenListener::onNotifyClipRectSelectiveEvent(const uint32_t &_x, const uint32_t &_y, const uint32_t &_width, const uint32_t &_height, const uint32_t &_screenID){
#if POCO_OS == POCO_OS_WINDOWS_NT
	std::vector<ALLMONITORINFO> displayInfo;
	getDisplayInfo(displayInfo);
	if (_screenID >= displayInfo.size()) {
		return;
	}
	MoveWindow((HWND)m_windowID, displayInfo[_screenID].rect.left + _x, _y, _width, _height, false);
#elif POCO_OS == POCO_OS_QNX
	int pos[2] = { _x, _y };
	int size[2] = { _width, _height };
	screen_set_window_property_iv(m_screenInfo.second.second, SCREEN_PROPERTY_CLIP_POSITION, pos);
	screen_set_window_property_iv(m_screenInfo.second.second, SCREEN_PROPERTY_CLIP_SIZE, size);
	attachDisplay(m_screenInfo.second.second, m_displayPtr[_screenID]);
	//screen_flush_context(m_screenInfo.second.first, SCREEN_WAIT_IDLE);
#endif
	ClipRectChangedEvent.dispatch({ _x, _y, _width, _height,  _screenID });
}

void ScreenListener::onNotifyFullScreenSelectiveEvent(const bool &_bFull, const uint32_t &_screenID){
#if POCO_OS == POCO_OS_WINDOWS_NT
	if (_bFull) {
		std::vector<ALLMONITORINFO> displayInfo;
		getDisplayInfo(displayInfo);
		if (_screenID >= displayInfo.size()) {
			return;
		}

		LONG curWinStyle = GetWindowLong((HWND)m_windowID, GWL_STYLE);
		curWinStyle = curWinStyle & ~WS_OVERLAPPEDWINDOW;
		SetWindowLong((HWND)m_windowID, GWL_STYLE, curWinStyle | WS_POPUP);

		MoveWindow((HWND)m_windowID, displayInfo[_screenID].rect.left, displayInfo[_screenID].rect.top, displayInfo[_screenID].rect.right - displayInfo[_screenID].rect.left, displayInfo[_screenID].rect.bottom - displayInfo[_screenID].rect.top, false);
	}
	else {
		SetWindowLong((HWND)m_windowID, GWL_STYLE, GetWindowLong((HWND)m_windowID, GWL_STYLE) | WS_OVERLAPPEDWINDOW);
		//GWL_EXSTYLE  extended style扩展格式。WS_EX_WINDOWEDGE  使得窗口显示，并不覆盖下面的任务栏。
		SetWindowLong((HWND)m_windowID, GWL_EXSTYLE, GetWindowLong((HWND)m_windowID, GWL_EXSTYLE) | WS_EX_WINDOWEDGE);
	}
#elif POCO_OS == POCO_OS_QNX
	if (_bFull){
		int pos[2] = { 0, 0 };
		int dims[2] = { 0, 0 };
		screen_get_display_property_iv(m_displayPtr[_screenID], SCREEN_PROPERTY_SIZE, dims);
		screen_set_window_property_iv(m_screenInfo.second.second, SCREEN_PROPERTY_POSITION, pos);
		screen_set_window_property_iv(m_screenInfo.second.second, SCREEN_PROPERTY_SIZE, dims);
		attachDisplay(m_screenInfo.second.second, m_displayPtr[_screenID]);
		//screen_flush_context(m_screenInfo.second.first, SCREEN_WAIT_IDLE);
	}
#endif
	FullScreenChangedEvent.dispatch({ _bFull , _screenID });
}

void ScreenListener::onNotifyscreenshotSelectiveEvent(const std::string &_filePath, const uint32_t &_screenID){
}