#pragma once
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Util/Application.h>
#include "screenfw/ScreenListener.h"

namespace uit { namespace screenfw {

class ListenerApp : public Poco::Util::Application
{
public:
	virtual int main(const std::vector<std::string>& args) override;
	virtual void initialize(Application& app) override;
	virtual void uninitialize() override;
	virtual void defineOptions(Poco::Util::OptionSet& options) override;

private:
	enum CmdType
	{
		help = 0,
		debug,
		post_event,
	};

	void handleHelp(const std::string &name, const std::string &value);
	void handleDebug(const std::string &name, const std::string &value);
	void handlePostEvent(const std::string &name, const std::string &value);
	void showHelp();

	CmdType						m_cmdType{debug};
	std::vector<std::string>	m_values;
	ScreenListener						m_listener;
	std::map<std::string, Poco::JSON::Object::Ptr>	m_Events;
};

}}

POCO_APP_MAIN(uit::screenfw::ListenerApp)