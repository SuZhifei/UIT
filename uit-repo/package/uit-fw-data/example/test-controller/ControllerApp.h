#pragma once
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Util/Application.h>
#include "datafw/Application.h"
#include "datafw/Controller.h"
#include "datafw/Parser.h"

namespace uit { namespace datafw {

class ControllerApp : public Poco::Util::Application
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
		auto_test,
		set_bool,
		set_int,
		set_real,
		set_string,
		set_enum,
		resize_list,
		insert_list,
		remove_list,
		check_list,
		focus_list,
		set_list_viewport,
		set_list_element_bool,
		set_list_element_int,
		set_list_element_real,
		set_list_element_string,
		set_list_element_enum,
		vkey,
	};

	void handleDataEventDir(const std::string &name, const std::string &value);
	void handleHelp(const std::string &name, const std::string &value);
	void handleDebug(const std::string &name, const std::string &value);
	void handleSetBool(const std::string &name, const std::string &value);
	void handleSetInteger(const std::string &name, const std::string &value);
	void handleSetReal(const std::string &name, const std::string &value);
	void handleSetString(const std::string &name, const std::string &value);
	void handleSetEnum(const std::string &name, const std::string &value);
	void handleResizeList(const std::string &name, const std::string &value);
	void handleInsertList(const std::string &name, const std::string &value);
	void handleRemoveList(const std::string &name, const std::string &value);
	void handleCheckList(const std::string &name, const std::string &value);
	void handleFocusList(const std::string &name, const std::string &value);
	void handleSetViewport(const std::string &name, const std::string &value);
	void handleSetListBool(const std::string &name, const std::string &value);
	void handleSetListInteger(const std::string &name, const std::string &value);
	void handleSetListReal(const std::string &name, const std::string &value);
	void handleSetListString(const std::string &name, const std::string &value);
	void handleSetListEnum(const std::string &name, const std::string &value);
	void handleVKey(const std::string &name, const std::string &value);

	void loopRegisterEvent(Poco::JSON::Object::Ptr obj, const std::string &sParent, std::vector<std::string> &events);
	void onUserEvent(const Controller::UserArgs &args);
	ImageData loadImage(const std::string &path);
	void showHelp();
	Poco::JSON::Array::Ptr generateItem(ValueType t, const std::string & element, const std::string &value);

	std::string					m_optDataEventDir;
	CmdType						m_cmdType{debug};
	std::vector<std::string>	m_values;
	Parser						m_Parser;
};

}}
