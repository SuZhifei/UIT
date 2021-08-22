#include "EnumDefinitionManager.h"

using namespace uit::datafw;

bool EnumDefinitionManager::insert(const std::string &sDefinitionName, const std::vector<std::string> &stringValues)
{
	std::pair<std::map< std::string, std::vector<std::string> >::iterator, bool> ret = m_Denifitions.insert(std::make_pair(sDefinitionName, stringValues));
	return ret.second;
}

bool EnumDefinitionManager::find(const std::string &sDefinitionName, std::vector<std::string> &stringValues) const
{
	auto iterFind = m_Denifitions.find(sDefinitionName);
	if (iterFind != m_Denifitions.end())
	{
		stringValues = iterFind->second;
		return true;
	}

	return false;
}

bool EnumDefinitionManager::has(const std::string &sDefinitionName) const
{
	return m_Denifitions.find(sDefinitionName) != m_Denifitions.end();
}

std::string EnumDefinitionManager::getEnumStringValue(const std::string &sDefinitionName, int32_t index) const
{
	auto iterFind = m_Denifitions.find(sDefinitionName);
	if (iterFind == m_Denifitions.end() || (index < 0 || index >= (int)(iterFind->second.size())))
		return "";

	return iterFind->second.at(index);
}

int32_t EnumDefinitionManager::getIndexOfStringValue(const std::string &sDefinitionName, const std::string &sStringValue) const
{
	auto iterFind = m_Denifitions.find(sDefinitionName);
	if (iterFind == m_Denifitions.end())
		return -1;

	for (int32_t i = 0; i != iterFind->second.size(); ++i)
		if (sStringValue == iterFind->second.at(i))
			return i;

	return -1;
}

void EnumDefinitionManager::clear()
{
	m_Denifitions.clear();
}
