#include "ControllerApp.h"
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/StringTokenizer.h>
#include <uit/core/Log.h>
#include <uit/core/Runtime.h>
#include <cassert>
#include <thread>

using namespace uit::datafw;
using namespace Poco;
using namespace Poco::Util;

#define DATAFW_CONTROLLER_APP_LOG_TAG	"datafw|controller-app"
#define DEFAULT_DATA_EVENT_DIR			uit::Runtime::getUitEtcDirectory() + "data-event"

#if 0
class TestApp : public uit::datafw::Application
{
public:
	virtual void onIdle() override
	{
		printf("TestApp::onIdle\n");
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
};
int main(int argc, char **argv)
{
	TestApp app;
	return app.run();
}
#else
POCO_APP_MAIN(uit::datafw::ControllerApp)
#endif

int ControllerApp::main(const std::vector<std::string>& args)
{
#ifndef WIN32	//someip for win32退出会挂死，所以需要按ctrl+c来停止
	if(m_cmdType == debug || m_cmdType == auto_test)
#endif
		while (1)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	return Application::EXIT_OK;
}

void ControllerApp::initialize(Application & app)
{
	Application::initialize(app);
	if(m_cmdType == ControllerApp::help)
		return;
	//注册所有事件
	Controller::connect(5 * 1000);
	Controller::userEvent() += std::bind(&ControllerApp::onUserEvent, this, std::placeholders::_1);
	std::string sDataEventDir = m_optDataEventDir.empty() ? DEFAULT_DATA_EVENT_DIR : m_optDataEventDir;
	m_Parser.configureLoadingDir(sDataEventDir);
	m_Parser.parse();
	std::vector<std::string> events;
	loopRegisterEvent(m_Parser.getEventRoot(), "", events);
	Controller::registerEvents(events);

	try {
		switch (m_cmdType)
		{
		case ControllerApp::debug:
		{
			Controller::setBoolean("Upgrade.Online.NewVersionFound", true);
			Controller::setInteger("DrivingInfo.Current.Revs", 10);
			Controller::setReal("DrivingInfo.Current.Speed", 100.2);
			Controller::setString("Upgrade.Online.Version", "Desay");
			Controller::setEnum("DrivingInfo.Current.Gear", 3);
			Controller::setEnum("DrivingInfo.Current.Gear", "N");
			Controller::setImage("Music.CurrentMediaInfo.Picture", loadImage(uit::Runtime::getUitResourceDirectory() + "dataservice/uit.test.data.jpg"));

			//以组的方式更新
			std::vector<std::pair<std::string, Poco::Dynamic::Var>> nodes{
				{ "Engine.OilLowPressure", "On" },
				{ "Engine.StartStopSystem", "Invalid" },
				{ "Engine.EmissionsError", "On" },
				{ "Engine.Tcu", "MinorFailure" },
				{ "BodyControl.Seat.Seat0.AirBag", "Activated" },
				{ "BodyControl.Seat.Seat0.Belt", "Activated" },
				{ "Engine.EpsError", "On" },
				{ "Engine.BrakeFluidLow", "On" },
				{ "Engine.AbsError", "On" },
				{ "Engine.EspSwitch", "On" },
				{ "Engine.Esp", "Activated" },
				{ "Engine.Hdc", "Activated" },
				{ "Engine.Avh", "Activated" },
				{ "Engine.Epb", "Unstuck" },
				{ "Engine.EpbError", "Unstuck" },
				{ "Phev.Charge", "Charging" },
				{ "BodyControl.DoorWindow.DoorWarning", "On" },
				{ "Engine.TpmsError", "On" },
				{ "Light.LeftTurn", "Activated" },
				{ "Light.RightTurn", "Activated" },
				{ "Light.RearFog", "Activated" },
				{ "Light.Position", "Activated" },
				{ "Light.HighBeam", "Activated" },
				{ "Light.LowBeam", "Activated" },
				{ "Engine.WaterOverTemp", "On" },
				{ "DrivingInfo.Current.OilLow", "On" },
				{ "DrivingInfo.Current.OverSpeed", "On" },
				{ "System.Warning", "On" },
			};

			Controller::setGroup(nodes);
			std::this_thread::sleep_for(std::chrono::seconds(2));

			std::vector<std::pair<std::string, Poco::Dynamic::Var>> nodes1{
				{ "Engine.OilLowPressure", "Off" },
				{ "Engine.StartStopSystem", "Valid" },
				{ "Engine.EmissionsError", "Off" },
				{ "Engine.Tcu", "MinorFailure" },
				{ "BodyControl.Seat.Seat0.AirBag", "Valid" },
				{ "BodyControl.Seat.Seat0.Belt", "Valid" },
				{ "Engine.EpsError", "Off" },
				{ "Engine.BrakeFluidLow", "Off" },
				{ "Engine.AbsError", "Off" },
				{ "Engine.EspSwitch", "Off" },
				{ "Engine.Esp", "Valid" },
				{ "Engine.Hdc", "Valid" },
				{ "Engine.Avh", "Valid" },
				{ "Engine.Epb", "Unstuck" },
				{ "Engine.EpbError", "Unstuck" },
				{ "Phev.Charge", "Charging" },
				{ "BodyControl.DoorWindow.DoorWarning", "Off" },
				{ "Engine.TpmsError", "Off" },
				{ "Light.LeftTurn", "Valid" },
				{ "Light.RightTurn", "Valid" },
				{ "Light.RearFog", "Valid" },
				{ "Light.Position", "Valid" },
				{ "Light.HighBeam", "Valid" },
				{ "Light.LowBeam", "Valid" },
				{ "Engine.WaterOverTemp", "Off" },
				{ "DrivingInfo.Current.OilLow", "Off" },
				{ "DrivingInfo.Current.OverSpeed", "Off" },
				{ "System.Warning", "Off" },
			};
			Controller::setGroup(nodes1);

			std::this_thread::sleep_for(std::chrono::seconds(2));
			Controller::setGroup(nodes);

			//更新列表属性
			Controller::resizeList("Music.MediaList", 10);
			Controller::insertListItems("Music.MediaList", 2, 3);
			Controller::removeListItems("Music.MediaList", 0, 5);
			Controller::updateListViewport("Music.MediaList", 3);
			Controller::checkListItem("Music.MediaList", 6);
			Controller::focusListItem("Music.MediaList", 11);

			//更新列表items数据
			std::set<unsigned int> indexs = { 1, 2, 3, 4, 20 };
			Poco::JSON::Array::Ptr items = new Poco::JSON::Array();
			for (int i = 0; i != indexs.size(); ++i)
			{
			Poco::JSON::Object::Ptr item = new Poco::JSON::Object();
			item->set("FileName", "pxxxd");
			/*
			char arr[10] = {0};
			sprintf(arr, "e:/%d.jpg", i);
			ImageData img = loadImage(arr);
			item->set("Picture", img);*/
			items->add(item);
			}
			Controller::updateListItems("Music.MediaList", indexs, items);

			//
			Controller::vkey(vkeyUp, vkeyStatePress);
			Controller::vkey(vkeyDown, vkeyStateCancel);
			Controller::vkey(vkeyLeft, vkeyStateLongPress);
			Controller::vkey(vkeyRight, vkeyStateRelease);
			Controller::vkey(vkeyLeftUp, vkeyStateClick);
			Controller::vkey(vkeyRightUp, vkeyStateClick);
			Controller::vkey(vkeyLeftDown, vkeyStateClick);
			Controller::vkey(vkeyRightDown, vkeyStateClick);
			Controller::vkey(vkeyClockwise, vkeyStateClick);
			Controller::vkey(vkeyAntiClockwise, vkeyStateClick);
			Controller::vkey(vkeyOK, vkeyStateClick);
			Controller::vkey(vkeyBack, vkeyStateClick);

		}
		break;
		case ControllerApp::auto_test:																					break;
		case ControllerApp::set_bool:					Controller::setBoolean(m_values[0], std::stoi(m_values[1]));	break;
		case ControllerApp::set_int:					Controller::setInteger(m_values[0], std::stoi(m_values[1]));	break;
		case ControllerApp::set_real:					Controller::setReal(m_values[0], std::stod(m_values[1]));		break;
		case ControllerApp::set_string:					Controller::setString(m_values[0], m_values[1]);				break;
		case ControllerApp::set_enum:
		{
			assert(m_values.size() >= 2);
			try {
				Controller::setEnum(m_values[0], std::stoi(m_values[1]));
			}
			catch (...) {
				Controller::setEnum(m_values[0], m_values[1]);
			}
		}
		break;
		case ControllerApp::resize_list:
			Controller::resizeList(m_values[0], std::stoi(m_values[1]));
			break;
		case ControllerApp::insert_list:
			Controller::insertListItems(m_values[0], std::stoi(m_values[1]), std::stoi(m_values[2]));
			break;
		case ControllerApp::remove_list:
			Controller::removeListItems(m_values[0], std::stoi(m_values[1]), std::stoi(m_values[2]));
			break;
		case ControllerApp::check_list:
			Controller::checkListItem(m_values[0], std::stoi(m_values[1]));
			break;
		case ControllerApp::focus_list:
			Controller::focusListItem(m_values[0], std::stoi(m_values[1]));
			break;
		case ControllerApp::set_list_viewport:
			Controller::updateListViewport(m_values[0], std::stoi(m_values[1]));
			break;
		case ControllerApp::set_list_element_bool:		
		case ControllerApp::set_list_element_int:
		case ControllerApp::set_list_element_enum:
			Controller::updateListItems(m_values[0], { (uint32_t)std::stoi(m_values[1]) }, generateItem(ValueType::intValue, m_values[2], m_values[3]));
			break;
		case ControllerApp::set_list_element_string:
			Controller::updateListItems(m_values[0], { (uint32_t)std::stoi(m_values[1]) }, generateItem(ValueType::stringValue, m_values[2], m_values[3]));
			break;
		case ControllerApp::set_list_element_real:
			Controller::updateListItems(m_values[0], { (uint32_t)std::stoi(m_values[1]) }, generateItem(ValueType::realValue, m_values[2], m_values[3]));
			break;
		case ControllerApp::vkey:
			Controller::vkey((VKeyId)std::stoi(m_values[0]), (VKeyState)std::stoi(m_values[1]));
			break;
		default:
			break;
		}
	}
	catch (...) {
		showHelp();
	}
}

void ControllerApp::uninitialize()
{
	Application::uninitialize();
}

void ControllerApp::defineOptions(OptionSet & options)
{
	Application::defineOptions(options);
	//shortName有很多限制，比如必须是fullName的开头，不能有多个Option shortName相似等。不设置了
	options.addOption(Option("dir", "", "specify data event dir", false, "dir").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleDataEventDir)));
	options.addOption(Option("help", "", "display help imformation", false).repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleHelp)));
	options.addOption(Option("debug", "", "debug mode", false).repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleDebug)));
	options.addOption(Option("setbool", "", "set bool value", false, "path#int").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleSetBool)));
	options.addOption(Option("setint", "", "set integer value", false, "path#int").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleSetInteger)));
	options.addOption(Option("setreal", "", "set real value", false, "path#real").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleSetReal)));
	options.addOption(Option("setstring", "", "set string value", false, "path#string").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleSetString)));
	options.addOption(Option("setenum", "", "set enum value", false, "path#int|string").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleSetEnum)));
	options.addOption(Option("resizelist", "", "resize list", false, "path#int").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleResizeList)));
	options.addOption(Option("insertlist", "", "insert list", false, "path#int#int").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleInsertList)));
	options.addOption(Option("removelist", "", "remove list", false, "path#int#int").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleRemoveList)));
	options.addOption(Option("checklist", "", "check list", false, "path#int").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleCheckList)));
	options.addOption(Option("focuslist", "", "focus list", false, "path#int").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleFocusList)));
	options.addOption(Option("setlistviewport", "", "set list viewport", false, "path#int").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleSetViewport)));
	options.addOption(Option("setlistbool", "", "set list element bool value", false, "path#line#element#int").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleSetListBool)));
	options.addOption(Option("setlistint", "", "set list element integer value", false, "path#line#element#int").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleSetListInteger)));
	options.addOption(Option("setlistreal", "", "set list element real value", false, "path#line#element#real").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleSetListReal)));
	options.addOption(Option("setliststring", "", "set list element string value", false, "path#line#element#string").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleSetListString)));
	options.addOption(Option("setlistenum", "", "set list element enum value", false, "path#line#element#int|string").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleSetListEnum)));
	options.addOption(Option("vkey", "", "set vkey state", false, "int#int").repeatable(false).callback(OptionCallback<ControllerApp>(this, &ControllerApp::handleVKey)));
	
}

