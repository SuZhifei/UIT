#include "datafw/Controller.h"
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Process.h>
#include <Poco/Thread.h>
#include <CommonAPI/CommonAPI.hpp>
#include <v1/uit/datafw/ControllerServerProxy.hpp>
#include "uit/core/Log.h"
#include "datafw/Parser.h"
#include "Share.h"

using namespace uit::datafw;
using namespace v1::uit::datafw;

static std::shared_ptr<ControllerServerProxy<>> g_controllerServerProxy;
static uit::Event<Controller::UserArgs>	UserEvent;

bool Controller::connect(uint32_t timeoutMs)
{
	static int i = 0;
	if (i == 0)
	{
		initEnv();
		Log::info(DATAFW_VIEW_LOG_TAG, "COMMONAPI_CONFIG=%s", getenv("COMMONAPI_CONFIG"));
		Log::info(DATAFW_VIEW_LOG_TAG, "VSOMEIP_CONFIGURATION=%s", getenv("VSOMEIP_CONFIGURATION"));
		++i;
	}

	auto k = uit::getTickCount();
	if (g_controllerServerProxy)
		return true;

	bool bTimeout = false;
	g_controllerServerProxy = buildProxy<ControllerServerProxy>(UIT_SERVER_DOMAIN, UIT_CONTROLLER_SERVER_INSTANCE, UIT_DATAFW_LIBBASE, timeoutMs, bTimeout);
	int pid = Poco::Process::id();
	if (g_controllerServerProxy)
	{
		g_controllerServerProxy->getUserActionSelectiveEvent().subscribe(onUserAction);
		Log::info(DATAFW_CONTROLLER_LOG_TAG, "controller[%s] connect success, cost [%d] ms.", uit::getProcessName(pid).data(), (int)(uit::getTickCount() - k));
	}
	else
	{
		bTimeout ? Log::error(DATAFW_CONTROLLER_LOG_TAG, "controller[%s] connect fail timeout for [%d] ms, maybe uit.model not run.", uit::getProcessName(pid).data(), timeoutMs) : Log::error(DATAFW_CONTROLLER_LOG_TAG, "controller[%s] connect fail for null proxy, maybe dbus-deamon not run.", uit::getProcessName(pid).data());
	}
	return g_controllerServerProxy != nullptr;
}

bool Controller::disconnect()
{
	//commonapi server无法知道client退出，所以做不了很多实际性的事情。但在server端尝试给client端发送事件的时候，无异常
	g_controllerServerProxy.reset();
	return true;
}

bool Controller::isConnected()
{
	return g_controllerServerProxy != nullptr;
}

void Controller::registerEvents(const std::vector<std::string> & sEventNames)
{
	checkConnectState(__FUNCTION__);
	g_controllerServerProxy->RegisterEventsAsync(Poco::Process::id(), sEventNames, [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "RegisterEventsAsync"); });
}

void Controller::setBoolean(const std::string &path, bool b)
{
	checkConnectState(__FUNCTION__);
	g_controllerServerProxy->SetBooleanAsync(path, b, uit::getTickCount(), [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "SetBooleanAsync"); });
}

void Controller::setInteger(const std::string &path, int32_t n)
{
	checkConnectState(__FUNCTION__);
	g_controllerServerProxy->SetIntegerAsync(path, n, uit::getTickCount(), [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "SetIntegerAsync"); });
}

void Controller::setEnum(const std::string &path, int32_t e)
{
	checkConnectState(__FUNCTION__);
	g_controllerServerProxy->SetEnumAsync(path, e, uit::getTickCount(), [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "SetEnumAsync"); });
}

void Controller::setEnum(const std::string &path, const std::string &enumStringValue)
{
	checkConnectState(__FUNCTION__);
	g_controllerServerProxy->SetEnumStringValueAsync(path, enumStringValue, uit::getTickCount(), [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "SetEnumStringValueAsync"); });
}

void Controller::setReal(const std::string &path, double d)
{
	checkConnectState(__FUNCTION__);
	g_controllerServerProxy->SetRealAsync(path, d, uit::getTickCount(), [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "SetRealAsync"); });
}

