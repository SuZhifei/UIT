#pragma once
#include <iostream>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Application.h>
#include "../libscreenfw/Model.h"

namespace uit { namespace screenfw {

class ScreenService : public Poco::Util::ServerApplication
{
public:
	virtual int main(const std::vector<std::string>& args) override;
	virtual void initialize(Application& app) override;
	virtual void uninitialize() override;
	virtual void defineOptions(Poco::Util::OptionSet& options) override;

private:
	void handleHelp(const std::string &name, const std::string &value);
	void handleDataEventDir(const std::string &name, const std::string &value);

	std::string			m_optDataEventDir;
	bool				m_bHelp{ false };
	Model				m_model;
};

}}

POCO_SERVER_MAIN(uit::screenfw::ScreenService)
