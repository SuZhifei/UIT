#include "datafw/Parser.h"
#include <fstream>
#include <Poco/JSON/Parser.h>
#include <Poco/StringTokenizer.h>
#include <uit/core/Log.h>
#include <v1/uit/datafw/Types.hpp>
#include "EnumDefinitionManager.h"
#include "ItemTemplateManager.h"
#include "Share.h"

using namespace uit::datafw;
using namespace v1::uit::datafw;

Parser::Parser()
{
	m_EnumDefinitionManager = std::make_shared<EnumDefinitionManager>();
	m_ItemTemplateManager = std::make_shared<ItemTemplateManager>();
}

void Parser::configureLoadingDir(const std::string &dir)
{
	m_ConfigureDir = dir + "/";
}

std::string Parser::getConfiureLoadingDir() const
{
	return m_ConfigureDir;
}

bool Parser::parse()
{
	return parseEnums() & parseItemTempate() & parseData() & parseEvent();
}

Poco::JSON::Object::Ptr Parser::getEnumRoot()
{
	return m_EnumRoot;
}

Poco::JSON::Object::Ptr Parser::getItemTemplateRoot()
{
	return m_ItemTemplateRoot;
}

Poco::JSON::Object::Ptr Parser::getDataRoot()
{
	return m_DataRoot;
}

Poco::JSON::Object::Ptr Parser::getEventRoot()
{
	return m_EventRoot;
}

bool Parser::extractEnumDefinition(const Poco::Dynamic::Var &vEnum, std::string &sDefinitionName, std::vector<std::string> &enumStringValues) const
{
	if (testType(vEnum) != enumValue)
		return false;

	sDefinitionName = vEnum.extract<Poco::JSON::Array::Ptr>()->get(0).toString();
	return getEnumStringValues(sDefinitionName, enumStringValues);
}

int32_t Parser::extractEnumInteger(const Poco::Dynamic::Var &vEnum) const
{
	Poco::JSON::Array::Ptr array = vEnum.extract<Poco::JSON::Array::Ptr>();
	if (testType(vEnum) != enumValue || !m_EnumDefinitionManager->has(array->get(0)))
		return -1;

	return array->get(1);
}

std::string Parser::extractEnumStringValue(const Poco::Dynamic::Var &vEnum) const
{
	if (testType(vEnum) != enumValue)
		return "";

	Poco::JSON::Array::Ptr array = vEnum.extract<Poco::JSON::Array::Ptr>();
	return m_EnumDefinitionManager->getEnumStringValue(array->get(0), array->get(1));
}

bool Parser::getEnumStringValues(const std::string &sDefinitionName, std::vector<std::string> &stringValues) const
{
	return m_EnumDefinitionManager->find(sDefinitionName, stringValues);
}

int32_t Parser::getEnumIntegerByStringValue(const std::string &sDefinitionName, const std::string &stringValue) const
{
	return m_EnumDefinitionManager->getIndexOfStringValue(sDefinitionName, stringValue);
}

std::string Parser::getEnumStringValueByInteger(const std::string &sDefinitionName, int32_t e) const
{
	return m_EnumDefinitionManager->getEnumStringValue(sDefinitionName, e);
}

Poco::JSON::Object::Ptr Parser::extractItemTemplate(const Poco::Dynamic::Var &vList) const
{
	if (testType(vList) != listValue)
		return nullptr;

	return findItemTemplate(vList.extract<Poco::JSON::Array::Ptr>()->get(0));
}

Poco::JSON::Object::Ptr Parser::findItemTemplate(const std::string &templateName) const
{
	return m_ItemTemplateManager->find(templateName);
}

Poco::JSON::Array::Ptr Parser::extractListItems(const Poco::Dynamic::Var &vList) const
{
	if (testType(vList) != listValue)
		return nullptr;

	Poco::JSON::Array::Ptr list = vList.extract<Poco::JSON::Array::Ptr>();
	Poco::JSON::Array::Ptr ret = new Poco::JSON::Array();
	for (int i = 1; i < list->size(); ++i)
		ret->add(list->get(i));

	return ret;
}

