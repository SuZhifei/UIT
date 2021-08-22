#pragma once
#include <map>
#include <vector>
#include <string>

namespace uit { namespace datafw {

class EnumDefinitionManager final
{
public:
	//插入一个枚举定义
	bool insert(const std::string &sDefinitionName, const std::vector<std::string> &stringValues);

	//是否存在枚举定义
	bool has(const std::string &sDefinitionName) const;

	//查找一个枚举定义
	//如果存在返回true，否则返回false。stringValues为返回的枚举值
	bool find(const std::string &sDefinitionName, std::vector<std::string> &stringValues) const;

	//获取枚举定义当前字符串值
	//如果枚举定义不存在，返回"",否则返回当前字符串值
	std::string getEnumStringValue(const std::string &sDefinitionName, int32_t index) const;

	//获取枚举值
	//如果枚举定义不存在，返回-1
	int32_t getIndexOfStringValue(const std::string &sDefinitionName, const std::string &sStringValue) const;

	//清除
	void clear();

private:
	std::map< std::string, std::vector<std::string> >	m_Denifitions;
};

}}
