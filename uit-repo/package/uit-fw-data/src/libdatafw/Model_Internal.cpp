#include "Model_Internal.h"
#include <Poco/JSON/Parser.h>
#include <uit/core/Log.h>
#include "ViewServerImp.h"
#include "ControllerServerImp.h"
#include "Share.h"
#include "datafw/Model.h"
#include <stdarg.h>
#include <stdlib.h>

using namespace uit::datafw;

Model_Internal::Model_Internal(Model *p)
	: m_owner(p)
	, m_running(false)
{
}

bool Model_Internal::configDataEventDir(const std::string &sDir)
{
	m_Parser.configureLoadingDir(sDir);
	return initDataEvent();
}

std::string Model_Internal::getDataEventDir() const
{
	return m_Parser.getConfiureLoadingDir();
}

void Model_Internal::enableSocketDebugHelper(bool bEnable)
{
	m_socketHelper = bEnable ? std::make_shared<SocketDebugHelper>(this) : nullptr;
}

bool Model_Internal::isSocketDebugHelperEnable() const
{
	return m_socketHelper != nullptr;
}

bool Model_Internal::startup()
{
	initEnv();
	Log::info(DATAFW_MODEL_LOG_TAG, "COMMONAPI_CONFIG=%s", getenv("COMMONAPI_CONFIG"));
	Log::info(DATAFW_MODEL_LOG_TAG, "VSOMEIP_CONFIGURATION=%s", getenv("VSOMEIP_CONFIGURATION"));
	bool bSuccess = initControllerServer(DATAFW_SERVER_INIT_TIMEOUT);
	bSuccess &= initViewServer(DATAFW_SERVER_INIT_TIMEOUT);
	if (m_socketHelper)
		m_socketHelper->startup();

	m_running = bSuccess;
	return m_running;
}

bool Model_Internal::stop()
{
	m_running = false;
	if (m_socketHelper)
		m_socketHelper->stop();
	return true;
}

Parser &Model_Internal::parser()
{
	return m_Parser;
}

const Parser &Model_Internal::getParser() const
{
	return m_Parser;
}

void Model_Internal::registerView(const std::shared_ptr<CommonAPI::ClientId> &view, int32_t pid, const std::string &sDataJsonString)
{
	uint64_t k = uit::getTickCount();
	Poco::JSON::Object::Ptr toBeSyncData;
	Poco::JSON::Parser parser;
	try {
		Poco::Dynamic::Var v = parser.parse(sDataJsonString);
		toBeSyncData = v.extract<Poco::JSON::Object::Ptr>();
	}
	catch (...) { 
		Log::error(DATAFW_MODEL_LOG_TAG, "parse error when registData:\r\n%s, view attention none data", sDataJsonString.data()); 
		return;
	}
	std::vector<Types::Image> images;
	std::vector<Types::ListAttachment> listattachments;
	m_DataCluster.syncTo(toBeSyncData, images, listattachments);
	m_viewServerImp->notifySyncData(view, pid, toBeSyncData, images, listattachments);

	checkFuncPerformance(k, __FUNCTION__, "view: [0x%08lX]", view->hashCode());
	m_owner->ViewConnectedEvent.dispatch({ pid, uit::getProcessName(pid) });
}

void Model_Internal::postEvent(const std::string &sEventName, const std::string &sArgJsonString)
{
	uint64_t k = uit::getTickCount();

	Poco::JSON::Parser parser;
	Poco::JSON::Object::Ptr args;
	try {
		args = parser.parse(sArgJsonString).extract<Poco::JSON::Object::Ptr>();
	}
	catch (...) {
		Log::error(DATAFW_MODEL_LOG_TAG, "parse error when postEvent:\r\n%s", sArgJsonString.data());
		return;
	}

	if(m_EventCluster.verify(sEventName, args))
		m_controllerServerImp->postEvent(sEventName, Poco::Dynamic::Var(args).toString());
	else
		Log::error(DATAFW_MODEL_LOG_TAG, "there is no event named [%s] when view want to post it", sEventName.data());

	checkFuncPerformance(k, __FUNCTION__, "sEventName: %s, sArgJsonString: \r\n%s", sEventName.data(), sArgJsonString.data());

	m_owner->UserActionEvent.dispatch({ sEventName, Poco::Dynamic::Var(args).toString() });
}

void Model_Internal::registerEvents(const std::shared_ptr<CommonAPI::ClientId> &controller, int32_t pid, const std::vector<std::string> &events)
{
	uint64_t k = uit::getTickCount();

	for(auto const &e : events)
	if (m_Parser.findEvent(e))
		m_controllerServerImp->registerEvent(controller, e);
	else
		Log::error(DATAFW_MODEL_LOG_TAG, "this no event name [%s] when call [%s].", e.data(), __FUNCTION__);

	checkFuncPerformance(k, __FUNCTION__, "controller: [0x%08lX], sEventName: [%s]", controller->hashCode(), Poco::cat(std::string("|"), events.begin(), events.end()).data());
	m_owner->ControllerConnectedEvent.dispatch({ pid, uit::getProcessName(pid) });
}