Poco::Dynamic::Var Parser::findData(const std::string &path) const
{
	return find(m_DataRoot, path);
}

ValueType Parser::testType(const Poco::Dynamic::Var &v) const
{
	if (v.type() == typeid(Poco::JSON::Object::Ptr))
	{
		return objectValue;
	}
	else if (v.type() == typeid(Poco::JSON::Array::Ptr))
	{
		Poco::JSON::Array::Ptr array = v.extract<Poco::JSON::Array::Ptr>();
		return testArrayType(array);
	}
	else
	{
		return v.isBoolean() ? booleanValue : v.isString() ? stringValue : v.isNumeric() ? (v.isInteger() ? intValue : realValue) : nullValue;
	}
}

ValueType Parser::testPath(const std::string &path, Poco::JSON::Object::Ptr &objParent, Poco::JSON::Array::Ptr &arrayParent)
{
	Poco::StringTokenizer st(path, ".");
	std::vector<std::string> nodeNames(st.begin(), st.end());
	Poco::JSON::Object::Ptr p = m_DataRoot;
	for (int32_t i = 0; i != nodeNames.size(); ++i)
	{
		const std::string &sName = nodeNames[i];
		if (!p->has(sName))
		{
			return nullValue;
		}
		else if (p->isObject(sName))
		{
			if (i == nodeNames.size() - 1)
			{
				objParent = p->getObject(sName);
				return objectValue;
			}
			p = p->getObject(sName);
		}
		else if (p->isArray(sName))
		{
			arrayParent = p->getArray(sName);
			return testArrayType(arrayParent);
		}
		else
		{
			objParent = p;
			Poco::Dynamic::Var v = p->get(sName);
			return v.isBoolean() ? booleanValue : v.isString() ? stringValue : v.isNumeric() ? (v.isInteger() ? intValue : realValue) : nullValue;
		}
	}
	return nullValue;
}

Poco::JSON::Object::Ptr Parser::findEvent(const std::string &path) const
{
	Poco::JSON::Object::Ptr obj = m_EventRoot;
	Poco::StringTokenizer st(path, ".");
	std::vector<std::string> nodeNames(st.begin(), st.end());
	for (int32_t i = 0; i != nodeNames.size(); ++i)
	{
		const std::string &sName = nodeNames[i];
		if (i == nodeNames.size() - 1)
		{
			Poco::JSON::Object::Ptr eventObj = obj->getObject(sName);
			if (!eventObj)
			{
				return nullptr;
			}
			else
			{
				for (auto iter = eventObj->begin(); iter != eventObj->end(); ++iter)
				{
					ValueType argType = testType(iter->second);
					if (argType != booleanValue && argType != intValue && argType != realValue && argType != stringValue && argType != enumValue)
						return nullptr;
				}
				return eventObj;
			}
		}
		else
		{
			obj = obj->getObject(sName);
			if (!obj)
				return nullptr;
		}
	}
	return nullptr;
}

bool Parser::findEnumDefinition(const std::string &path, std::vector<std::string> &stringValues) const
{
	return m_EnumDefinitionManager->find(path, stringValues);
}

ImageData Parser::extractImage(const Poco::Dynamic::Var &v) const
{
	auto extract = [&](const Poco::Dynamic::Var &v)-> Types::Image
	{
		if (testType(v) != imageValue)
			return Types::Image();

		Poco::JSON::Array::Ptr array = v.extract<Poco::JSON::Array::Ptr>();
		if (array->size() == 1)
		{
			return Types::Image();
		}
		else
		{
			Types::Image ret;
			try {
				ret = array->get(1).extract<Types::Image>();
			}
			catch (...) { Log::error(DATAFW_PARSER_LOG_TAG, "extract image catch an exception."); return Types::Image(); }
			return ret;
		}
	};

	Types::Image image = extract(v);
	if (image.getFormatknown())
		return ImageData(image.getBuffer().data(), image.getWidth(), image.getHeight(), (ImageFormat)image.getFormat());
	else
		return ImageData(image.getBuffer().data(), image.getBuffer().size());

}

