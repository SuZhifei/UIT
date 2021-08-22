/******************************************************************************

* Copyright (c) 2016，德赛西威汽车电子有限公司

* All rights reserved

* 编写者: 潘荣涛

* 编写日期: 2017-11

* 简要描述: Controller是数据驱动模型的“控制者”身份，它将是数据驱使的直接触发者。
	当需要根据底层状态的变化来驱使GUI参数变化时，则调用相关函数Setxxx来设置数据；
	或者xxxListxxx来更新列表数据。
	Controller拥有以下特性：
	1、Controller的函数都是请求异步执行，且线程安全，这意味着你可以在任何线程执行它的函数调用
	2、Controller的事件也是异步的
	3、所有的数据、事件皆由model加载的json文件提供

* 修改记录:

*****************************************************************************/

#pragma once
#include <string>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include "Type.h"
#include "ImageData.h"
#include "uit/core/Event.h"

namespace uit { namespace datafw {

class UIT_API Controller final
{
public:
	//删除全部的默认函数
	Controller() = delete;
	~Controller() = delete;
	Controller(const Controller &other) = delete;
	void operator = (const Controller &other) = delete;

	//连接model
	//timeoutMs：超时毫秒
	static bool connect(uint32_t timeoutMs);

	//是否连接了model
	static bool isConnected();

	//断开连接
	static bool disconnect();

	//注册多个事件
	//形如"Camera.ChangeBrightnessEvent"，如果不存在，model将报告并忽略此次请求
	static void registerEvents(const std::vector<std::string> &events);

	//设置布尔值
	//path：数据节点路径，如果不存在或者该路径的数据不是布尔类型，model将报告并忽略此次请求
	//b：布尔值
	static void setBoolean(const std::string &path, bool b);

	//设置整数值
	//path：数据节点路径，如果不存在或者该路径的数据不是整数类型，model将报告并忽略此次请求
	//n：整数值
	static void setInteger(const std::string &path, int32_t n);

	//设置浮点值
	//path：数据节点路径，如果不存在或者该路径的数据不是浮点类型，model将报告并忽略此次请求
	//d：浮点值
	static void setReal(const std::string &path, double d);

	//设置字符串值
	//path：数据节点路径，如果不存在或者该路径的数据不是字符串类型，model将报告并忽略此次请求
	//str：字符串值，如果str长度超过2mb，将发生越界错误并忽略此次请求
	static void setString(const std::string &path, const std::string &str);

	//设置枚举值（整数形式）
	//path：数据节点路径，如果不存在或者该路径的数据不是枚举类型，model将报告并忽略此次请求
	//e：枚举值的整数形式，如果e不是该枚举类型的值范围，model将报告并忽略此次请求
	static void setEnum(const std::string &path, int32_t e);

	//设置枚举值（字符串形式）
	//path：数据节点路径，如果不存在或者该路径的数据不是枚举类型，model将报告并忽略此次请求
	//enumStringValue：枚举值的字符串形式，如果enumStringValue不是该枚举类型的字符串值中的一个，model将报告并忽略此次请求
	static void setEnum(const std::string &path, const std::string &enumStringValue);

	//设置图片数据
	//path：数据节点路径，如果不存在或者该路径的数据不是图片类型，model将报告并忽略此次请求
	//image：图片数据，如果图片数据超过5mb，将发生越界错误并忽略此次请求
	static void setImage(const std::string &path, const ImageData &image);

	//设置一组数据
	//注意：子节点仅支持普通类型如：bool，int，double，string，Enum(int)，否则该节点被忽略
	static void setGroup(const std::vector<std::pair<std::string, Poco::Dynamic::Var>> &nodes);

	//列表相关
	//重置列表大小
	//path：数据节点路径，如果不存在或者该路径的数据不是列表类型，model将报告并忽略此次请求
	//size：新大小。如果size小于原来的大小，则截断尾部items；否则在尾部追加项数差值；如果插入后列表项大于10w model将报告并忽略此次请求
	static void resizeList(const std::string &path, uint32_t size);

	//清空列表
	//path：数据节点路径，如果不存在或者该路径的数据不是列表类型，model将报告并忽略此次请求
	//等效于resizeList(0)
	static void clearList(const std::string &path);

	//插入列表项
	//path：数据节点路径，如果不存在或者该路径的数据不是列表类型，model将报告并忽略此次请求
	//beg：插入的起始位置，如果beg大于等于当前item数量，将在尾部插入
	//count：插入数量；如果插入后列表项大于10w model将报告并忽略此次请求
	static void insertListItems(const std::string &path, uint32_t beg, uint32_t count);

	//追加列表项
	//path：数据节点路径，如果不存在或者该路径的数据不是列表类型，model将报告并忽略此次请求
	//count：追加数量；如果插入后列表项大于10w model会忽略此次请求
	//等效于insertListItems(path, -1, count)
	static void appendListItems(const std::string &path, uint32_t count);

	//移除列表项
	//path：数据节点路径，如果不存在或者该路径的数据不是列表类型，model将报告并忽略此次请求
	//beg：移除的起始位置，如果beg大于等于当前item数量，将不做移除动作
	//count：移除数量
	static void removeListItems(const std::string &path, uint32_t beg, uint32_t count);

	//更新列表数据
	//path：数据节点路径，如果不存在或者该路径的数据不是列表类型，model将报告并忽略此次请求
	//indexs：下标，如 std::set<uint32_t> = {1, 3, 5, 6}，如果indexs中的下标值越界列表，此下标将被忽略更新
	//items：更新的内容
	//说明：如果indexs个数大于items个数，则实际更新的item数为items的个数；否则实际更新的item数为indexs的个数；
	//说明：列表元素中的整数类型请勿传入无符号整型
	//说明：item的类型必须为Poco::JSON::Object::Ptr，如果不是，将报告错误；item元素类型将被严格检查，除了bool、int、real、string、enum、image外，都不被允许，非法的类型将被忽略
	//说明：如果item元素包含Image，需要保持对齐，也就是Image的个数应该与item个数一致（即使为空数据也需为它设置为空的Image），否则报告错误。
	static void updateListItems(const std::string &path, const std::set<uint32_t> &indexs, Poco::JSON::Array::Ptr items);

	//更新视口
	//path：数据节点路径，如果不存在或者该路径的数据不是列表类型，model将报告并忽略此次请求
	//first：第一可视项
	static void updateListViewport(const std::string &path, uint32_t first);

	//选中标记唯一列表项
	//path：数据节点路径，如果不存在或者该路径的数据不是列表类型，model将报告并忽略此次请求
	//index：下标，如果为-1表示取消标记
	static void checkListItem(const std::string &path, int32_t index);

	//聚焦唯一列表项
	//path：数据节点路径，如果不存在或者该路径的数据不是列表类型，model将报告并忽略此次请求
	//index：下标，如果为-1表示取消标记
	static void focusListItem(const std::string &path, int32_t index);

	//按键
	//id：按键的id
	//state：按键的状态
	static void vkey(VKeyId key, VKeyState state);

	//用户事件
	//userEvent()事件参数
	//m_EventName：事件名
	//m_Args：事件参数
	struct UserArgs { std::string eventName; Poco::JSON::Object::Ptr args; };
	static uit::Event<UserArgs> &userEvent();

private:
	static void checkConnectState(const std::string &funcName);
	static void onUserAction(const std::string &sEvenName, const std::string &sArgsJsonString);
};

}}