void Model_Internal::setBoolean(const std::string &path, bool b, uint64_t timestamp)
{
	uint64_t k = uit::getTickCount();

	if (m_DataCluster.updateBoolean(path, b))
		m_viewServerImp->notifyBoolean(path, b, timestamp);

	checkFuncPerformance(k, __FUNCTION__, "path: %s, b: %d, timestamp: %lld", path.data(), b, timestamp);
}

void Model_Internal::setInteger(const std::string &path, int32_t n, uint64_t timestamp)
{
	uint64_t k = uit::getTickCount();

	if (m_DataCluster.updateInteger(path, n))
		m_viewServerImp->notifyInteger(path, n, timestamp);

	checkFuncPerformance(k, __FUNCTION__, "path: %s, n: %d, timestamp: %lld", path.data(), n, timestamp);
}

void Model_Internal::setReal(const std::string &path, double d, uint64_t timestamp)
{
	uint64_t k = uit::getTickCount();

	if (m_DataCluster.updateReal(path, d))
		m_viewServerImp->notifyReal(path, d, timestamp);

	checkFuncPerformance(k, __FUNCTION__, "path: %s, d: %.2f, timestamp: %lld", path.data(), d, timestamp);
}

void Model_Internal::setString(const std::string &path, const std::string &str, uint64_t timestamp)
{
	uint64_t k = uit::getTickCount();

	if (m_DataCluster.updateString(path, str))
		m_viewServerImp->notifyString(path, str, timestamp);

	checkFuncPerformance(k, __FUNCTION__, "path: %s, str: %s, timestamp: %lld", path.data(), str.data(), timestamp);
}

void Model_Internal::setEnum(const std::string &path, int32_t e, uint64_t timestamp)
{
	uint64_t k = uit::getTickCount();

	if (m_DataCluster.updateEnum(path, e))
		m_viewServerImp->notifyEnum(path, e, m_Parser.extractEnumStringValue(m_Parser.findData(path)), timestamp);

	checkFuncPerformance(k, __FUNCTION__, "path: %s, e: %d, timestamp: %lld", path.data(), e, timestamp);
}

void Model_Internal::setEnumStringValue(const std::string &path, const std::string sStringValue, uint64_t timestamp)
{
	uint64_t k = uit::getTickCount();

	if (m_DataCluster.updateEnumStringValue(path, sStringValue))
		m_viewServerImp->notifyEnum(path, m_Parser.extractEnumInteger(m_Parser.findData(path)), sStringValue, timestamp);

	checkFuncPerformance(k, __FUNCTION__, "path: %s, sStringValue: %s, timestamp: %lld", path.data(), sStringValue.data(), timestamp);
}

void Model_Internal::setImage(const std::string &path, const Types::Image &image, uint64_t timestamp)
{
	uint64_t k = uit::getTickCount();

	if (m_DataCluster.updateImage(path, image))
		m_viewServerImp->notifyImage(path, image, timestamp);

	checkFuncPerformance(k, __FUNCTION__, "path: %s, image.size: %d, timestamp: %lld", path.data(), image.getBuffer().size(), timestamp);
}

void Model_Internal::setGroup(const std::vector<std::string> &paths, const std::string &json, uint64_t timestamp)
{
	uint64_t k = uit::getTickCount();
	Poco::JSON::Array::Ptr values;
	Poco::JSON::Parser parser;
	try {
		Poco::Dynamic::Var v = parser.parse(json);
		values = v.extract<Poco::JSON::Array::Ptr>();
	}
	catch (...) {
		Log::error(DATAFW_MODEL_LOG_TAG, "parse error when registData:\r\n%s, view attention none data", json.data());
		return;
	}
	std::vector<std::string> paths_out;
	Poco::JSON::Array::Ptr values_out = new Poco::JSON::Array();
	if (m_DataCluster.updateGroup(paths, values, paths_out, values_out))
		m_viewServerImp->notifyGroup(paths_out, values_out, timestamp);
	checkFuncPerformance(k, __FUNCTION__, "timestamp: %lld", timestamp);
}

void Model_Internal::updateListItems(const std::string &path, const std::vector<uint32_t> &indexs, const std::string &sItemsJsonString, const std::vector<Types::Image> &images)
{
	uint64_t k = uit::getTickCount();

	std::vector<uint32_t> indexs_out;
	std::string sItemsJsonString_out;
	std::vector<Types::Image> images_out;
	if (m_DataCluster.updateListItems(path, indexs, sItemsJsonString, images, indexs_out, sItemsJsonString_out, images_out))
		m_viewServerImp->notifyUpdateListItems(path, indexs_out, sItemsJsonString_out, images_out);

	checkFuncPerformance(k, __FUNCTION__, "path: %s, indexs_count: %d, images_cout: %d, sItemsJsonString_out:%s", path.data(), indexs_out.size(), images_out.size(), sItemsJsonString_out.data());
}

