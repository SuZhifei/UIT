#pragma once
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Util/Application.h>
#include <Poco/Timer.h>
#include "screenfw/ScreenController.h"

namespace uit { namespace screenfw {

class ScreenControllerApp : public Poco::Util::Application
{
public:
	virtual int main(const std::vector<std::string>& args) override;
	virtual void initialize(Application& app) override;
	virtual void uninitialize() override;
	virtual void defineOptions(Poco::Util::OptionSet& options) override;

	Poco::Timer m_timer;
	void onTimeout_visible(Poco::Timer& timer);
	void onTimeout_position(Poco::Timer& timer);
	void onTimeout_size(Poco::Timer& timer);
	void onTimeout_rect(Poco::Timer& timer);
	void onTimeout_clip_rect(Poco::Timer& timer);

private:
	enum CmdType
	{
		help = 0,
		debug,
		auto_test,
	};
	void handleHelp(const std::string &name, const std::string &value);
	void handleDebug(const std::string &name, const std::string &value);

	void showHelp();
	CmdType						m_cmdType{debug};
};

}}
