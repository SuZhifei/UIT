#pragma once
#include <Poco/Logger.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/SimpleFileChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/AutoPtr.h>
#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/Util/PropertyFileConfiguration.h>
#include "core/Log.h"

namespace uit {

class ConsoleChannel
{
public:
	void print(Log::Level level, const std::string &msg) const;
	void setLevel(Log::Level level);
	Log::Level getLevel() const;

private:
	Log::Level		m_filter{Log::Level::debug};
};

class FileChannel
{
public:
	FileChannel();
	void print(Log::Level level, const std::string &msg) const;
	void setLevel(Log::Level level);
	Log::Level getLevel() const;
	void setPath(const std::string &path);
	std::string getPath() const;
	bool setProperty(const std::string &name, const std::string &value);
	bool getProperty(const std::string &name, std::string &value) const;

private:
	Log::Level							m_filter{ Log::Level::debug };
	Poco::AutoPtr<Poco::FileChannel>	m_channel;
};


class Log_Internal
{
public:
	static Log_Internal *getInstance();

	bool configFromFile(const std::string &path);
	std::string getConfigFile() const;
	void setLevel(int channels, Log::Level level, bool perpetual);
	Log::Level getLevel(Log::Channel channel) const;
	bool setFileChannelProperty(const std::string &name, const std::string &value);
	bool getFileChannelProperty(const std::string &name, std::string &value) const;
	void print(Log::Level level, const std::string &msg);

private:
	bool testChannel(int channels, Log::Channel c) const;

	std::string												m_confPath;
	bool													m_confed{ false };
	Poco::AutoPtr<Poco::Util::PropertyFileConfiguration>	m_conf;
	ConsoleChannel											m_consoleChannel;
	FileChannel												m_fileChannel;
	static Log_Internal										*g_instance;
};

}