#include "ControllerServerImp.h"
#include "Model_Internal.h"
#include "Share.h"
#include <Poco/StringTokenizer.h>

using namespace uit::datafw;

ControllerServerImp::ControllerServerImp(Model_Internal * p)
	: m_model(p)
{
}

void ControllerServerImp::registerEvent(const std::shared_ptr<CommonAPI::ClientId> &controller, const std::string &sEvent)
{
	auto iter = m_controllers.find(controller->hashCode());
	if (iter == m_controllers.end())
		m_controllers[controller->hashCode()] = { controller, std::set<std::string>({sEvent}) };
	else
		iter->second.second.insert(sEvent);
}

void ControllerServerImp::postEvent(const std::string &sEvent, const std::string &sArgsJsonString)
{
	auto receivers = std::make_shared<CommonAPI::ClientIdList>();
	for (auto const &iter : m_controllers)
		if (iter.second.second.find(sEvent) != iter.second.second.end())
			receivers->insert(iter.second.first);

	if (receivers->empty())
		Log::warn(DATAFW_MODEL_LOG_TAG, "no controller register event [%s] when view try to post it.", sEvent.data());
	else
		fireUserActionSelective(sEvent, sArgsJsonString, receivers);
}

void ControllerServerImp::RegisterEvents(const std::shared_ptr<CommonAPI::ClientId> _client, int32_t _pid, std::vector<std::string> _events, RegisterEventsReply_t _reply)
{
	Log::info(DATAFW_MODEL_LOG_TAG, "controller(%s-%d)[0x%08lX] register events.", uit::getProcessName(_pid).data(), _pid, _client->hashCode());
	m_model->registerEvents(_client, _pid, _events);
	_reply();
}

void ControllerServerImp::SetBoolean(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, bool _value, uint64_t _timestamp, SetBooleanReply_t _reply)
{
	m_model->setBoolean(_path, _value, _timestamp);
	_reply();
}

void ControllerServerImp::SetInteger(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, int32_t _value, uint64_t _timestamp, SetIntegerReply_t _reply)
{
	m_model->setInteger(_path, _value, _timestamp);
	_reply();
}

void ControllerServerImp::SetReal(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, double _value, uint64_t _timestamp, SetRealReply_t _reply)
{
	m_model->setReal(_path, _value, _timestamp);
	_reply();
}

void ControllerServerImp::SetString(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, std::string _value, uint64_t _timestamp, SetStringReply_t _reply)
{
	m_model->setString(_path, _value, _timestamp);
	_reply();
}

void ControllerServerImp::SetEnum(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, int32_t _value, uint64_t _timestamp, SetEnumReply_t _reply)
{
	m_model->setEnum(_path, _value, _timestamp);
	_reply();
}

void ControllerServerImp::SetEnumStringValue(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, std::string _value, uint64_t _timestamp, SetEnumStringValueReply_t _reply)
{
	m_model->setEnumStringValue(_path, _value, _timestamp);
	_reply();
}

void ControllerServerImp::SetImage(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, Types::Image _image, uint64_t _timestamp, SetImageReply_t _reply)
{
	m_model->setImage(_path, _image, _timestamp);
	_reply();
}

void ControllerServerImp::SetGroup(const std::shared_ptr<CommonAPI::ClientId> _client, std::vector<std::string> _paths, std::string _json, uint64_t _timestamp, SetGroupReply_t _reply)
{
	m_model->setGroup(_paths, _json, _timestamp);
	_reply();
}

void ControllerServerImp::ResizeList(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, uint32_t _size, ResizeListReply_t _reply)
{
	m_model->resizeList(_path, _size);
	_reply();
}

void ControllerServerImp::InsertListItems(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, uint32_t _beg, uint32_t _count, InsertListItemsReply_t _reply)
{
	m_model->insertListItems(_path, _beg, _count);
	_reply();
}

void ControllerServerImp::RemoveListItems(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, uint32_t _beg, uint32_t _count, RemoveListItemsReply_t _reply)
{
	m_model->removeListItems(_path, _beg, _count);
	_reply();
}

void ControllerServerImp::SetListViewport(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, uint32_t _first, SetListViewportReply_t _reply)
{
	m_model->updateListViewport(_path, _first);
	_reply();
}

void ControllerServerImp::UpdateListItems(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, std::vector<uint32_t> _indexs, std::string _json, std::vector<Types::Image> _images, UpdateListItemsReply_t _reply)
{
	m_model->updateListItems(_path, _indexs, _json, _images);
	_reply();
}

void ControllerServerImp::CheckListItem(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, int32_t _index, CheckListItemReply_t _reply)
{
	m_model->checkListItem(_path, _index);
	_reply();
}

void ControllerServerImp::FocusListItem(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, int32_t _index, FocusListItemReply_t _reply)
{
	m_model->focusListItem(_path, _index);
	_reply();
}

void ControllerServerImp::VKey(const std::shared_ptr<CommonAPI::ClientId> _client, int32_t _key, int32_t _keystate, VKeyReply_t _reply)
{
	m_model->vkey(_key, _keystate);
	_reply();
}
