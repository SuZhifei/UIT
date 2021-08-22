#pragma once
#include <v1/uit/datafw/ControllerServerStubDefault.hpp>

using namespace v1::uit::datafw;
namespace uit { namespace datafw {

class Model_Internal;
class ControllerServerImp final : public ControllerServerStubDefault
{
public:
	ControllerServerImp(Model_Internal *p);

	//imp方法，如果有多个线程调用，请加锁
	void registerEvent(const std::shared_ptr<CommonAPI::ClientId> &controller, const std::string &sEvent);
	void postEvent(const std::string &sEvent, const std::string &sArgsJsonString);

private:
	//远程调用
	virtual void RegisterEvents(const std::shared_ptr<CommonAPI::ClientId> _client, int32_t _pid, std::vector< std::string > _events, RegisterEventsReply_t _reply) override;
	virtual void SetBoolean(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, bool _value, uint64_t _timestamp, SetBooleanReply_t _reply) override;
	virtual void SetInteger(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, int32_t _value, uint64_t _timestamp, SetIntegerReply_t _reply) override;
	virtual void SetReal(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, double _value, uint64_t _timestamp, SetRealReply_t _reply) override;
	virtual void SetString(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, std::string _value, uint64_t _timestamp, SetStringReply_t _reply) override;
	virtual void SetEnum(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, int32_t _value, uint64_t _timestamp, SetEnumReply_t _reply) override;
	virtual void SetEnumStringValue(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, std::string _value, uint64_t _timestamp, SetEnumStringValueReply_t _reply) override;
	virtual void SetImage(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, Types::Image _image, uint64_t _timestamp, SetImageReply_t _reply) override;
	virtual void SetGroup(const std::shared_ptr<CommonAPI::ClientId> _client, std::vector< std::string > _paths, std::string _json, uint64_t _timestamp, SetGroupReply_t _reply) override;
	virtual void ResizeList(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, uint32_t _size, ResizeListReply_t _reply) override;
	virtual void InsertListItems(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, uint32_t _beg, uint32_t _count, InsertListItemsReply_t _reply) override;
	virtual void RemoveListItems(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, uint32_t _beg, uint32_t _count, RemoveListItemsReply_t _reply) override;
	virtual void SetListViewport(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, uint32_t _first, SetListViewportReply_t _reply) override;
	virtual void UpdateListItems(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, std::vector< uint32_t > _indexs, std::string _json, std::vector< Types::Image > _images, UpdateListItemsReply_t _reply) override;
	virtual void CheckListItem(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, int32_t _index, CheckListItemReply_t _reply) override;
	virtual void FocusListItem(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path, int32_t _index, FocusListItemReply_t _reply) override;
	virtual void VKey(const std::shared_ptr<CommonAPI::ClientId> _client, int32_t _key, int32_t _keystate, VKeyReply_t _reply) override;

	Model_Internal	*m_model;
	std::map<size_t, std::pair<std::shared_ptr<CommonAPI::ClientId>, std::set<std::string>>>	m_controllers;
};

}}