void Controller::setString(const std::string &path, const std::string &str)
{
	checkConnectState(__FUNCTION__);
	if (str.size() > DATAFW_STRING_SIZE_LIMIT)
		Log::error(DATAFW_CONTROLLER_LOG_TAG, "[%s] set too long string[len=%d] when call [%s] limited by %d bytes.", path.data(), str.size(), __FUNCTION__, DATAFW_STRING_SIZE_LIMIT);
	else
		g_controllerServerProxy->SetStringAsync(path, str, uit::getTickCount(), [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "SetStringAsync"); });
}

void Controller::setImage(const std::string &path, const ImageData &image)
{
	checkConnectState(__FUNCTION__);
	if (image.bytes() > DATAFW_IMAGE_SIZE_LIMIT)
		Log::error(DATAFW_CONTROLLER_LOG_TAG, "[%s] set too big image[len=%d] when call [%s] limited by %d bytes.", path.data(), image.bytes(), __FUNCTION__, DATAFW_IMAGE_SIZE_LIMIT);
	else
		g_controllerServerProxy->SetImageAsync(path, Types::Image("", image.isFormatKnown(), image.format(), image.width(), image.height(), CommonAPI::ByteBuffer(image.getData(), image.getData() + image.bytes())), uit::getTickCount(), [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "SetImageAsync"); });
}

void Controller::setGroup(const std::vector<std::pair<std::string, Poco::Dynamic::Var>> &nodes)
{
	checkConnectState(__FUNCTION__);
	std::vector<std::string> paths;
	Poco::JSON::Array::Ptr values = new Poco::JSON::Array();
	for (auto one : nodes)
	{
		auto path = one.first;
		auto v = one.second;
		if (v.isBoolean() || v.isNumeric() || v.isString())
		{
			paths.push_back(path);
			values->add(v);
		}
		else
		{
			Log::error(DATAFW_CONTROLLER_LOG_TAG, "[%s]'s type is [%s], not a supported type, ignore it when call %s.", path.data(), v.type().name(), __FUNCTION__);
		}
	}
	try {
		g_controllerServerProxy->SetGroupAsync(paths, Poco::Dynamic::Var(values).toString(), uit::getTickCount(), [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "ResizeListAsync"); });
	}
	catch (...)
	{
		Log::error(DATAFW_CONTROLLER_LOG_TAG, "nodes contain uncommon type.");
	}
}

void Controller::resizeList(const std::string &path, uint32_t size)
{
	checkConnectState(__FUNCTION__);
	if (size > DATAFW_LIST_ITEM_COUNT_LIMIT)
		Log::error(DATAFW_CONTROLLER_LOG_TAG, "[%s] set too big item count[size=%d] when call interface [%s] limited by %d .", path.data(), size, __FUNCTION__, DATAFW_LIST_ITEM_COUNT_LIMIT);
	else
		g_controllerServerProxy->ResizeListAsync(path, size, [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "ResizeListAsync"); });
}

void Controller::clearList(const std::string & path)
{
	resizeList(path, 0);
}

void Controller::insertListItems(const std::string &path, uint32_t beg, uint32_t count)
{
	checkConnectState(__FUNCTION__);
	g_controllerServerProxy->InsertListItemsAsync(path, beg, count, [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "InsertListItemsAsync"); });
}

void Controller::appendListItems(const std::string &path, uint32_t count)
{
	insertListItems(path, -1, count);	//传入-1表示append，不可修改
}

void Controller::removeListItems(const std::string &path, uint32_t beg, uint32_t count)
{
	checkConnectState(__FUNCTION__);
	g_controllerServerProxy->RemoveListItemsAsync(path, beg, count, [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "RemoveListItemsAsync"); });
}

void Controller::updateListViewport(const std::string &path, uint32_t first)
{
	checkConnectState(__FUNCTION__);
	g_controllerServerProxy->SetListViewportAsync(path, first, [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "SetListViewportAsync"); });
}

