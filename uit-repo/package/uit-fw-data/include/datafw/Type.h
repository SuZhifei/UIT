#pragma once
#include <string>
#include <uit/core/Def.h>

namespace uit { namespace datafw{

enum ValueType
{
	nullValue = 0,			//空类型
	booleanValue,			//布尔类型
	intValue,				//整型
	realValue,				//浮点类型
	stringValue,			//字符串类型
	enumValue,				//枚举类型
	imageValue,				//图片类型
	listValue,				//数组类型
	objectValue,			//结构类型
};
enum ImageFormat
{
	invalidFormat = 0,		//不可用的格式
	rgb555Format,			//16位555格式
	rgb565Format,			//16位565格式
	rgb888Format,			//24位888格式
	rgb8888Format,			//32位8888格式
};
enum VKeyId
{
	vkeyNone = 0,			//未知按键
	vkeyUp,					//上
	vkeyDown,				//下
	vkeyLeft,				//左
	vkeyRight,				//右
	vkeyLeftUp,				//左上
	vkeyRightUp,			//右上
	vkeyLeftDown,			//左下
	vkeyRightDown,			//右下
	vkeyClockwise,			//正时针
	vkeyAntiClockwise,		//逆时针
	vkeyOK,					//Ok
	vkeyBack,				//返回
};
enum VKeyState
{
	vkeyStateUnknown = 0,	//未知按键状态
	vkeyStatePress,			//压下
	vkeyStateCancel,		//取消
	vkeyStateLongPress,		//长按
	vkeyStateRelease,		//释放
	vkeyStateClick,			//点击
};

UIT_API std::string toString(ValueType type);
UIT_API std::string toString(ImageFormat type);
UIT_API std::string toString(VKeyId id);
UIT_API std::string toString(VKeyState state);

}}