void ControllerApp::handleDataEventDir(const std::string & name, const std::string & value)
{
	m_optDataEventDir = value;
}

void ControllerApp::handleHelp(const std::string & name, const std::string & value)
{
	showHelp();
}

void ControllerApp::handleDebug(const std::string & name, const std::string & value)
{
	m_cmdType = debug;
	stopOptionsProcessing();
}

void ControllerApp::handleSetBool(const std::string & name, const std::string & value)
{
	m_cmdType = set_bool;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 2)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::handleSetInteger(const std::string & name, const std::string & value)
{
	m_cmdType = set_int;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 2)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::handleSetReal(const std::string & name, const std::string & value)
{
	m_cmdType = set_real;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 2)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::handleSetString(const std::string & name, const std::string & value)
{
	m_cmdType = set_string;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 2)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::handleSetEnum(const std::string & name, const std::string & value)
{
	m_cmdType = set_enum;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 2)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::handleResizeList(const std::string & name, const std::string & value)
{
	m_cmdType = resize_list;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 2)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::handleInsertList(const std::string & name, const std::string & value)
{
	m_cmdType = insert_list;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 3)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::handleRemoveList(const std::string & name, const std::string & value)
{
	m_cmdType = remove_list;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 3)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::handleCheckList(const std::string & name, const std::string & value)
{
	m_cmdType = check_list;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 2)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::handleFocusList(const std::string & name, const std::string & value)
{
	m_cmdType = focus_list;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 2)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::handleSetViewport(const std::string & name, const std::string & value)
{
	m_cmdType = set_list_viewport;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 2)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::handleSetListBool(const std::string & name, const std::string & value)
{
	m_cmdType = set_list_element_bool;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 4)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::handleSetListInteger(const std::string & name, const std::string & value)
{
	m_cmdType = set_list_element_int;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 4)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::handleSetListReal(const std::string & name, const std::string & value)
{
	m_cmdType = set_list_element_real;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 4)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::handleSetListString(const std::string & name, const std::string & value)
{
	m_cmdType = set_list_element_string;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 4)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::handleSetListEnum(const std::string & name, const std::string & value)
{
	m_cmdType = set_list_element_int;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 4)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::handleVKey(const std::string & name, const std::string & value)
{
	m_cmdType = vkey;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 2)
		showHelp();
	stopOptionsProcessing();
}