Poco::Dynamic::Var Parser::find(Poco::JSON::Object::Ptr root, const std::string &path)
{
	if (!root || path.empty())
		return Poco::Dynamic::Var();

	Poco::JSON::Object::Ptr obj = root;
	Poco::StringTokenizer st(path, ".");
	std::vector<std::string> nodeNames(st.begin(), st.end());
	for (int32_t i = 0; i != nodeNames.size(); ++i)
	{
		const std::string &sName = nodeNames[i];
		if (i == nodeNames.size() - 1)
		{
			return obj->get(sName);
		}
		else
		{
			obj = obj->getObject(sName);
			if (!obj)
				return Poco::Dynamic::Var();
		}
	}
	return Poco::Dynamic::Var();
}

Poco::Dynamic::Var Parser::makeDefault(ValueType type, const std::string &sExtra)
{
	switch (type)
	{
	case booleanValue:	return false;
	case intValue:		return 0;
	case realValue:		return 0.0;
	case stringValue:	return "";
	case enumValue:
	{
		Poco::JSON::Array::Ptr array = new Poco::JSON::Array();
		array->set(0, sExtra);
		array->set(1, 0);
		return array;
	}
	case imageValue:
	{
		Poco::JSON::Array::Ptr array = new Poco::JSON::Array();
		array->set(0, DATAFW_IMAGE_DEFINITION_STRING);
		return array;
	}
	case objectValue:
	{
		Poco::JSON::Object::Ptr obj = new Poco::JSON::Object();
		return obj;
	}
	case listValue:
	{
		Poco::JSON::Array::Ptr array = new Poco::JSON::Array();
		array->set(0, sExtra);
		return array;
	}
	default:	return Poco::Dynamic::Var();
	}
}

bool Parser::isImage(const Poco::Dynamic::Var &v)
{
	bool bIsArray = v.type() == typeid(Poco::JSON::Array::Ptr);
	if (!bIsArray)
		return false;

	Poco::JSON::Array::Ptr array = v.extract<Poco::JSON::Array::Ptr>();
	return array->size() >= 1 && array->get(0).toString() == DATAFW_IMAGE_DEFINITION_STRING;

}

bool Parser::parseOneFile(const std::string &path, Poco::JSON::Object::Ptr &obj)
{
	std::ifstream stream(path.data());
	if (!stream)
	{
		Log::error(DATAFW_PARSER_LOG_TAG, "can't find [%s].", path.data());
		return false;
	}

	Poco::JSON::Parser parser;
	Poco::Dynamic::Var result;
	try {
		result = parser.parse(stream);
	}
	catch (Poco::JSON::JSONException)
	{
		stream.close();
		Log::error(DATAFW_PARSER_LOG_TAG, "parse [%s] error.", path.data());
		return false;
	}
	stream.close();
	obj = result.extract<Poco::JSON::Object::Ptr>();
	return true;
}

bool Parser::parseEnums()
{
	m_EnumDefinitionManager->clear();
	m_EnumRoot = new Poco::JSON::Object();
	bool ret = parseOneFile(m_ConfigureDir + DATAFW_ENUM_DEFINITION_FILE, m_EnumRoot);
	if (!ret)
		return false;

	try { recurveEnums(m_EnumRoot, ""); }
	catch (JsonException &) { ret = false; }

	return ret;
}

