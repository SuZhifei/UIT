#include "ViewApp.h"
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

#define DATAFW_CONTROLLER_APP_LOG_TAG	"datafw|view-app"
#define DEFAULT_DATA_EVENT_DIR			uit::Runtime::getUitEtcDirectory() + "data-event"

int ViewApp::main(const std::vector<std::string>& args)
{
#ifndef WIN32	//someip for win32退出会挂死，所以需要按ctrl+c来停止
	if (m_cmdType != ViewApp::help && m_cmdType != ViewApp::post_event)
#endif
		while (1)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

	return Application::EXIT_OK;
}

void ViewApp::initialize(Application & app)
{
	Application::initialize(app);
	if(m_cmdType == ViewApp::help)
		return;

	m_view.SyncedModelEvent += std::bind(&ViewApp::onSyncedModel, this, std::placeholders::_1);
	m_view.BooleanChangedEvent += std::bind(&ViewApp::onBooleanChanged, this, std::placeholders::_1);
	m_view.IntegerChangedEvent += std::bind(&ViewApp::onIntegerChanged, this, std::placeholders::_1);
	m_view.RealChangedEvent += std::bind(&ViewApp::onRealChanged, this, std::placeholders::_1);
	m_view.StringChangedEvent += std::bind(&ViewApp::onStringChanged, this, std::placeholders::_1);
	m_view.EnumChangedEvent += std::bind(&ViewApp::onEnumChanged, this, std::placeholders::_1);
	m_view.ImageChangedEvent += std::bind(&ViewApp::onImageChanged, this, std::placeholders::_1);
	m_view.ListResizeEvent += std::bind(&ViewApp::onListResize, this, std::placeholders::_1);
	m_view.ListInsertItemsEvent += std::bind(&ViewApp::onListInsertItems, this, std::placeholders::_1);
	m_view.ListRemoveItemsEvent += std::bind(&ViewApp::onListRemoveItems, this, std::placeholders::_1);
	m_view.ListUpdateViewportEvent += std::bind(&ViewApp::onListUpdateViewport, this, std::placeholders::_1);
	m_view.ListCheckItemEvent += std::bind(&ViewApp::onListCheckItem, this, std::placeholders::_1);
	m_view.ListFocusItemEvent += std::bind(&ViewApp::onListFocusItem, this, std::placeholders::_1);
	m_view.ListItemsUpdatedEvent += std::bind(&ViewApp::onListUpdateItems, this, std::placeholders::_1);
	m_view.VKeyEvent += std::bind(&ViewApp::onVkey, this, std::placeholders::_1);

	uint64_t k = uit::getTickCount();
	std::string sDir = m_optDataEventDir.empty() ? DEFAULT_DATA_EVENT_DIR : m_optDataEventDir;
	m_Parser.configureLoadingDir(sDir);
	bool b = m_Parser.parse();
	if (!b)
	{
		printf("view parse json error\r\n");
		return;
	}
	bool bSuccess = false;
	while (!bSuccess)
	{
		bSuccess = m_view.connect(m_Parser.getDataRoot());
	}
	printf("view connected, cost [%d] %s.\n", (int)(uit::getTickCount() - k), bSuccess ? "success" : "fail");

	switch (m_cmdType)
	{
	case ViewApp::post_event:
	{
		const std::string &eventPath = m_values[0];
		Poco::JSON::Object::Ptr args = new Poco::JSON::Object();
		for (int i = 1; i < m_values.size(); i += 2)
		{
			const std::string &argName = m_values[i];
			const std::string &argValue = m_values[i + 1];
			Poco::JSON::Object::Ptr e = m_Parser.findEvent(eventPath);
			if (e)
			{
				Poco::Dynamic::Var v = e->get(argName);
				switch (m_Parser.testType(v))
				{
				case ValueType::booleanValue:
				case ValueType::intValue:		args->set(argName, std::stoi(argValue));	break;
				case ValueType::realValue:		args->set(argName, std::stod(argValue));	break;
				case ValueType::enumValue:
				{
					try {
						args->set(argName, std::stoi(argValue));
					}
					catch (...) {
						args->set(argName, argValue);
					}
				}
				break;
				case ValueType::stringValue:	args->set(argName, argValue);				break;
				default:						args->set(argName, argValue);				break;
				}
			}
			else
			{
				args->set(argName, argValue);
			}
		}
		m_view.postEvent(m_values[0], args);
	}
	break;
	default:
		break;
	}
}

void ViewApp::uninitialize()
{
	Application::uninitialize();
}

void ViewApp::defineOptions(OptionSet & options)
{
	Application::defineOptions(options);
	//shortName有很多限制，比如必须是fullName的开头，不能有多个Option shortName相似等。不设置了
	options.addOption(Option("dir", "", "specify data event dir", false, "dir").repeatable(false).callback(OptionCallback<ViewApp>(this, &ViewApp::handleDataEventDir)));
	options.addOption(Option("help", "", "display help imformation", false).repeatable(false).callback(OptionCallback<ViewApp>(this, &ViewApp::handleHelp)));
	options.addOption(Option("debug", "", "display help imformation", false).repeatable(false).callback(OptionCallback<ViewApp>(this, &ViewApp::handleDebug)));
	options.addOption(Option("post", "", "post event", false, "path#argname#argvalue#argname#argvalue...").repeatable(false).callback(OptionCallback<ViewApp>(this, &ViewApp::handlePostEvent)));
	
}

