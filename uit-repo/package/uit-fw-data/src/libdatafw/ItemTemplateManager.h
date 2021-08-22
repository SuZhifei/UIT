#pragma once
#include <map>
#include <string>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Object.h>

namespace uit { namespace datafw {

class ItemTemplateManager final
{
public:
	//插入一个模板定义
	bool insert(const std::string &sName, Poco::JSON::Object::Ptr itemTemplate);

	//查找模板，如果查不到，返回空
	Poco::JSON::Object::Ptr find(const std::string &sTemplateName) const;

	//清除
	void clear();

public:
	static bool isItemTemplate(Poco::JSON::Object::Ptr obj);

private:
	std::map<std::string, Poco::JSON::Object::Ptr>	m_ItemTemplates;
};

}}
