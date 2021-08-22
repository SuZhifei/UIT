#include "Log_Internal.h"
#include <chrono>
#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/Util/JSONConfiguration.h>

using namespace uit;
#define LOG_DATA_NAME			"uit_log.txt"
#define LOG_CONSOLE_NAME		"ConsoleLog"	//也作为配置文件的ConsoleLog节
#define LOG_FILE_NAME			"FileLog"		//也作为配置文件的FileLog节
static Poco::Logger &m_fileLog = Poco::Logger::get(LOG_FILE_NAME);

Log::Level fromString(const std::string &s)
{
	if (s == "debug")		return Log::Level::debug;
	if (s == "information")	return Log::Level::information;
	if (s == "warning")		return Log::Level::warning;
	if (s == "error")		return Log::Level::error;
	if (s == "fatal")		return Log::Level::fatal;
	return Log::Level::none;
}

std::string toString(Log::Level level)
{
	switch (level)
	{
	case Log::Level::debug:			return "debug";
	case Log::Level::information:	return "information";
	case Log::Level::warning:		return "warning";
	case Log::Level::error:			return "error";
	case Log::Level::fatal:			return "fatal";
	default:						return "none";
	}
}

void ConsoleChannel::print(Log::Level level, const std::string & msg) const
{
	if (level >= m_filter && m_filter != Log::Level::none)
		printf("%s\n", msg.data());
}

void ConsoleChannel::setLevel(Log::Level level)
{
	m_filter = level;
}

//////////////
Log::Level ConsoleChannel::getLevel() const
{
	return m_filter;
}

FileChannel::FileChannel()
{
	m_channel = new Poco::FileChannel();
	m_fileLog.setChannel(m_channel);
	m_fileLog.setLevel(Poco::Message::PRIO_TRACE);	//设置到最低，表示都打印，不需要poco内部的等级控制
}

void FileChannel::print(Log::Level level, const std::string & msg) const
{
	if (level >= m_filter && m_filter != Log::Level::none)
	{
		if (Poco::Path(m_channel->path()).isFile())
			try {
			//直接使用最低等级trace，因为不需要使用Poco::FileChannel内部等级控制
			m_fileLog.trace(msg);
		}
		catch (...) {}
	}
}

void FileChannel::setLevel(Log::Level level)
{
	setProperty("level", toString(level));
}

Log::Level FileChannel::getLevel() const
{
	return m_filter;
}

void FileChannel::setPath(const std::string &path)
{
	setProperty("path", path);
}

std::string FileChannel::getPath() const
{
	return m_channel->path();
}

bool FileChannel::setProperty(const std::string & name, const std::string & value)
{
	try {
		if (name == "level")
		{
			m_filter = fromString(value);
		}
		else if (name == "path" && !value.empty())
		{
			Poco::Path p(value);
			Poco::File f(p.parent());
			//保证文件夹的存在才能保证logfile时可以创建log文件
			f.createDirectories();
			m_channel->setProperty(name, value);
		}
		else
		{
			m_channel->setProperty(name, value);
		}
		return true;
	}
	catch (...) {
		return false;
	}
}

bool FileChannel::getProperty(const std::string & name, std::string & value) const
{
	value = m_channel->getProperty(name);
	return true;
}

/////////////////////
Log_Internal *Log_Internal::g_instance = nullptr;
Log_Internal * Log_Internal::getInstance()
{
	if (!g_instance)
		g_instance = new Log_Internal();
	return g_instance;
}

bool Log_Internal::configFromFile(const std::string & path)
{
	auto k = getTickCount();

	bool ret = false;
	try {
		m_conf = new Poco::Util::PropertyFileConfiguration(path);
		std::string name = std::string(LOG_CONSOLE_NAME) + "." + "level";
		if (m_conf->hasProperty(name))
			m_consoleChannel.setLevel(fromString(m_conf->getString(name)));

		std::vector<std::string> propertys;
		m_conf->keys(LOG_FILE_NAME, propertys);
		for (auto const &p : propertys)
			bool b = setFileChannelProperty(p, m_conf->getString(std::string(LOG_FILE_NAME) + "." + p));

		ret = true;
	}
	catch (...) { 
		ret = false;
	}

	printf("config uit log system from file [%s] %s cost %d ms.\n[%s.level=%s]\n[%s.level=%s]\n[%s.path=%s]\n", path.data(), ret ? "success" : "fail", (int)(getTickCount() - k),
		LOG_CONSOLE_NAME, toString(m_consoleChannel.getLevel()).data(), LOG_FILE_NAME, toString(m_fileChannel.getLevel()).data(), LOG_FILE_NAME, m_fileChannel.getPath().data());
	m_confed = ret;
	m_confPath = path;
	return ret;
}

std::string Log_Internal::getConfigFile() const
{
	return m_confPath;
}

void Log_Internal::setLevel(int channels, Log::Level level, bool perpetual)
{
	if (testChannel(channels, Log::console))
	{
		m_consoleChannel.setLevel(level);
		std::string name = std::string(LOG_CONSOLE_NAME) + "." + "level";
		if (perpetual && m_confed && m_conf->hasProperty(name))
		{
			m_conf->setString(name, toString(level));
			m_conf->save(m_confPath);
		}
	}
	if (testChannel(channels, Log::file))
	{
		m_fileChannel.setLevel(level);
		std::string name = std::string(LOG_FILE_NAME) + "." + "level";
		if (perpetual && m_confed && m_conf->has(name))
		{
			m_conf->setString(name, toString(level));
			m_conf->save(m_confPath);
		}
	}
}

Log::Level Log_Internal::getLevel(Log::Channel channel) const
{
	switch (channel)
	{
	case Log::console:	return m_consoleChannel.getLevel();
	case Log::file:		return m_fileChannel.getLevel();
	default:			return Log::Level::none;
	}
	
}

bool Log_Internal::setFileChannelProperty(const std::string & name, const std::string & value)
{
	return m_fileChannel.setProperty(name, value);
}

bool Log_Internal::getFileChannelProperty(const std::string & name, std::string &value) const
{
	return m_fileChannel.getProperty(name, value);
}

void Log_Internal::print(Log::Level level, const std::string & msg)
{
	m_consoleChannel.print(level, msg);
	m_fileChannel.print(level, msg);
}

bool Log_Internal::testChannel(int channels, Log::Channel c) const
{
	return ((channels & c) == c) && (c != 0 || channels == c);
}
