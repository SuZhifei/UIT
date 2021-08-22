#include "datafw/View.h"
#include <CommonAPI/CommonAPI.hpp>
#include <v1/uit/datafw/ViewServerProxy.hpp>
#include <Poco/JSON/Parser.h>
#include <Poco/Process.h>
#include <uit/core/Log.h>
#include "datafw/ImageData.h"
#include "datafw/View.h"
#include "datafw/Type.h"
#include "datafw/Parser.h"
#include "Share.h"

using namespace uit::datafw;
using namespace v1::uit::datafw;

View::View()
	: m_connected(false)
{
	initEnv();
	Log::info(DATAFW_VIEW_LOG_TAG, "COMMONAPI_CONFIG=%s", getenv("COMMONAPI_CONFIG"));
	Log::info(DATAFW_VIEW_LOG_TAG, "VSOMEIP_CONFIGURATION=%s", getenv("VSOMEIP_CONFIGURATION"));
}

bool View::connect(Poco::JSON::Object::Ptr registerDataRoot)
{
	uint64_t k = uit::getTickCount();
	if (m_connected)
		return true;

	bool bTimeout = false;
	m_viewServerProxy = buildProxy<ViewServerProxy>(UIT_SERVER_DOMAIN, UIT_VIEW_SERVER_INSTANCE, UIT_DATAFW_LIBBASE, DATAFW_VIEW_CONNECT_TIMEOUT, bTimeout);
	int pid = Poco::Process::id();
	if (m_viewServerProxy)
	{
		m_viewServerProxy->getNotifySyncedModelSelectiveEvent().subscribe([=](const std::string &sDataJsonString, const std::vector<Types::Image> &images, const std::vector<Types::ListAttachment> &listattachments)
		{ 
			Poco::JSON::Parser parser;
			Poco::Dynamic::Var v = parser.parse(sDataJsonString);
			Poco::JSON::Object::Ptr root = v.extract<Poco::JSON::Object::Ptr>();
			//更新图片节点数据
			for (int32_t i = 0; i != images.size(); ++i)
			{
				const Types::Image &image = images[i];
				Poco::JSON::Array::Ptr vImg = Parser::find(root, image.getPath()).extract<Poco::JSON::Array::Ptr>();
				vImg->set(1, image);
			}
			//更新列表中的图片
			for (uint32_t i = 0; i != listattachments.size(); ++i)
			{
				const std::string &listPath = listattachments[i].getListpath();
				const std::vector<Types::Image> &listimages = listattachments[i].getImages();
				int imageIndex = 0;
				Poco::JSON::Array::Ptr listNode = Parser::find(root, listPath).extract<Poco::JSON::Array::Ptr>();

				for (int32_t j = 1; j != listNode->size(); ++j)
				{
					Poco::JSON::Object::Ptr item = listNode->getObject(j);
					for (Poco::JSON::Object::ConstIterator iter = item->begin(); iter != item->end(); ++iter)
					{
						const std::string &sElementName = iter->first;
						const Poco::Dynamic::Var &v = iter->second;
						if (Parser::isImage(v))
						{
							Poco::JSON::Array::Ptr arr = v.extract<Poco::JSON::Array::Ptr>();
							arr->set(1, listimages[imageIndex++]);
						}
					}
				}
			}
			SyncedModelEvent.dispatch({ root });
		});
		m_viewServerProxy->getNotifyUpdateListItemsSelectiveEvent().subscribe([&](const std::string &path, const std::vector< uint32_t > &indexs, const std::string &sItemsJson, const std::vector<Types::Image> &images) 
		{ 
			Poco::JSON::Parser parser;
			Poco::Dynamic::Var v = parser.parse(sItemsJson);
			Poco::JSON::Array::Ptr array = v.extract<Poco::JSON::Array::Ptr>();
			for (int32_t i = 0; i != array->size(); ++i)
			{
				Poco::JSON::Object::Ptr item = array->getObject(i);
				for (Poco::JSON::Object::ConstIterator iter = item->begin(); iter != item->end(); ++iter)
				{
					Poco::Dynamic::Var v = iter->second;
					if (Parser::isImage(v))
					{
						item->getArray(iter->first)->set(1, images[i]);
					}
				}
			}
			ListItemsUpdatedEvent.dispatch({ path, std::set<uint32_t>(indexs.begin(), indexs.end()), array });
		});
		m_viewServerProxy->getNotifyGroupSelectiveEvent().subscribe([&](const std::vector<std::string> &paths, const std::string &json, uint64_t timestamp)
		{
			auto getType = [](const Poco::Dynamic::Var &v)->ValueType {
				return v.isBoolean() ? booleanValue : v.isString() ? stringValue : v.isNumeric() ? (v.isInteger() ? intValue : realValue) : enumValue;
			};
			Poco::JSON::Parser parser;
			Poco::JSON::Array::Ptr values = parser.parse(json).extract<Poco::JSON::Array::Ptr>();
			for (int32_t i = 0; i != values->size(); ++i)
			{
				auto v = values->get(i);
				auto t = getType(v);
				switch (t)
				{
				case booleanValue:	BooleanChangedEvent.dispatch({ paths[i], v.convert<bool>(), timestamp });		break;
				case intValue:		IntegerChangedEvent.dispatch({ paths[i], v.convert<int>(), timestamp });		break;
				case realValue:		RealChangedEvent.dispatch({ paths[i], v.convert<float>(), timestamp });			break;
				case stringValue:	StringChangedEvent.dispatch({ paths[i], v.convert<std::string>(), timestamp });	break;
				case enumValue:		EnumChangedEvent.dispatch({ paths[i], v.extract<Poco::JSON::Array::Ptr>()->get(1).convert<int>(), "", timestamp });		break;	//无法得知stringValue值
				default:																							break;
				}
			}
		});
		m_viewServerProxy->getNotifyBooleanSelectiveEvent().subscribe([&](const std::string &path, bool b, uint64_t timestamp) { BooleanChangedEvent.dispatch({ path , b, timestamp }); });
		m_viewServerProxy->getNotifyIntegerSelectiveEvent().subscribe([&](const std::string &path, int32_t n, uint64_t timestamp) { IntegerChangedEvent.dispatch({ path, n, timestamp }); });
		m_viewServerProxy->getNotifyRealSelectiveEvent().subscribe([&](const std::string &path, double d, uint64_t timestamp) { RealChangedEvent.dispatch({ path, d, timestamp }); });
		m_viewServerProxy->getNotifyStringSelectiveEvent().subscribe([&](const std::string &path, const std::string &str, uint64_t timestamp) { StringChangedEvent.dispatch({ path, str, timestamp }); });
		m_viewServerProxy->getNotifyEnumSelectiveEvent().subscribe([&](const std::string &path, int32_t e, const std::string &sEnumStringValue, uint64_t timestamp) { EnumChangedEvent.dispatch({ path, e, sEnumStringValue, timestamp }); });
		m_viewServerProxy->getNotifyImageSelectiveEvent().subscribe([&](const std::string &path, const Types::Image &image, uint64_t timestamp) { ImageChangedEvent.dispatch({ path, image.getFormatknown() ? ImageData(image.getBuffer().data(), image.getWidth(), image.getHeight(), (ImageFormat)image.getFormat()) : ImageData(image.getBuffer().data(), image.getBuffer().size()), timestamp }); });
		m_viewServerProxy->getNotifyResizeListSelectiveEvent().subscribe([&](const std::string &path, uint32_t size) { ListResizeEvent.dispatch({ path, size }); });
		m_viewServerProxy->getNotifyInsertListItemsSelectiveEvent().subscribe([&](const std::string &path, uint32_t beg, uint32_t count) { ListInsertItemsEvent.dispatch({ path, beg, count }); });
		m_viewServerProxy->getNotifyRemoveListItemsSelectiveEvent().subscribe([&](const std::string &path, uint32_t beg, uint32_t count) { ListRemoveItemsEvent.dispatch({ path, beg, count }); });
		m_viewServerProxy->getNotifyListViewportSelectiveEvent().subscribe([&](const std::string &path, uint32_t first) { ListUpdateViewportEvent.dispatch({ path, first }); });
		m_viewServerProxy->getNotifyCheckListItemSelectiveEvent().subscribe([&](const std::string &path, int32_t index) { ListCheckItemEvent.dispatch({ path, index }); });
		m_viewServerProxy->getNotifyFocusListItemSelectiveEvent().subscribe([&](const std::string &path, int32_t index) { ListFocusItemEvent.dispatch({ path, index }); });
		m_viewServerProxy->getNotifyVKeyEvent().subscribe([&](int32_t key, int32_t state) { VKeyEvent.dispatch({ (VKeyId)key, (VKeyState)state }); });
		try {
			m_viewServerProxy->ConnectAsync(pid, registerDataRoot ? Poco::Dynamic::Var(registerDataRoot).toString() : "", [](const CommonAPI::CallStatus &callStatus) {onCallErrorCallback(callStatus, "RegisterDataAsync"); });
		}
		catch (...) {
			m_connected = false;
			Log::info(DATAFW_VIEW_LOG_TAG, "view[%s] connect fail, registerDataRoot convert to json fail when call [%s], maybe args contain uncommon type arg.", uit::getProcessName(pid).data(), __FUNCTION__);
		}

		m_connected = true;
		Log::info(DATAFW_VIEW_LOG_TAG, "view[%s] connect success, cost [%lld] ms.", uit::getProcessName(pid).data(), uit::getTickCount() - k);
	}
	else
	{
		m_connected = false;
		bTimeout ? Log::error(DATAFW_VIEW_LOG_TAG, "view[%s] connect fail timeout for [%d] ms.", uit::getProcessName(pid).data(), DATAFW_VIEW_CONNECT_TIMEOUT) : Log::error(DATAFW_VIEW_LOG_TAG, "view[%s] connect fail for null proxy.", uit::getProcessName(pid).data());
	}
	return m_connected;
}

void View::postEvent(const std::string &sEventName, Poco::JSON::Object::Ptr args)
{
	if (!m_connected)
	{
		Log::error(DATAFW_VIEW_LOG_TAG, "view[%s] has not connected to model when call [%s].", uit::getProcessName(Poco::Process::id()).data(), __FUNCTION__);
		std::quick_exit(1);
	}
	else
	{
		try {
			std::string sJson = Poco::Dynamic::Var(args).toString();
			m_viewServerProxy->PostEventAsync(sEventName, sJson, [](const CommonAPI::CallStatus &callStatus) { onCallErrorCallback(callStatus, "PostEventAsync"); });
		}
		catch (...) {
			Log::error(DATAFW_VIEW_LOG_TAG, "args convert to json fail when call [%s], maybe args contain uncommon type arg.", __FUNCTION__);
		}
	}
}
