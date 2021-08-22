#pragma once
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Util/Application.h>
#include "datafw/View.h"
#include "datafw/Parser.h"

namespace uit { namespace datafw {

class ViewApp : public Poco::Util::Application
{
public:
	virtual int main(const std::vector<std::string>& args) override;
	virtual void initialize(Application& app) override;
	virtual void uninitialize() override;
	virtual void defineOptions(Poco::Util::OptionSet& options) override;

	void onSyncedModel(const View::SyncedModelArgs &args);
	void onBooleanChanged(const View::BooleanChangedArgs &args);
	void onIntegerChanged(const View::IntegerChangedArgs &args);
	void onRealChanged(const View::RealChangedArgs &args);
	void onStringChanged(const View::StringChangedArgs &args);
	void onEnumChanged(const View::EnumChangedArgs &args);
	void onImageChanged(const View::ImageChangedArgs &args);
	void onListResize(const View::ListResizeArgs &args);
	void onListInsertItems(const View::ListInsertItemsArgs &args);
	void onListRemoveItems(const View::ListRemoveItemsArgs &args);
	void onListUpdateViewport(const View::ListUpdateViewportArgs &args);
	void onListCheckItem(const View::ListCheckItemArgs &args);
	void onListFocusItem(const View::ListFocusItemArgs &args);
	void onListUpdateItems(const View::ListItemsUpdatedArgs &args);
	void onVkey(const View::VKeyArgs &args);

private:
	enum CmdType
	{
		help = 0,
		debug,
		post_event,
	};

	void handleDataEventDir(const std::string &name, const std::string &value);
	void handleHelp(const std::string &name, const std::string &value);
	void handleDebug(const std::string &name, const std::string &value);
	void handlePostEvent(const std::string &name, const std::string &value);
	void showHelp();

	void loopData(Poco::JSON::Object::Ptr obj, const std::string &sParent);
	void loopEvent(Poco::JSON::Object::Ptr obj, const std::string & sParent);

	std::string					m_optDataEventDir;
	CmdType						m_cmdType{debug};
	std::vector<std::string>	m_values;
	View						m_view;
	Parser						m_Parser;
	uint32_t					m_DataCount{ 0 };
	std::map<std::string, Poco::JSON::Object::Ptr>	m_Events;
};

}}

POCO_APP_MAIN(uit::datafw::ViewApp)