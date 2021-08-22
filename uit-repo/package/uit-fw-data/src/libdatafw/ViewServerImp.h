#pragma once
#include <v1/uit/datafw/ViewServerStubDefault.hpp>
#include <Poco/JSON/Object.h>

using namespace v1::uit::datafw;
namespace uit { namespace datafw {

class Model_Internal;
class ViewServerImp final : public ViewServerStubDefault
{
public:
	ViewServerImp(Model_Internal *model);

	//imp方法，如果有多个线程调用，请加锁
	void notifySyncData(const std::shared_ptr<CommonAPI::ClientId>& view, int32_t pid, Poco::JSON::Object::Ptr newestData, const std::vector< Types::Image > &images, const std::vector<Types::ListAttachment> &listAttachments);

	void notifyBoolean(const std::string &path, bool b, uint64_t timestamp);
	void notifyInteger(const std::string &path, int32_t n, uint64_t timestamp);
	void notifyReal(const std::string &path, double d, uint64_t timestamp);
	void notifyString(const std::string &path, const std::string &str, uint64_t timestamp);
	void notifyEnum(const std::string &path, int32_t e, const std::string &sEnumStringValue, uint64_t timestamp);
	void notifyImage(const std::string &path, const Types::Image &image, uint64_t timestamp);
	void notifyGroup(const std::vector<std::string> &paths, Poco::JSON::Array::Ptr values, uint64_t timestamp);

	void notifyResizeList(const std::string &path, uint32_t size);
	void notifyInsertListItems(const std::string &path, uint32_t index, uint32_t count);
	void notifyRemoveListItems(const std::string &path, uint32_t index, uint32_t count);
	void notifyUpdateListItems(const std::string &path, const std::vector<uint32_t> &indexs, const std::string &itemsJsonString, const std::vector< Types::Image > &images);
	void notifyUpdateListViewport(const std::string &path, uint32_t first);
	void notifyCheckListItem(const std::string &path, int32_t index);
	void notifyFocusListItem(const std::string &path, int32_t index);

	void notifyVKey(int32_t key, int32_t state);

private:
	//远程调用
	virtual void Connect(const std::shared_ptr<CommonAPI::ClientId> _client, int32_t _pid, std::string _data_json, ConnectReply_t _reply) override;
	virtual void PostEvent(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _event, std::string _args, PostEventReply_t _reply) override;

private:
	std::shared_ptr<CommonAPI::ClientIdList> getReceiver(const std::string &path) const;

	Model_Internal	*m_model;
	std::map<size_t, std::pair<std::shared_ptr<CommonAPI::ClientId>, Poco::JSON::Object::Ptr>>		m_views;
};

}}