void Model_Internal::resizeList(const std::string &path, uint32_t size)
{
	uint64_t k = uit::getTickCount();

	if (m_DataCluster.resizeList(path, size))
		m_viewServerImp->notifyResizeList(path, size);

	checkFuncPerformance(k, __FUNCTION__, "path: %s, size: %d", path.data(), size);
}

void Model_Internal::insertListItems(const std::string &path, uint32_t index, uint32_t count)
{
	uint64_t k = uit::getTickCount();

	if (m_DataCluster.insertListItems(path, index, count))
		m_viewServerImp->notifyInsertListItems(path, index, count);

	checkFuncPerformance(k, __FUNCTION__, "path: %s, index: %d, count: %d", path.data(), index, count);
}

void Model_Internal::removeListItems(const std::string &path, uint32_t index, uint32_t count)
{
	uint64_t k = uit::getTickCount();

	if (m_DataCluster.removeListItems(path, index, count))
		m_viewServerImp->notifyRemoveListItems(path, index, count);

	checkFuncPerformance(k, __FUNCTION__, "path: %s, index: %d, count: %d", path.data(), index, count);
}

void Model_Internal::updateListViewport(const std::string &path, uint32_t first)
{
	uint64_t k = uit::getTickCount();

	if (m_DataCluster.updateListViewport(path, first))
		m_viewServerImp->notifyUpdateListViewport(path, first);

	checkFuncPerformance(k, __FUNCTION__, "path: %s, first: %d", path.data(), first);
}

void Model_Internal::checkListItem(const std::string &path, int32_t index)
{
	uint64_t k = uit::getTickCount();

	if (m_DataCluster.checkListItem(path, index))
		m_viewServerImp->notifyCheckListItem(path, index);

	checkFuncPerformance(k, __FUNCTION__, "path: %s, index: %d", path.data(), index);
}

void Model_Internal::focusListItem(const std::string &path, int32_t index)
{
	uint64_t k = uit::getTickCount();

	if (m_DataCluster.focusListItem(path, index))
		m_viewServerImp->notifyFocusListItem(path, index);

	checkFuncPerformance(k, __FUNCTION__, "path: %s, index: %d", path.data(), index);
}

void Model_Internal::vkey(int32_t key, int32_t state)
{
	uint64_t k = uit::getTickCount();

	m_viewServerImp->notifyVKey(key, state);

	checkFuncPerformance(k, __FUNCTION__, "key: %s, state: %s", toString((VKeyId)key).data(), toString((VKeyState)state).data());
}

bool Model_Internal::initDataEvent()
{
	bool b = m_Parser.parse();
	m_DataCluster.setParser(&m_Parser);
	m_EventCluster.setParser(&m_Parser);
	return b;
}

bool Model_Internal::initViewServer(uint32_t timeoutMs)
{
	//把一些初始化的动作放在register之前，可以保证服务注册后这些已经初始化，客服端随时调用无漏洞（之前有个漏洞是，viewServerImp的m_model后设置导致可能调用时为空的情况。
	m_viewServerImp = std::make_shared<ViewServerImp>(this);
	bool bSuccess = registerServer<ViewServerImp>(m_viewServerImp, UIT_SERVER_DOMAIN, UIT_VIEW_SERVER_INSTANCE, UIT_DATAFW_LIBBASE, timeoutMs);
	if (!bSuccess)
		Log::error(DATAFW_MODEL_LOG_TAG, "register view server timeout for [%d] ms, maybe stub dll(so) not found. check env COMMONAPI_CONFIG.", timeoutMs);

	return bSuccess;
}

bool Model_Internal::initControllerServer(uint32_t timeoutMs)
{
	//把一些初始化的动作放在register之前，可以保证服务注册后这些已经初始化，客服端随时调用无漏洞（之前有个漏洞是，viewServerImp的m_model后设置导致可能调用时为空的情况。
	m_controllerServerImp = std::make_shared<ControllerServerImp>(this);
	bool bSuccess = registerServer<ControllerServerImp>(m_controllerServerImp, UIT_SERVER_DOMAIN, UIT_CONTROLLER_SERVER_INSTANCE, UIT_DATAFW_LIBBASE, timeoutMs);
	if (!bSuccess)
		Log::error(DATAFW_MODEL_LOG_TAG, "register controller server timeout for [%d] ms, maybe stub dll(so) not found. check env COMMONAPI_CONFIG.", timeoutMs);

	return bSuccess;
}

void Model_Internal::checkFuncPerformance(uint64_t begTick, const std::string &sFuncName, const char *format, ...)
{
	auto cost = uit::getTickCount() - begTick;
	if (cost > 100)
	{
		char head[256] = { 0 };
		snprintf(head, sizeof(head), "model func[%s] cost too much time for %d ms: ", sFuncName.data(), (int)cost);

		char arr[2048] = { 0 };
		va_list args;
		va_start(args, format);
		vsnprintf(arr, sizeof(arr), format, args);
		va_end(args);

		std::string sAll = std::string(head) + arr;

		Log::warn(DATAFW_MODEL_LOG_TAG, "%s", sAll.data());
	}
}