void Controller::checkListItem(const std::string &path, int32_t index)
{
	checkConnectState(__FUNCTION__);
	g_controllerServerProxy->CheckListItemAsync(path, index, [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "CheckListItemAsync"); });
}

void Controller::focusListItem(const std::string &path, int32_t index)
{
	checkConnectState(__FUNCTION__);
	g_controllerServerProxy->FocusListItemAsync(path, index, [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "FocusListItemAsync"); });
}

void Controller::updateListItems(const std::string &path, const std::set<uint32_t> &indexs, Poco::JSON::Array::Ptr items)
{
	checkConnectState(__FUNCTION__);
	if (indexs.empty() || !items || items->size() == 0)	return;

	//检查分类存放
	//item元素类型将被严格检查，除了bool、int、real、string、enum、image，都不被允许
	std::vector<Types::Image> images;
	for (int32_t i = 0; i != items->size(); ++i)
	{
		Poco::JSON::Object::Ptr item = items->getObject(i);
		if (!item)
		{
			Log::error(DATAFW_CONTROLLER_LOG_TAG, "item must be [Poco::JSON::Object::Ptr] when call [%s], return.", __FUNCTION__);
			return;
		}

		std::vector<std::string> names;
		item->getNames(names);
		for (auto const &name : names)
		{
			Poco::Dynamic::Var v = item->get(name);
			if (v.isBoolean() || v.isNumeric() || v.isString())
			{
				continue;
			}
			else if (v.type() == typeid(ImageData))
			{
				const ImageData &imageData = v.extract<ImageData>();
				images.push_back(Types::Image("", imageData.isFormatKnown(), imageData.format(), imageData.width(), imageData.height(), CommonAPI::ByteBuffer(imageData.getData(), imageData.getData() + imageData.bytes())));
				item->set(name, Parser::makeDefault(imageValue));
			}
			else
			{
				Log::error(DATAFW_CONTROLLER_LOG_TAG, "[%s] item element[%d][%s]'s type is [%s], not a surppoted type in item, ignore it when call [%s].", path.data(), i, name.data(), v.type().name(), __FUNCTION__);
				item->remove(name);
			}
		}
	}

	//如果此次更新包含image，空值也需为它设置为空的
	//image保持对齐，以便model方便处理
	if (!images.empty() && images.size() != items->size())
	{
		Log::error(DATAFW_CONTROLLER_LOG_TAG, "[%s] item element contain image, you should fit iamges size[%d] to items size[%d] when call [%s], use default constructor to create a image if it's an empty one.", path.data(), images.size(), items->size(), __FUNCTION__);
		return;
	}
	try { g_controllerServerProxy->UpdateListItemsAsync(path, std::vector<uint32_t>(indexs.begin(), indexs.end()), Poco::Dynamic::Var(items).toString(), images, [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "UpdateListItemsAsync"); }); }
	catch (...) { Log::error(DATAFW_CONTROLLER_LOG_TAG, "items convert to json fail when call [%s], maybe items contain uncommon type element.", __FUNCTION__); }
}

void Controller::vkey(VKeyId key, VKeyState state)
{
	checkConnectState(__FUNCTION__);
	g_controllerServerProxy->VKeyAsync(key, state, [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "VKeyAsync"); });
}

uit::Event<Controller::UserArgs> &Controller::userEvent()
{
	return UserEvent;
}

void Controller::onUserAction(const std::string &sEvenName, const std::string &sArgsJsonString)
{
	Poco::JSON::Parser parser;
	Controller::userEvent().dispatch({ sEvenName, parser.parse(sArgsJsonString).extract<Poco::JSON::Object::Ptr>() });
}

void Controller::checkConnectState(const std::string &funcName)
{
	if (!isConnected())
	{
		Log::error(DATAFW_CONTROLLER_LOG_TAG, "controller[%s] has not connected to model when call [%s].", uit::getProcessName(Poco::Process::id()).data(), funcName.data());
		std::quick_exit(1);
	}
}