void ViewApp::handleDataEventDir(const std::string & name, const std::string & value)
{
	m_optDataEventDir = value;
}

void ViewApp::handleHelp(const std::string & name, const std::string & value)
{
	showHelp();
}

void ViewApp::handleDebug(const std::string & name, const std::string & value)
{
	m_cmdType = debug;
	stopOptionsProcessing();
}

void ViewApp::handlePostEvent(const std::string & name, const std::string & value)
{
	m_cmdType = post_event;
	Poco::StringTokenizer sk(value, "#", StringTokenizer::TOK_TRIM);
	m_values.assign(sk.begin(), sk.end());
	if (m_values.size() < 3 || m_values.size() % 2 == 0)
		showHelp();
	stopOptionsProcessing();
}

void ViewApp::showHelp()
{
	HelpFormatter hp(options());
	hp.format(std::cout);
	stopOptionsProcessing();
	m_cmdType = help;
}

void ViewApp::onSyncedModel(const View::SyncedModelArgs &args)
{
	//param.m_DataRoot是连接model后拿到的最新同步数据，使用它们来初始化gui中的元素
	//遍历数据，以构建与GUI相关的数据
	loopData(args.data, "");
	loopEvent(m_Parser.getEventRoot(), "");
	printf("on sync done, data[%d], event[%d]\n", m_DataCount, (int)m_Events.size());
}

void ViewApp::onBooleanChanged(const View::BooleanChangedArgs &args)
{
	printf("%s : %s\r\n", args.path.data(), args.value ? "true" : "false");
}

void ViewApp::onIntegerChanged(const View::IntegerChangedArgs &args)
{
	printf("%s : %d\r\n", args.path.data(), args.value);
}

void ViewApp::onRealChanged(const View::RealChangedArgs &args)
{
	printf("%s : %.2f\r\n", args.path.data(), args.value);
}

void ViewApp::onStringChanged(const View::StringChangedArgs &args)
{
	printf("%s : %s\r\n", args.path.data(), args.value.data());
}

void ViewApp::onEnumChanged(const View::EnumChangedArgs &args)
{
	printf("%s : %d(%s)\r\n", args.path.data(), args.intValue, args.stringValue.data());
}

void ViewApp::onImageChanged(const View::ImageChangedArgs &args)
{
	printf("image [%s] changed, size[%d], formatKnown[%s]\r\n", args.path.data(), args.value.bytes(), args.value.isFormatKnown() ? "true" : "false");
	if (args.value.isFormatKnown())
	{
		/*		Bitmap::PixelFormat bmFormat = Bitmap::Format_Invalid;
		switch (param.m_Image.format())
		{
		case rgb555Format:		bmFormat = Bitmap::Format_Bpp16_Rgb555;	break;
		case rgb565Format:		bmFormat = Bitmap::Format_Bpp16_Rgb565;	break;
		case rgb888Format:		bmFormat = Bitmap::Format_Bpp24_Rgb888;	break;
		case rgb8888Format:	bmFormat = Bitmap::Format_Bpp32_Rgba8888; break;
		default:	break;
		}
		Bitmap bm((const unsigned char *)param.m_Image.getData(), param.m_Image.width(), param.m_Image.height(), bmFormat);
		bm.saveAsFile("d:/from_known_format.png");*/
	}
	else
	{
		FILE *pFile = fopen("d:/from_unknown_format.png", "wb");
		if(pFile)
		{
			fwrite(const_cast<View::ImageChangedArgs &>(args).value.data(), 1, args.value.bytes(), pFile);
			fclose(pFile);
		}
	}
}

void ViewApp::onListResize(const View::ListResizeArgs &args)
{
	printf("list [%s] resize to %d\r\n", args.path.data(), args.size);
}

void ViewApp::onListInsertItems(const View::ListInsertItemsArgs &args)
{
	printf("list [%s] insert items beg %d, count %d\r\n", args.path.data(), args.beg, args.count);
}

void ViewApp::onListRemoveItems(const View::ListRemoveItemsArgs &args)
{
	printf("list [%s] remove items beg %d, count %d\r\n", args.path.data(), args.beg, args.count);
}

void ViewApp::onListUpdateViewport(const View::ListUpdateViewportArgs &args)
{
	printf("list [%s] update viewport, topIndex %d\r\n", args.path.data(), args.beg);
}

void ViewApp::onListCheckItem(const View::ListCheckItemArgs &args)
{
	printf("list [%s] check item %d\r\n", args.path.data(), args.index);
}

void ViewApp::onListFocusItem(const View::ListFocusItemArgs &args)
{
	printf("list [%s] focus item %d\r\n", args.path.data(), args.index);
}