void Parser::recurveEnums(Poco::JSON::Object::Ptr obj, const std::string &sParent)
{
	for (Poco::JSON::Object::Iterator iter = obj->begin(); iter != obj->end(); ++iter)
	{
		std::string sName = iter->first;
		const Poco::Dynamic::Var &v = iter->second;
		std::string sCombine = sParent + "." + sName;
		if (v.type() == typeid(Poco::JSON::Object::Ptr))
		{
			recurveEnums(v.extract<Poco::JSON::Object::Ptr>(), sCombine);
		}
		else if (v.type() == typeid(Poco::JSON::Array::Ptr))
		{
			Poco::JSON::Array::Ptr array = v.extract<Poco::JSON::Array::Ptr>();
			std::vector<std::string> stringValues;
			for (int32_t j = 0; j != array->size(); ++j)
			{
				Poco::Dynamic::Var e = array->get(j);
				if (e.isString())
				{
					std::string sValue = array->get(j);
					if (std::find(stringValues.begin(), stringValues.end(), sValue) == stringValues.end())
						stringValues.push_back(sValue);
					else
						Log::warn(DATAFW_PARSER_LOG_TAG, "[%s] enum value redefine.", (sCombine.substr(1) + "." + sValue).data());
				}
				else
				{
					Log::error(DATAFW_PARSER_LOG_TAG, "enum value must be a string type, check [%s][%d] in file [%s]", sCombine.substr(1).data(), j, DATAFW_ENUM_DEFINITION_FILE);
					throw JsonException();
				}
			}
			m_EnumDefinitionManager->insert(sCombine.substr(1), stringValues);
		}
		else
		{
			Log::error(DATAFW_PARSER_LOG_TAG, "enum definition must be a array type , check [%s] in file [%s]", sCombine.substr(1).data(), DATAFW_ENUM_DEFINITION_FILE);
			throw JsonException();
		}
	}
}

bool Parser::parseItemTempate()
{
	m_ItemTemplateManager->clear();
	m_ItemTemplateRoot = new Poco::JSON::Object();
	bool ret = parseOneFile(m_ConfigureDir + DATAFW_ITEMTEMPLATE_DEFINITION_FILE, m_ItemTemplateRoot);
	if (!ret)
		return false;

	try { recurveItemTemplates(m_ItemTemplateRoot, ""); }
	catch (JsonException &) { ret = false; }

	return ret;
}

void Parser::recurveItemTemplates(Poco::JSON::Object::Ptr obj, const std::string &sParent)
{
	for (Poco::JSON::Object::Iterator iter = obj->begin(); iter != obj->end(); ++iter)
	{
		std::string sName = iter->first;
		const Poco::Dynamic::Var &v = iter->second;
		std::string sCombine = sParent + "." + sName;
		if (v.type() == typeid(Poco::JSON::Object::Ptr))
		{
			Poco::JSON::Object::Ptr xObj = v.extract<Poco::JSON::Object::Ptr>();
			if (ItemTemplateManager::isItemTemplate(xObj))
			{
				m_ItemTemplateManager->insert(sCombine.substr(1), xObj);
			}
			else
			{
				recurveItemTemplates(xObj, sCombine);
			}
		}
		else
		{
			Log::error(DATAFW_PARSER_LOG_TAG, "item template must be a object type, check [%s] in file [%s]", sCombine.substr(1).data(), DATAFW_ITEMTEMPLATE_DEFINITION_FILE);
			throw JsonException();
		}
	}
}

bool Parser::parseData()
{
	m_DataRoot = new Poco::JSON::Object();
	return parseOneFile(m_ConfigureDir + DATAFW_DATA_DEFINITION_FILE, m_DataRoot);
}

bool Parser::parseEvent()
{
	m_EventRoot = new Poco::JSON::Object();
	return parseOneFile(m_ConfigureDir + DATAFW_EVENT_DEFINITION_FILE, m_EventRoot);
}

ValueType Parser::testArrayType(Poco::JSON::Array::Ptr array) const
{
	int32_t size = array->size();
	if (size == 0)
		return nullValue;

	std::string s0 = array->get(0).toString();
	if (size == 2 && m_EnumDefinitionManager->has(s0) && array->get(1).isInteger())
	{
		return enumValue;
	}
	else if (size >= 1 && s0 == DATAFW_IMAGE_DEFINITION_STRING)
	{
		return imageValue;
	}
	else if (size >= 1 && m_ItemTemplateManager->find(s0))
	{
		return listValue;
	}
	else
	{
		return nullValue;
	}
}
