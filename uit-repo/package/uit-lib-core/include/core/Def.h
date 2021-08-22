#pragma once
#include <typeinfo>
#include <string>
#include <stdint.h>
#include <memory>
#include <algorithm>

//要求c++11
//#if __cplusplus < 201103L
//	#error "c++11 supported requested."
//#endif

//平台识别,如果识别不了平台，编译器则会输出报错，并终止编译
#define UIT_OS							UIT_OS_UNKNOWN
#define UIT_OS_UNKNOWN					0x00000001
#define UIT_OS_WINDOWS					0x00000002
#define UIT_OS_WINCE					0x00000004
#define UIT_OS_LINUX_X11				0x00000008
#define UIT_OS_LINUX_ARM				0x00000010
#define UIT_OS_MAC						0x00000020
#define UIT_OS_IOS						0x00000040
#define UIT_OS_ANDROID					0x00000080
#define UIT_OS_QNX						0x00000100

//类UNIX下可使用g++ -dM -E - </dev/null命令查看编译器默认宏
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS)	//windows
#undef	UIT_OS
#define UIT_OS		UIT_OS_WINDOWS
#pragma warning(disable: 4251)
#if _MSC_VER < 1900
#error "you must build under vs2015 or later version"
#endif
#elif defined(WINCE)															//wince
#undef	UIT_OS
#define UIT_OS		UIT_OS_WINCE
#elif defined(__ANDROID__) || (__Android__)										//android
#undef	UIT_OS
#define UIT_OS		UIT_OS_ANDROID
#elif defined(__QNX__) || defined(__QNXNTO)										//qnx(gcc 5.4)
#undef	UIT_OS
#define UIT_OS		UIT_OS_QNX
#elif defined(__ARM_ARCH)														//linux-arm，该值为几代表第几代，比如是7代表第7代v7
#undef	UIT_OS
#define UIT_OS		UIT_OS_LINUX_ARM
#elif defined(linux) || defined(__linux__) || defined(__LINUX__)				//linux-x11
#undef	UIT_OS
#define UIT_OS		UIT_OS_LINUX_X11
#elif defined(__IOS__) || defined(__Ios__) || defined(ios)						//ios（ios只能主动设置？）
#undef	UIT_OS
#define UIT_OS		UIT_OS_IOS
#elif defined(__APPLE__) || defined(Mac) || defined(Apple)						//mac
#undef	UIT_OS
#define UIT_OS		UIT_OS_MAC
#else
#error "!!!!!!!!!!Can not recognize the target platform type: have you configured an sopported platform ? check file 'core/Def.h' to confirm the Macro 'UIT_OS'.!!!!!!!!!!"
#endif


//定义API导出宏
#if UIT_OS == UIT_OS_WINDOWS
#define UIT_API					__declspec(dllexport)
#else
#define UIT_API
#endif

namespace uit
{
	//获取开机以来的时钟滴答数（毫秒数）
	UIT_API uint64_t getTickCount();

	//根据pid获取进程名
	UIT_API std::string getProcessName(int pid);

}