#pragma once
#include "Type.h"
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <uit/core/Event.h>
#include "ImageData.h"
#include "Parser.h"

namespace uit { namespace datafw {

class Model_Internal;
class UIT_API Model final
{
public:
	//构建一个数据模型对象
	Model();
	~Model() = default;
	Model(const Model &other) = delete;
	void operator = (const Model &other) = delete;

	//启用socket调试助手（需在startup前调用，默认为不启用）
	//启用后，可通过远程socket进行调试
	void enableSocketDebugHelper(bool bEnable);
	bool isSocketDebugHelperEnable() const;

	//加载目录下的数据、事件
	bool configDataEventDir(const std::string &sDir);
	std::string getDataEventDir() const;

	//启动
	bool startup();
	bool stop();

	//获取解析器
	Parser &parser();
	const Parser &getParser() const;

	///////////////////////////
	bool postEvent(const std::string &sEventName, Poco::JSON::Object::Ptr args);

	void setBoolean(const std::string &path, bool b);
	void setInteger(const std::string &path, int32_t n);
	void setReal(const std::string &path, double d);
	void setString(const std::string &path, const std::string &str);
	void setEnum(const std::string &path, int32_t e);
	void setEnumStringValue(const std::string &path, const std::string sStringValue);
	bool setImage(const std::string &path, const ImageData &image);

	void updateListItems(const std::string &path, const std::set<uint32_t> &indexs, Poco::JSON::Array::Ptr items);
	void resizeList(const std::string &path, uint32_t size);
	void insertListItems(const std::string &path, uint32_t index, uint32_t count);
	void removeListItems(const std::string &path, uint32_t index, uint32_t count);
	void updateListViewport(const std::string &path, uint32_t first);
	void checkListItem(const std::string &path, int32_t index);
	void focusListItem(const std::string &path, int32_t index);

	void vkey(int32_t key, int32_t state);

public:
	//view接入事件
	struct ViewConnectedArgs { int pid; std::string name; };
	uit::Event<ViewConnectedArgs>			ViewConnectedEvent;

	//conctroller接入事件
	struct ControllerConnectedArgs { int pid; std::string name; };
	uit::Event<ControllerConnectedArgs>		ControllerConnectedEvent;

	//用户动作事件
	struct UserActionArgs { std::string sEvent; std::string argsJson; };
	uit::Event<UserActionArgs>				UserActionEvent;

private:
	std::shared_ptr<Model_Internal>		m_internal;
};

}}
