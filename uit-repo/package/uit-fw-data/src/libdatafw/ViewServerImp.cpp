#include "ViewServerImp.h"
#include "Model_Internal.h"
#include "Share.h"

using namespace uit::datafw;

ViewServerImp::ViewServerImp(Model_Internal * model)
	: m_model(model)
{
}

void ViewServerImp::notifySyncData(const std::shared_ptr<CommonAPI::ClientId>& view, int32_t pid, Poco::JSON::Object::Ptr newestData, const std::vector<Types::Image> &images, const std::vector<Types::ListAttachment> &listAttachments)
{
	Log::info(DATAFW_MODEL_LOG_TAG, "view(%s-%d)[0x%08lX] sync data.", uit::getProcessName(pid).data(), pid, view->hashCode());
	m_views[view->hashCode()] = std::make_pair(view, newestData);
	auto receivers = std::make_shared<CommonAPI::ClientIdList>();
	receivers->insert(view);
	fireNotifySyncedModelSelective(Poco::Dynamic::Var(newestData).toString(), images, listAttachments, receivers);
}

void ViewServerImp::notifyBoolean(const std::string &path, bool b, uint64_t timestamp)
{
	fireNotifyBooleanSelective(path, b, timestamp, getReceiver(path));
}

void ViewServerImp::notifyInteger(const std::string &path, int32_t n, uint64_t timestamp)
{
	fireNotifyIntegerSelective(path, n, timestamp, getReceiver(path));
}

void ViewServerImp::notifyReal(const std::string &path, double d, uint64_t timestamp)
{
	fireNotifyRealSelective(path, d, timestamp, getReceiver(path));
}

void ViewServerImp::notifyString(const std::string &path, const std::string &str, uint64_t timestamp)
{
	fireNotifyStringSelective(path, str, timestamp, getReceiver(path));
}

void ViewServerImp::notifyEnum(const std::string &path, int32_t e, const std::string &sEnumStringValue, uint64_t timestamp)
{
	fireNotifyEnumSelective(path, e, sEnumStringValue, timestamp, getReceiver(path));
}

void ViewServerImp::notifyImage(const std::string &path, const Types::Image &image, uint64_t timestamp)
{
	fireNotifyImageSelective(path, image, timestamp, getReceiver(path));
}

void ViewServerImp::notifyGroup(const std::vector<std::string>& paths, Poco::JSON::Array::Ptr values, uint64_t timestamp)
{
	for (auto view : m_views)
	{
		auto receivers = std::make_shared<CommonAPI::ClientIdList>();
		std::vector<std::string> _paths;
		Poco::JSON::Array::Ptr _values = new Poco::JSON::Array();
		for (auto i = 0; i != paths.size(); ++i)
		{
			auto const &path = paths[i];
			if (Parser::find(view.second.second, path) != Poco::Dynamic::Var())
			{
				_paths.push_back(path);
				_values->add(values->get(i));
			}
		}
		if (!_paths.empty())
		{
			receivers->insert(view.second.first);
			fireNotifyGroupSelective(_paths, Poco::Dynamic::Var(_values).toString(), timestamp, receivers);
		}
	}
}

void ViewServerImp::notifyResizeList(const std::string &path, uint32_t size)
{
	fireNotifyResizeListSelective(path, size, getReceiver(path));
}

void ViewServerImp::notifyInsertListItems(const std::string &path, uint32_t index, uint32_t count)
{
	fireNotifyInsertListItemsSelective(path, index, count, getReceiver(path));
}

void ViewServerImp::notifyRemoveListItems(const std::string &path, uint32_t index, uint32_t count)
{
	fireNotifyRemoveListItemsSelective(path, index, count, getReceiver(path));
}

void ViewServerImp::notifyUpdateListItems(const std::string &path, const std::vector<uint32_t> &indexs, const std::string &sItemsJsonString, const std::vector<Types::Image> &images)
{
	fireNotifyUpdateListItemsSelective(path, indexs, sItemsJsonString, images, getReceiver(path));
}

void ViewServerImp::notifyUpdateListViewport(const std::string &path, uint32_t first)
{
	fireNotifyListViewportSelective(path, first, getReceiver(path));
}

void ViewServerImp::notifyCheckListItem(const std::string &path, int32_t index)
{
	fireNotifyCheckListItemSelective(path, index, getReceiver(path));
}

void ViewServerImp::notifyFocusListItem(const std::string &path, int32_t index)
{
	fireNotifyFocusListItemSelective(path, index, getReceiver(path));
}

void ViewServerImp::notifyVKey(int32_t key, int32_t state)
{
	fireNotifyVKeyEvent(key, state);
}

std::shared_ptr<CommonAPI::ClientIdList> ViewServerImp::getReceiver(const std::string &path) const
{
	auto receivers = std::make_shared<CommonAPI::ClientIdList>();
	for (auto view : m_views)
		if (Parser::find(view.second.second, path) != Poco::Dynamic::Var())
			receivers->insert(view.second.first);
	return receivers;
}

void ViewServerImp::Connect(const std::shared_ptr<CommonAPI::ClientId> _client, int32_t _pid, std::string _data_json, ConnectReply_t _reply)
{
	m_model->registerView(_client, _pid, _data_json);
	_reply();
}

void ViewServerImp::PostEvent(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _event, std::string _args, PostEventReply_t _reply)
{
	m_model->postEvent(_event, _args);
	_reply();
}