void ControllerApp::loopRegisterEvent(Poco::JSON::Object::Ptr obj, const std::string & sParent, std::vector<std::string> &events)
{
	for (Poco::JSON::Object::ConstIterator iter = obj->begin(); iter != obj->end(); ++iter)
	{
		const std::string &sName = iter->first;
		const Poco::Dynamic::Var &v = iter->second;
		std::string sCombine = sParent + "." + sName;
		std::string sEvent = sCombine.substr(1);
		Poco::JSON::Object::Ptr args = m_Parser.findEvent(sEvent);
		if (args)
		{
			events.push_back(sEvent);
		}
		else
		{
			Poco::JSON::Object::Ptr obj;
			try {
				obj = v.extract<Poco::JSON::Object::Ptr>();
			}
			catch (...)
			{
				printf("[%s] is not a obj event type, maybe [%s] is a event type, but it contain unknown arg type.\r\n", sCombine.substr(1).data(), sParent.substr(1).data());
				break;
			}
			loopRegisterEvent(obj, sCombine, events);
		}
	}
}

void ControllerApp::onUserEvent(const Controller::UserArgs & args)
{
	printf("onUserEvent:\n%s:%s\n", args.eventName.data(), Poco::Dynamic::Var(args.args).toString().data());
}

ImageData ControllerApp::loadImage(const std::string &path)
{
	FILE *pFile = fopen(path.data(), "rb");
	if (pFile == nullptr)
		return ImageData();

	fseek(pFile, 0, SEEK_END);
	int nLength = ftell(pFile);
	unsigned char *pData = new unsigned char[nLength];
	fseek(pFile, 0, SEEK_SET);
	fread(pData, nLength, 1, pFile);
	ImageData ret((uint8_t *)pData, nLength);
	delete[]pData;
	fclose(pFile);
	return ret;
}

void ControllerApp::showHelp()
{
	HelpFormatter hp(options());
	hp.format(std::cout);
	stopOptionsProcessing();
	m_cmdType = help;
}

Poco::JSON::Array::Ptr ControllerApp::generateItem(ValueType t, const std::string & element, const std::string & value)
{
	Poco::JSON::Array::Ptr items = new Poco::JSON::Array();
	Poco::JSON::Object::Ptr item = new Poco::JSON::Object();
	switch (t)
	{
	case booleanValue:	
	case intValue:
	case enumValue:	item->set(element, std::stoi(value));	break;
	case realValue:	item->set(element, std::stod(value));	break;
	case stringValue:	item->set(element, value);				break;
	default:					throw std::exception();					break;
	}
	items->add(item);
	return items;
}
