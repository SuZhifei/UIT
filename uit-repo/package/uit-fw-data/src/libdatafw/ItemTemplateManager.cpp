#include "ItemTemplateManager.h"

using namespace uit::datafw;

bool ItemTemplateManager::insert(const std::string &sName, Poco::JSON::Object::Ptr itemTemplate)
{
	std::pair<std::map<std::string, Poco::JSON::Object::Ptr>::iterator, bool> ret = m_ItemTemplates.insert(std::make_pair(sName, itemTemplate));
	return ret.second;
}

Poco::JSON::Object::Ptr ItemTemplateManager::find(const std::string &sTemplateName) const
{
	auto iterFind = m_ItemTemplates.find(sTemplateName);
	if (iterFind != m_ItemTemplates.end())
		return iterFind->second;

	return nullptr;
}

void ItemTemplateManager::clear()
{
	m_ItemTemplates.clear();
}

bool ItemTemplateManager::isItemTemplate(Poco::JSON::Object::Ptr obj)
{
	if (obj->size() == 0)
		return false;

	for (Poco::JSON::Object::ConstIterator iter = obj->begin(); iter != obj->end(); ++iter)
	{
		const Poco::Dynamic::Var &v = iter->second;
		if (v.isBoolean() || v.isNumeric() || v.isString() || v.type() == typeid(Poco::JSON::Array::Ptr))
			continue;
		else
			return false;
	}
	return true;
}
