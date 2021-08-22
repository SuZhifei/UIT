#pragma once
#include <CommonAPI/CommonAPI.hpp>
#include "datafw/Parser.h"
#include "DataCluster.h"
#include "EventCluster.h"

#include "SocketDebugHelper.h"

namespace uit { namespace datafw {

class Model;
class ViewServerImp;
class ControllerServerImp;
class Model_Internal final
{
public:
	Model_Internal(Model *p);

	bool configDataEventDir(const std::string &sDir);
	std::string getDataEventDir() const;
	void enableSocketDebugHelper(bool bEnable);
	bool isSocketDebugHelperEnable() const;

	bool startup();
	bool stop();

	Parser &parser();
	const Parser &getParser() const;

	//view调用接口
	void registerView(const std::shared_ptr<CommonAPI::ClientId> &view, int32_t pid, const std::string &sDataJsonString);
	void postEvent(const std::string &sEventName, const std::string &sArgJsonString);

	//controller调用接口
	void registerEvents(const std::shared_ptr<CommonAPI::ClientId> &controller, int32_t pid, const std::vector<std::string> &events);
	void setBoolean(const std::string &path, bool b, uint64_t timestamp);
	void setInteger(const std::string &path, int32_t n, uint64_t timestamp);
	void setReal(const std::string &path, double d, uint64_t timestamp);
	void setString(const std::string &path, const std::string &str, uint64_t timestamp);
	void setEnum(const std::string &path, int32_t e, uint64_t timestamp);
	void setEnumStringValue(const std::string &path, const std::string sStringValue, uint64_t timestamp);
	void setImage(const std::string &path, const Types::Image &image, uint64_t timestamp);
	void setGroup(const std::vector<std::string> &paths, const std::string &sItemsJsonString, uint64_t timestamp);

	void updateListItems(const std::string &path, const std::vector<uint32_t> &indexs, const std::string &sItemsJsonString, const std::vector<Types::Image> &images);
	void resizeList(const std::string &path, uint32_t size);
	void insertListItems(const std::string &path, uint32_t index, uint32_t count);
	void removeListItems(const std::string &path, uint32_t index, uint32_t count);
	void updateListViewport(const std::string &path, uint32_t first);
	void checkListItem(const std::string &path, int32_t index);
	void focusListItem(const std::string &path, int32_t index);
	void vkey(int32_t key, int32_t state);

private:
	bool initDataEvent();
	bool initViewServer(uint32_t timeoutMs);
	bool initControllerServer(uint32_t timeoutMs);

	void checkFuncPerformance(uint64_t begTick, const std::string &sFuncName, const char *format, ...);

private:
	bool										m_running;
	Parser										m_Parser;
	DataCluster									m_DataCluster;
	EventCluster								m_EventCluster;
	std::shared_ptr<ViewServerImp>				m_viewServerImp;
	std::shared_ptr<ControllerServerImp>		m_controllerServerImp;
	std::shared_ptr<SocketDebugHelper>			m_socketHelper;
	Model										*m_owner;
};

}}
