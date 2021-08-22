/**********************************************
*	编写者：	Pan.瞳
*
*	编写日期：	2019-3-15
*
*	文件名称：	Log.h
*
*	描述：		日志系统
*
*	说明：		日志系统可提供多个日志输出通道，包括Console, File，Device等，并且可以对它们的输出等级进行控制
*				每个使用Log的进程将在第一次使用时初始化系统，加载配置文件（如果需要的话）以控制通道属性
*				Log系统是线程安全的，可在多线程中使用。
*				UIT_LOG_USE_SYSTEM_CONFIG宏可以让你的进程方便的使用系统统一配置，也可以使用接口或configFromFile单独
*				对你的进程进行设置。更多细节请查看doc下的文档
*
************************************************/
#pragma once
#include "Def.h"
#include <string>

namespace uit {

class UIT_API Log
{
public:
	enum Channel
	{
		console = 0x1 << 0,		//控制台型通道
		file	= 0x1 << 1,		//文件型通道
		device	= 0x1 << 2,		//设备
	};

	enum class Level
	{
		none = 0,				//不启用
		debug,					//调试（默认）
		information,			//信息
		warning,				//警告
		error,					//错误
		fatal,					//致命
	};

public:
	//以配置文件的方式初始化log系统
	static bool configFromFile(const std::string &path);

	//获取配置文件路径，若未配置返回空
	static std::string getConfigFile();

	//设置日志等级
	//channels：可以是单个或者输出通道的组合，Log::console或者 Log::console | Log::file表示控制台和文件通道的组合
	//level：高于或者等于level的日志才能被打印，如果设置为None，将关闭
	//perpetual：存储到配置文件中，需调用前使用configFromFile或UIT_LOG_USE_SYSTEM_CONFIG才有效
	static void setLevel(int channels, Log::Level level, bool perpetual = false);

	//获取日志等级
	//channel：通道
	static Log::Level getLevel(Channel channel);

	//设置文件通道属性
	//console支持的属性为{ "level" : ["debug", "information", "warning", "error", "fatal", "none"] }
	//file支持的属性有：
	//{ "path" : ""} log文件的输出位置
	//{ "level" : ["debug", "information", "warning", "error", "fatal", "none"] }
	//{	"rotation" : [ "n", "n K", "n M", "24:00", "daily", "weekly", "monthly", "n hours", "n weeks", "n months"]，其中，n表示整数。日志循环方式，超过尺寸或满足一定条件将建立/覆盖（如果文件数达上限）新的log文件
	//{ "archive" : ["timestamp", "number"]} 归档日志的目录名后缀
	//{ "times" : ["local", "utc"]} 日期格式
	//{ "compress" : ["true", "false"]} 自动压缩
	//{ "purgeAge" : ["n minutes", "n hours", "n days", "n weeks", "n months"]} 文档期限差值，超过的文件将被删除 ，比如5 days
	//{	"purgeCount" : "n"}文件数量限制，超过后最早的文件将被删除
	//注意：不要把所有的进程的Log::file的path属性都设置为同一路径，否则会导致多进程读写同一log文件而挂死！！！！！！！！！！！！
	static bool setFileChannelProperty(const std::string &name, const std::string &value);
	static bool getFileChannelProperty(const std::string &name, std::string &value);

	//格式化信息输出
	//module：模块名，形如"core|test"
	//format：格式化字符串，使用方式与printf的可变参数列表一样
	//系统会根据level对这些函数的输出权限进行过滤
	static void debug(const std::string &module, const char *format, ...);
	static void info(const std::string &module, const char *format, ...);
	static void warn(const std::string &module, const char *format, ...);
	static void error(const std::string &module, const char *format, ...);
	static void fatal(const std::string &module, const char *format, ...);

};

}

//使用系统配置初始化log系统
#define UIT_LOG_USE_SYSTEM_CONFIG	Log::configFromFile(uit::Runtime::getUitEtcDirectory() + "uit.log.conf");