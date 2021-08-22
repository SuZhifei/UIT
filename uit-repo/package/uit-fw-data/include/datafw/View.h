#pragma once
#include <vector>
#include <Poco/JSON/Object.h>
#include "Type.h"
#include "ImageData.h"
#include <uit/core/Event.h>

namespace v1 { namespace uit { namespace datafw{
	template <typename ... _AttributeExtensions>
	class ViewServerProxy;
} } }

namespace uit { namespace datafw {

class UIT_API View final
{
public:
	//构建一个view端
	View();
	~View() = default;
	View(const View &other) = delete;
	void operator = (const View &other) = delete;


	//连接model
	//registerDataRoot：注册的数据，如果为空，表示不关心任何数据，之后的不会有任何数据更新的通知，data可由Parser解析而来
	bool connect(Poco::JSON::Object::Ptr registerDataRoot);

	//发送异步事件
	//sEventName：形如"Bt.ChangeSubModuleEvent"
	//args：参数列表
	void postEvent(const std::string &sEventName, Poco::JSON::Object::Ptr args);

public:
	//同步model事件
	//data：最新的数据，用此来初始化GUI元素
	struct SyncedModelArgs { Poco::JSON::Object::Ptr data; };
	uit::Event<SyncedModelArgs>			SyncedModelEvent;

	//布尔数据更新通知
	//path：数据路径
	//value：布尔值
	//timestamp：时间戳（0表示此参数无意义）
	struct BooleanChangedArgs { std::string path; bool value; uint64_t timestamp; };
	uit::Event<BooleanChangedArgs>		BooleanChangedEvent;

	//整型数据更新通知
	//path：数据路径
	//value：整型值
	//timestamp：时间戳（0表示此参数无意义）
	struct IntegerChangedArgs { std::string path; int32_t value; uint64_t timestamp; };
	uit::Event<IntegerChangedArgs>		IntegerChangedEvent;

	//浮点数据更新通知
	//path：数据路径
	//value：浮点值
	//timestamp：时间戳（0表示此参数无意义）
	struct RealChangedArgs { std::string path; double value; uint64_t timestamp; };
	uit::Event<RealChangedArgs>			RealChangedEvent;

	//字符串数据更新通知
	//path：数据路径
	//value：字符串值
	//timestamp：时间戳（0表示此参数无意义）
	struct StringChangedArgs { std::string path; std::string value; uint64_t timestamp; };
	uit::Event<StringChangedArgs>			StringChangedEvent;

	//枚举数据更新通知
	//path：数据路径
	//intValue：枚举值的整数形式
	//stringValue：枚举值的字符串形式
	//timestamp：时间戳（0表示此参数无意义）
	struct EnumChangedArgs { std::string path; int32_t intValue; std::string stringValue; uint64_t timestamp; };
	uit::Event<EnumChangedArgs>			EnumChangedEvent;

	//图片数据更新事件
	//path：数据路径
	//value：图片
	//timestamp：时间戳（0表示此参数无意义）
	struct ImageChangedArgs { std::string path; ImageData value; uint64_t timestamp; };
	uit::Event<ImageChangedArgs>		ImageChangedEvent;

	//列表重置大小事件
	//path：数据路径
	//size：新的大小
	struct ListResizeArgs { std::string path; uint32_t size; };
	uit::Event<ListResizeArgs>			ListResizeEvent;

	//插入列表项
	//path：数据路径
	//beg：插入起始点
	//count：插入项数
	struct ListInsertItemsArgs { std::string path; uint32_t beg; uint32_t count; };
	uit::Event<ListInsertItemsArgs>		ListInsertItemsEvent;

	//移除列表项
	//path：数据路径
	//beg：移除起始点
	//count：移除项数
	struct ListRemoveItemsArgs { std::string path; uint32_t beg; uint32_t count; };
	uit::Event<ListRemoveItemsArgs>		ListRemoveItemsEvent;

	//更新视口
	//path：数据路径
	//beg：视口起始点 
	struct ListUpdateViewportArgs { std::string path; uint32_t beg; };
	uit::Event<ListUpdateViewportArgs>	ListUpdateViewportEvent;

	//选中标记唯一列表项
	//path：列表路径
	//index：下标，如果为-1表示取消标记
	struct ListCheckItemArgs { std::string path; int32_t index; };
	uit::Event<ListCheckItemArgs>		ListCheckItemEvent;

	//聚焦列表项
	//path：列表路径
	//index：下标，如果为-1表示取消聚焦
	struct ListFocusItemArgs { std::string path; int32_t index; };
	uit::Event<ListFocusItemArgs>		ListFocusItemEvent;

	//列表项更新事件
	//path：列表路径
	//indices：更新的目标下标集合
	//source：更新的源内容
	struct ListItemsUpdatedArgs { std::string path; std::set<uint32_t> indices; Poco::JSON::Array::Ptr source; };
	uit::Event<ListItemsUpdatedArgs>	ListItemsUpdatedEvent;

	//vkey事件
	//key：按键名
	//state：按键状态
	struct VKeyArgs { VKeyId key; VKeyState state; };
	uit::Event<VKeyArgs>				VKeyEvent;

private:
	std::shared_ptr<v1::uit::datafw::ViewServerProxy<>>	m_viewServerProxy;
	bool												m_connected;
};

}}