void ViewApp::onListUpdateItems(const View::ListItemsUpdatedArgs &args)
{
	const std::set<unsigned int> &indexs = args.indices;
	printf("list [%s] update items, count %d\r\n", args.path.data(), (int)indexs.size());

	Poco::JSON::Array::Ptr items = args.source;
	int i = 0;
	for (std::set<unsigned int>::const_iterator iter = indexs.begin(); iter != indexs.end() && i < items->size(); ++iter, ++i)
	{
		//更新的目标item index
		int updateIndex = *iter;
		//更新的内容
		Poco::JSON::Object::Ptr item = items->getObject(i);
		for (Poco::JSON::Object::ConstIterator iterItem = item->begin(); iterItem != item->end(); ++iterItem)
		{
			std::string sName = iterItem->first;
			Poco::Dynamic::Var v = iterItem->second;
			ValueType t = m_Parser.testType(v);
			switch (t)
			{
			case booleanValue:					break;
			case intValue:						break;
			case realValue:						break;
			case stringValue:					break;
			case imageValue:
			{
				ImageData img = m_Parser.extractImage(v);
				break;
			}
			case enumValue:						break;
			case listValue:						break;
			case objectValue:					break;
			default:							break;
			}
			//kanziObject[sName]->update();
		}
	}
}

void ViewApp::onVkey(const View::VKeyArgs &args)
{
	printf("vkey event, key=%s, state=%s\r\n", toString(args.key).data(), toString(args.state).data());
}

void ViewApp::loopData(Poco::JSON::Object::Ptr obj, const std::string &sParent)
{
	for (Poco::JSON::Object::ConstIterator iter = obj->begin(); iter != obj->end(); ++iter)
	{
		const std::string &sName = iter->first;
		std::string sCombine = sParent + "." + sName;
		if (sCombine[0] == '.')
			sCombine = sCombine.substr(1);
		const Poco::Dynamic::Var &v = iter->second;
		ValueType type = m_Parser.testType(v);
		switch (type)
		{
		case booleanValue:			++m_DataCount;		break;
		case intValue:				++m_DataCount;		break;
		case realValue:				++m_DataCount;		break;
		case stringValue:			++m_DataCount;		break;
		case imageValue:
		{
			++m_DataCount;
			ImageData img = m_Parser.extractImage(v);

			if (img.isFormatKnown())
			{
				/*	Bitmap::PixelFormat bmFormat = Bitmap::Format_Invalid;
				switch (img.format())
				{
				case rgb555Format:		bmFormat = Bitmap::Format_Bpp16_Rgb555;	break;
				case rgb565Format:		bmFormat = Bitmap::Format_Bpp16_Rgb565;	break;
				case rgb888Format:		bmFormat = Bitmap::Format_Bpp24_Rgb888;	break;
				case rgb8888Format:	bmFormat = Bitmap::Format_Bpp32_Rgba8888; break;
				default:	break;
				}*/
				//	Bitmap bm((const unsigned char *)img.getData(), img.width(), img.height(), bmFormat);
				//	bm.SaveAsFile("d:/from_known_format.png");
			}
			else
			{
				//	Bitmap bm;
				//	bm.LoadData((const unsigned char *)img.getData(), img.bytes());
				//	bm.SaveAsFile("d:/from_unknown_format.png");
			}
			break;
		}
		case enumValue:
		{
			++m_DataCount;
			std::vector<std::string> stringValues;
			std::string sEnumDefinitionName;
			bool b = m_Parser.extractEnumDefinition(v, sEnumDefinitionName, stringValues);
			break;
		}
		case listValue:
		{
			Poco::JSON::Object::Ptr itemTemplate = m_Parser.extractItemTemplate(v);
			Poco::JSON::Array::Ptr items = m_Parser.extractListItems(v);
			//std::string sJson = Poco::Dynamic::Var(list).toString();
			for (int i = 0; i != items->size(); ++i)
			{
				Poco::JSON::Object::Ptr item = items->get(i).extract<Poco::JSON::Object::Ptr>();
				for (Poco::JSON::Object::ConstIterator iterItem = item->begin(); iterItem != item->end(); ++iterItem)
				{
					const std::string &sColName = iterItem->first;
					const Poco::Dynamic::Var &colV = iterItem->second;
					switch (m_Parser.testType(colV))
					{
					case booleanValue:		break;
					case intValue:			break;
					case realValue:			break;
					case stringValue:		break;
					case enumValue:
					{
						std::vector<std::string> stringValues;
						std::string sEnumDefinitionName;
						bool b = m_Parser.extractEnumDefinition(colV, sEnumDefinitionName, stringValues);
						break;
					}
					case imageValue:
					{
						ImageData img = m_Parser.extractImage(colV);
						break;
					}
					default:		printf("unknown list element type.\r\n"); break;
					}
				}

			}

			break;
		}
		case objectValue:	loopData(v.extract<Poco::JSON::Object::Ptr>(), sCombine);	break;
		default:		printf("[%s]unknown type.\r\n", sCombine.data()); break;
		}
	}
}

void ViewApp::loopEvent(Poco::JSON::Object::Ptr obj, const std::string & sParent)
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
			m_Events[sEvent] = args;
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
			loopEvent(obj, sCombine);
		}
	}
}
