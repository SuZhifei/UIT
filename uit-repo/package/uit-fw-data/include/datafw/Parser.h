#pragma once
#include <vector>
#include <string>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Object.h>
#include "Type.h"
#include "ImageData.h"

namespace uit { namespace datafw {

class EnumDefinitionManager;
class ItemTemplateManager;
class UIT_API Parser final
{
public:
	//构建一个解析器
	Parser();
	~Parser() = default;

	//配置加载文件夹
	//parse防范将从此路径中加载并解析文件
	void configureLoadingDir(const std::string &dir);

	//获取加载文件夹
	std::string getConfiureLoadingDir() const;

	//获取配置目录

	//从配置文件夹中解析
	//返回值：true表示成功，否则失败
	bool parse();

	//获取数据根
	Poco::JSON::Object::Ptr getDataRoot();

	//获取事件根
	Poco::JSON::Object::Ptr getEventRoot();

	//获取枚举定义根
	Poco::JSON::Object::Ptr getEnumRoot();

	//获取模板定义根
	Poco::JSON::Object::Ptr getItemTemplateRoot();

	//提取枚举定义
	//vEnum：枚举v
	//sDefinitionName：枚举定义名，作为返回值
	//enumStringValues：枚举定义的所有枚举值，作为返回值
	//返回值：如果枚举定义存在返回true，否则返回false
	bool extractEnumDefinition(const Poco::Dynamic::Var &vEnum, std::string &sDefinitionName, std::vector<std::string> &enumStringValues) const;

	//提取枚举的整数值
	//vEnum：枚举v
	//返回值：返回枚举的整数值，如果枚举节点不存在，返回-1
	int32_t extractEnumInteger(const Poco::Dynamic::Var &vEnum) const;

	//提取枚举字符串值
	//vEnum：枚举v
	//返回值：返回枚举的字符串值，如果枚举节点不存在，返回""
	std::string extractEnumStringValue(const Poco::Dynamic::Var &vEnum) const;

	//获取枚举定义所有的字符串枚举值
	//sDefinitionName：枚举定义名字
	//enumValues：枚举定义对应的枚举值，作为返回值
	//返回值：如果枚举定义存在返回true，否则返回false
	bool getEnumStringValues(const std::string &sDefinitionName, std::vector<std::string> &stringValues) const;

	//获取枚举字符串值对应的整数值
	//sDefinitionName：枚举定义名字
	//stringValue：字符串值
	//返回值：如果stringValue是sDefinition的字符串值返回它的下标否则返回-1
	int32_t getEnumIntegerByStringValue(const std::string &sDefinitionName, const std::string &stringValue) const;

	//获取枚举整数值对应的字符串值
	//sDefinitionName：枚举定义名字
	//e：整数值
	//返回值：如果e是sDefinition的整数值值返回它的对应的字符串值否则返回""
	std::string getEnumStringValueByInteger(const std::string &sDefinitionName, int32_t e) const;

	//提取列表项模板
	//vList：列表v
	//如果vList不是列表类型，返回为空
	Poco::JSON::Object::Ptr extractItemTemplate(const Poco::Dynamic::Var &vList) const;

	//更加名字获取列表模板
	//templateName：模板名字
	//如果不存在返回空
	Poco::JSON::Object::Ptr findItemTemplate(const std::string &templateName) const;

	//获取列表项
	//如果vList不是列表，返回空
	Poco::JSON::Array::Ptr extractListItems(const Poco::Dynamic::Var &vList) const;

	//从路径的查找数据
	//path：数据路径
	//如果无此数据，返回空var
	Poco::Dynamic::Var findData(const std::string &path) const;

	//测试数据类型
	ValueType testType(const Poco::Dynamic::Var &v) const;
	ValueType testPath(const std::string &path, Poco::JSON::Object::Ptr &objParent, Poco::JSON::Array::Ptr &arrayParent);

	//从路径的查找事件
	//如果此事件的参数，如果无此事件，返回空
	Poco::JSON::Object::Ptr findEvent(const std::string &path) const;

	//从路径查找枚举定义
	bool findEnumDefinition(const std::string &path, std::vector<std::string> &stringValues) const;

	//var转换为ImageData
	ImageData extractImage(const Poco::Dynamic::Var &v) const;

public:
	//查找路径中的节点
	//root：根
	//path：路径
	//返回值：如果root为空或者path为空，返回空Poco::Dynamic::Var()
	static Poco::Dynamic::Var find(Poco::JSON::Object::Ptr root, const std::string &path);
	static Poco::Dynamic::Var makeDefault(ValueType type, const std::string &sExtra = "");
	static bool isImage(const Poco::Dynamic::Var &v);

private:
	//解析相关
	bool parseOneFile(const std::string &path, Poco::JSON::Object::Ptr &obj);
	bool parseEnums();
	void recurveEnums(Poco::JSON::Object::Ptr obj, const std::string &sParent);
	bool parseItemTempate();
	void recurveItemTemplates(Poco::JSON::Object::Ptr obj, const std::string &sParent);
	bool parseData();
	bool parseEvent();
	ValueType testArrayType(Poco::JSON::Array::Ptr array) const;

	std::string								m_ConfigureDir;
	Poco::JSON::Object::Ptr					m_EnumRoot;
	Poco::JSON::Object::Ptr					m_ItemTemplateRoot;
	Poco::JSON::Object::Ptr					m_DataRoot;
	Poco::JSON::Object::Ptr					m_EventRoot;
	std::shared_ptr<EnumDefinitionManager>	m_EnumDefinitionManager;
	std::shared_ptr<ItemTemplateManager>	m_ItemTemplateManager;
};

}}
