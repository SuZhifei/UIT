#include "core/Log.h"
#include "Log_Internal.h"
#include <thread>
#include <Poco/Thread.h>
#include <Poco/Process.h>
#include <Poco/LocalDateTime.h>
#include <stdarg.h>

using namespace uit;

#define LOG_MAX_SIZE	1024
char logBuffer[LOG_MAX_SIZE] = { 0 };

//由于Poco的格式化不好用，自己定义格式
#define __LOG(levelStr, level) \
Poco::LocalDateTime dt; \
int nPrefix = snprintf(logBuffer, sizeof(logBuffer), "[%d-%d-%d %02d:%02d:%02d.%03d.%03d](%u|0x%08lX) [%s] %s: ", dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second(), \
	dt.millisecond(), dt.microsecond(), Poco::Process::id(), Poco::Thread::currentTid(), module.data(), #levelStr); \
va_list args; \
va_start(args, format); \
vsnprintf(logBuffer + nPrefix, sizeof(logBuffer) - nPrefix, format, args);	\
va_end(args); \
Log_Internal::getInstance()->print(level, logBuffer); \

bool Log::configFromFile(const std::string & path)
{
	return Log_Internal::getInstance()->configFromFile(path);;
}

std::string Log::getConfigFile()
{
	return Log_Internal::getInstance()->getConfigFile();
}

void Log::setLevel(int types, Log::Level level, bool perpetual)
{
	Log_Internal::getInstance()->setLevel(types, level, perpetual);
}

Log::Level Log::getLevel(Channel channel)
{
	return Log_Internal::getInstance()->getLevel(channel);
}

bool Log::setFileChannelProperty(const std::string & name, const std::string & value)
{
	return Log_Internal::getInstance()->setFileChannelProperty(name, value);
}

bool Log::getFileChannelProperty(const std::string & name, std::string &value)
{
	return Log_Internal::getInstance()->getFileChannelProperty(name, value);
}

void Log::debug(const std::string & module, const char * format, ...)
{
	__LOG(DEBUG, Level::debug);
}

void Log::info(const std::string & module, const char * format, ...)
{
	__LOG(INFO, Level::information);
}

void Log::warn(const std::string & module, const char * format, ...)
{
	__LOG(WARN, Level::warning);
}

void Log::error(const std::string & module, const char * format, ...)
{
	__LOG(ERROR, Level::error);
}

void Log::fatal(const std::string & module, const char * format, ...)
{
	__LOG(FATAL, Level::fatal);
}
