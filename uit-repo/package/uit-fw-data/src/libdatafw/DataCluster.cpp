#include "DataCluster.h"
#include <Poco/JSON/Parser.h>
#include <Poco/String.h>
#include <uit/core/Log.h>
#include "datafw/Parser.h"
#include "EnumDefinitionManager.h"
#include "ItemTemplateManager.h"
#include "Share.h"

using namespace uit::datafw;

void DataCluster::setParser(Parser *parser)
{
	m_parser = parser;
}

bool DataCluster::updateBoolean(const std::string &path, bool b)
{
	Poco::JSON::Object::Ptr objParent;
	Poco::JSON::Array::Ptr arrayParent;
	ValueType type = m_parser->testPath(path, objParent, arrayParent);
	if (type == booleanValue)
	{
		std::string sNodeName = getNodeName(path);
		if (objParent->get(sNodeName) == b)
			return false;

		objParent->set(sNodeName, b);
		return true;
	}
	else
	{
		Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a %s data when call [%s].", path.data(), type == nullValue ? "valid" : "boolean", __FUNCTION__);
		return false;
	}
}

bool DataCluster::updateInteger(const std::string &path, int32_t n)
{
	Poco::JSON::Object::Ptr objParent;
	Poco::JSON::Array::Ptr arrayParent;
	ValueType type = m_parser->testPath(path, objParent, arrayParent);
	if (type == intValue)
	{
		std::string sNodeName = getNodeName(path);
		if (objParent->get(sNodeName) == n)
			return false;

		objParent->set(sNodeName, n);
		return true;
	}
	else
	{
		Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a %s data when call [%s].", path.data(), type == nullValue ? "valid" : "integer", __FUNCTION__);
		return false;
	}
}

bool DataCluster::updateReal(const std::string &path, double d)
{
	Poco::JSON::Object::Ptr objParent;
	Poco::JSON::Array::Ptr arrayParent;
	ValueType type = m_parser->testPath(path, objParent, arrayParent);
	if (type == realValue)
	{
		std::string sNodeName = getNodeName(path);
		if (objParent->get(sNodeName) == d)
			return false;

		objParent->set(sNodeName, d);
		return true;
	}
	else
	{
		Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a %s data when call [%s].", path.data(), type == nullValue ? "valid" : "real", __FUNCTION__);
		return false;
	}
}

bool DataCluster::updateString(const std::string &path, const std::string &str)
{
	Poco::JSON::Object::Ptr objParent;
	Poco::JSON::Array::Ptr arrayParent;
	ValueType type = m_parser->testPath(path, objParent, arrayParent);
	if (type == stringValue)
	{
		if (str.size() > DATAFW_STRING_SIZE_LIMIT)
		{
			Log::error(DATAFW_MODEL_LOG_TAG, "[%s] update too long string[len=%d] when call [%s] limited by %d bytes.", path.data(), str.size(), __FUNCTION__, DATAFW_STRING_SIZE_LIMIT);
			return false;
		}

		std::string sNodeName = getNodeName(path);
		if (objParent->get(sNodeName) == str)
			return false;

		objParent->set(sNodeName, str);
		return true;
	}
	else
	{
		Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a %s data when call [%s].", path.data(), type == nullValue ? "valid" : "string", __FUNCTION__);
		return false;
	}
}

bool DataCluster::updateEnum(const std::string &path, int32_t e)
{
	Poco::JSON::Object::Ptr objParent;
	Poco::JSON::Array::Ptr arrayParent;
	ValueType type = m_parser->testPath(path, objParent, arrayParent);
	if (type == enumValue)
	{
		if (arrayParent->get(1) == e)
			return false;

		std::string sEnumDefinition = arrayParent->get(0);
		std::vector<std::string> stringValues;
		m_parser->getEnumStringValues(sEnumDefinition, stringValues);
		if (e < 0 || e >= stringValues.size())
		{
			Log::error(DATAFW_MODEL_LOG_TAG, "[%d] is out of range for enum [%s]'s values(0-%d) when call [%s].", e, sEnumDefinition.data(), stringValues.size() - 1, __FUNCTION__);
			return false;
		}
		
		arrayParent->set(1, e);
		return true;
	}
	else
	{
		Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a %s data when call [%s].", path.data(), type == nullValue ? "valid" : "enum", __FUNCTION__);
		return false;
	}
}

bool DataCluster::updateEnumStringValue(const std::string &path, const std::string sEnumStringValue)
{
	Poco::JSON::Object::Ptr objParent;
	Poco::JSON::Array::Ptr arrayParent;
	ValueType type = m_parser->testPath(path, objParent, arrayParent);
	if (type == enumValue)
	{
		std::string sEnumDefinition = arrayParent->get(0);
		int32_t e = m_parser->getEnumIntegerByStringValue(sEnumDefinition, sEnumStringValue);
		if (e == -1)
		{
			std::vector<std::string> stringValues;
			m_parser->getEnumStringValues(sEnumDefinition, stringValues);
			Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a value of enum [%s], you can only use one of [%s] when call [%s].", 
				sEnumStringValue.data(), sEnumDefinition.data(), Poco::cat(std::string("|"), stringValues.begin(), stringValues.end()).data(), __FUNCTION__);
			return false;
		}
		else
		{
			if (arrayParent->get(1) == e)
				return false;

			arrayParent->set(1, e);
			return true;
		}
	}
	else
	{
		Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a %s data when call [%s].", path.data(), type == nullValue ? "valid" : "enum", __FUNCTION__);
		return false;
	}
}

bool DataCluster::updateImage(const std::string &path, const Types::Image &image)
{
	Poco::JSON::Object::Ptr objParent;
	Poco::JSON::Array::Ptr arrayParent;
	ValueType type = m_parser->testPath(path, objParent, arrayParent);
	if (type == imageValue)
	{
		if (image.getBuffer().size() > DATAFW_IMAGE_SIZE_LIMIT)
		{
			Log::error(DATAFW_MODEL_LOG_TAG, "[%s] update too large image[len=%d] when call [%s] limited by %d bytes.", path.data(), image.getBuffer().size(), __FUNCTION__, DATAFW_IMAGE_SIZE_LIMIT);
			return false;
		}

		if (arrayParent->size() == 2 && arrayParent->get(1).extract<Types::Image>() == image)
			return false;

		arrayParent->set(1, image);
		return true;
	}
	else
	{
		Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a %s data when call [%s].", path.data(), type == nullValue ? "valid" : "image", __FUNCTION__);
		return false;
	}
}

bool DataCluster::updateGroup(const std::vector<std::string> &paths, Poco::JSON::Array::Ptr values, std::vector<std::string> &paths_out, Poco::JSON::Array::Ptr values_out)
{
	for (auto i = 0; i != paths.size(); ++i)
	{
		auto const &path = paths[i];
		auto const &v = values->get(i);
		Poco::JSON::Object::Ptr objParent;
		Poco::JSON::Array::Ptr arrayParent;
		ValueType pathType = m_parser->testPath(path, objParent, arrayParent);
		ValueType vType = m_parser->testType(v);
		try {
			switch (pathType)
			{
			case booleanValue:	if (updateBoolean(path, v.convert<bool>())) { paths_out.push_back(path); values_out->add(v); };			break;
			case intValue:		if (updateInteger(path, v.convert<int>())) { paths_out.push_back(path); values_out->add(v); };			break;
			case realValue:		if (updateReal(path, v.convert<float>())) { paths_out.push_back(path); values_out->add(v + DATAFW_REAL_DIFF_VALUE); };			break;
			case stringValue:	if (updateString(path, v.convert<std::string>())) { paths_out.push_back(path); values_out->add(v); };	break;
			case enumValue:
			{
				std::string sEnumDefinition = arrayParent->get(0);
				Poco::JSON::Array::Ptr enumObj = new Poco::JSON::Array();
				enumObj->set(0, sEnumDefinition);
				if (vType == intValue && updateEnum(path, v))
				{
					paths_out.push_back(path);
					enumObj->set(1, v);
					values_out->add(enumObj);
				}
				else if (vType == stringValue && updateEnumStringValue(path, v))
				{
					paths_out.push_back(path);
					int32_t e = m_parser->getEnumIntegerByStringValue(sEnumDefinition, v);
					enumObj->set(1, e);
					values_out->add(enumObj);
				}
			}
			break;
			default:	Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a valid data when call [%s].", path.data(), __FUNCTION__); break;
			}
		}
		catch (...)
		{
			Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a %s data when call [%s].", path.data(), vType == nullValue ? "valid" : toString(vType).data(), __FUNCTION__);
			continue;
		}
	}
	return true;
}

//如果浮点是2.0这种接近整数的形式，poco::toString会直接转成 2的问题，导致类型由double变成int，是poco的bug，github上有人提过issue，答复说2.0版本解决？
bool DataCluster::updateListItems(const std::string &path, const std::vector<uint32_t> &indexs, const std::string &sItemJsonString, const std::vector<Types::Image> &images, std::vector<uint32_t> &indexs_out, std::string &sItemJsonString_out, std::vector<Types::Image> &images_out)
{
	Poco::JSON::Object::Ptr objParent;
	Poco::JSON::Array::Ptr localArray;
	ValueType type = m_parser->testPath(path, objParent, localArray);
	if (type == listValue)
	{
		if (localArray->size() == 1)
			return false;

		//获取item模板
		Poco::JSON::Object::Ptr itemTemplate = m_parser->findItemTemplate(localArray->get(0));
		if (!itemTemplate)
		{
			Log::error(DATAFW_MODEL_LOG_TAG, "[%s] get itemTemplate[%s] null when call [%s]", path.data(), localArray->get(0).toString().data(), __FUNCTION__);
			return false;
		}

		//实际更新的下标和item项
		Poco::JSON::Array::Ptr actualItemsWithoutAttachment = new Poco::JSON::Array();
		//解析待更新items
		Poco::JSON::Parser parser;
		Poco::JSON::Array::Ptr arraySource;
		try {
			Poco::Dynamic::Var vRoot = parser.parse(sItemJsonString);
			arraySource = vRoot.extract<Poco::JSON::Array::Ptr>();
		}
		catch (...)
		{
			Log::error(DATAFW_MODEL_LOG_TAG, "[%s] parse sItemJsonString fail when call [%s], items source:%s\r\n", path.data(), __FUNCTION__, sItemJsonString.data());
			return false;
		}

		//遍历更新item内容并得出实际更新的内容
		for (uint32_t i = 0; i != indexs.size(); ++i)
		{
			Poco::JSON::Object::Ptr localItem = localArray->getObject(indexs[i] + 1);	//此时的localItem可能是个空的Poco::Json::Object::Ptr，因为resize的时候填充的
			Poco::JSON::Object::Ptr sourceItem = arraySource->getObject(i);
			if (localItem.isNull() || sourceItem.isNull())	//找不到此下标的localItem或者sourceItem则忽略
				continue;

			indexs_out.push_back(indexs[i]);
			Poco::JSON::Object::Ptr itemWithoutAttachment = new Poco::JSON::Object();
			for (Poco::JSON::Object::Iterator iter = sourceItem->begin(); iter != sourceItem->end(); ++iter)
			{
				const std::string &sName = iter->first;
				const Poco::Dynamic::Var &localV = itemTemplate->get(sName);
				const Poco::Dynamic::Var &sourceV = iter->second;
				if (localV.isEmpty())	//如果模板中无此字段，则忽略
					continue;

				ValueType localType = m_parser->testType(localV);
				ValueType sourceType = m_parser->testType(sourceV);
				switch (localType)
				{
				case booleanValue:
				case intValue:
				case realValue:
				{
					if (sourceType == booleanValue || sourceType == intValue || sourceType == realValue)
					{
						//转换失败表示localType和sourceType不可互转，即不是同一类型
						//另外，client端整型传入无符号且数值较大（比如4294967295，__int64类型），convert会产生Poco::OutOfRange异常；传-1没问题
						Poco::Dynamic::Var v;
						try {
							if (localType == booleanValue)		v = sourceV.convert<bool>();
							else if (localType == intValue)		v = sourceV.convert<int>();
							else if (localType == realValue)	v = sourceV.convert<double>() + DATAFW_REAL_DIFF_VALUE;
						}
						catch (...){
							Log::error(DATAFW_MODEL_LOG_TAG, "type[%s] convert to type[%s] fail when update element[%s], ignore. maybe value overflow.", toString(localType).data(), toString(sourceType).data(), (path + "." + sName).data());
						}
						localItem->set(sName, v);
						itemWithoutAttachment->set(sName, v);
					}
					else
					{
						Log::error(DATAFW_MODEL_LOG_TAG, "you can't update [%s](%s) with type [%s], ignore", (path + "." + sName).data(), toString(localType).data(), toString(sourceType).data());
					}
					break;
				}
				case stringValue:
				{
					if (sourceType == stringValue)
					{
						localItem->set(sName, sourceV);
						itemWithoutAttachment->set(sName, sourceV);
					}
					else
					{
						Log::error(DATAFW_MODEL_LOG_TAG, "you can't update [%s](%s) with type [%s], ignore", (path + "." + sName).data(), toString(localType).data(), toString(sourceType).data());
					}
					break;
				}
				case enumValue:
				{
					std::string sEnumDefinition = itemTemplate->getArray(sName)->get(0);
					Poco::JSON::Array::Ptr enumObj = new Poco::JSON::Array();
					enumObj->set(0, sEnumDefinition);
					if (sourceType == intValue)
					{
						std::vector<std::string> stringValues;
						m_parser->getEnumStringValues(sEnumDefinition, stringValues);
						if (sourceV < 0 || sourceV >= stringValues.size())
						{
							Log::error(DATAFW_MODEL_LOG_TAG, "[%d] is out of range for enum [%s]'s values(0-%d) when call [%s].", sourceV.convert<int>(), sEnumDefinition.data(), stringValues.size() - 1, __FUNCTION__);
						}
						else
						{
							enumObj->set(1, sourceV);
							localItem->set(sName, enumObj);
							itemWithoutAttachment->set(sName, enumObj);
						}
					}
					else if (sourceType == stringValue)
					{
						int32_t e = m_parser->getEnumIntegerByStringValue(sEnumDefinition, sourceV.toString());
						if (e != -1)
						{
							enumObj->set(1, e);
							localItem->set(sName, enumObj);
							itemWithoutAttachment->set(sName, enumObj);
						}
						else
						{
							std::vector<std::string> stringValues;
							m_parser->getEnumStringValues(sEnumDefinition, stringValues);
							Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a value of enum [%s], you can only use one of [%s]", 
								sourceV.toString().data(), sEnumDefinition.data(), Poco::cat(std::string("|"), stringValues.begin(), stringValues.end()).data());
						}
					}
					else
					{
						Log::error(DATAFW_MODEL_LOG_TAG, "you can't update [%s](%s) with type [%s], ignore", (path + "." + sName).data(), toString(localType).data(), toString(sourceType).data());
					}
					break;
				}
				case imageValue:
				{
					if (sourceType == imageValue)
					{
						Poco::JSON::Array::Ptr imageObj = Parser::makeDefault(imageValue).extract<Poco::JSON::Array::Ptr>();
						if (i >= images.size())
						{
							Log::error(DATAFW_MODEL_LOG_TAG, "[%s] can't find image[%d] when update image element, if your updating items are with images, please feed each item even though iamge is empty.", (path + "." + sName).data(), i);
						}
						else
						{
							imageObj->set(1, images[i]);
							images_out.push_back(images[i]);
						}
						localItem->set(sName, imageObj);
						itemWithoutAttachment->set(sName, sourceV);
					}
					else
					{
						Log::error(DATAFW_MODEL_LOG_TAG, "you can't update [%s](%s) with type [%s], ignore", (path + "." + sName).data(), toString(localType).data(), toString(sourceType).data());
					}
					break;
				}
				default:
					Log::error(DATAFW_MODEL_LOG_TAG, "item element [%s]'s type is [%s], not a surppoted type in item", (path + "." + sName).data(), toString(localType).data());
					break;
				}
			}
			if(itemWithoutAttachment->size() != 0)
				actualItemsWithoutAttachment->add(itemWithoutAttachment);
		}

		if (actualItemsWithoutAttachment->size() == 0)
		{
			return false;
		}
		else
		{
			try {
				sItemJsonString_out = Poco::Dynamic::Var(actualItemsWithoutAttachment).toString();
			}
			catch (...)
			{
				Log::error(DATAFW_MODEL_LOG_TAG, "[%s] actualItemsWithoutAttachment convert to string fail when call [%s], items source:%s\r\n", path.data(), __FUNCTION__, sItemJsonString_out.data());
				return false;
			}
			return true;
		}
	}
	else
	{
		Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a %s data when call [%s].", path.data(), type == nullValue ? "valid" : "list", __FUNCTION__);
		return false;
	}
}

bool DataCluster::resizeList(const std::string &path, uint32_t size)
{
	Poco::JSON::Object::Ptr objParent;
	Poco::JSON::Array::Ptr arrayParent;
	ValueType type = m_parser->testPath(path, objParent, arrayParent);
	if (type == listValue)
	{
		if (size > DATAFW_LIST_ITEM_COUNT_LIMIT)
		{
			Log::error(DATAFW_MODEL_LOG_TAG, "[%s] set too big item count when call interface [%s] limited by %d .", path.data(), __FUNCTION__, DATAFW_LIST_ITEM_COUNT_LIMIT);
			return false;
		}

		int nOldSize = arrayParent->size() - 1;
		if (size == nOldSize)
		{
			return false;
		}
		else if (size > nOldSize)
		{
			for (int32_t i = 0; i != size - nOldSize; ++i)
			{
				//不能写成vtRange.push_back(new Poco::JSON::Object());不然解析的时候无法解析成Poco::JSON::Object::Ptr
				Poco::JSON::Object::Ptr pNew = new Poco::JSON::Object();
				arrayParent->add(pNew);
			}
		}
		else
		{
			for (int32_t i = 0; i != nOldSize - size; ++i)
				arrayParent->remove(arrayParent->size() - 1);
		}
		return true;
	}
	else
	{
		Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a %s data when call [%s].", path.data(), type == nullValue ? "valid" : "list", __FUNCTION__);
		return false;
	}
}

bool DataCluster::insertListItems(const std::string &path, uint32_t index, uint32_t count)
{
	if (count == 0)
		return false;

	Poco::JSON::Object::Ptr objParent;
	Poco::JSON::Array::Ptr arrayParent;
	ValueType type = m_parser->testPath(path, objParent, arrayParent);
	if (type == listValue)
	{
		//大于限制则error
		if (arrayParent->size() - 1 + count > DATAFW_LIST_ITEM_COUNT_LIMIT)
		{
			Log::error(DATAFW_MODEL_LOG_TAG, "[%s] set too big item count when call interface [%s] limited by %d .", path.data(), __FUNCTION__, DATAFW_LIST_ITEM_COUNT_LIMIT);
			return false;
		}
		
		//如果是-1，则是append的情况；++index表示第一个元素为模板需+1
		if ((int)index == -1 || ++index >= arrayParent->size())	
			index = arrayParent->size();

		//Poco::JSON::Array::Ptr无插入操作，只能自己处理
		//构建序列，并把新项存入
		std::vector<Poco::JSON::Object::Ptr> vtRange;
		for (int32_t i = 0; i != count; ++i)
		{
			//不能写成vtRange.push_back(new Poco::JSON::Object());不然解析的时候无法解析成Poco::JSON::Object::Ptr
			Poco::JSON::Object::Ptr pNew = new Poco::JSON::Object();
			vtRange.push_back(pNew);
		}

		//把需要移动的items加入序列后段
		for (int32_t i = index; i != arrayParent->size(); ++i)
			vtRange.push_back(arrayParent->getObject(i));

		//把序列重新接入前半段中
		int32_t newSize = arrayParent->size() + count;
		for (int32_t i = index; i != newSize; ++i)
			arrayParent->set(i, vtRange[i - index]);

		return true;
	}
	else
	{
		Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a %s data when call [%s].", path.data(), type == nullValue ? "valid" : "list", __FUNCTION__);
		return false;
	}
}

bool DataCluster::removeListItems(const std::string &path, uint32_t index, uint32_t count)
{
	if (count == 0)
		return false;

	Poco::JSON::Object::Ptr objParent;
	Poco::JSON::Array::Ptr arrayParent;
	ValueType type = m_parser->testPath(path, objParent, arrayParent);
	if (type == listValue)
	{
		if (index >= (arrayParent->size() - 1))
			return false;

		index += 1;
		uint32_t end = (index + count) > arrayParent->size() ? arrayParent->size() : (index + count);

		for (int32_t i = end - 1; i >= index; --i)
			arrayParent->remove(i);

		return true;
	}
	else
	{
		Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a %s data when call [%s].", path.data(), type == nullValue ? "valid" : "list", __FUNCTION__);
		return false;
	}
}

bool DataCluster::updateListViewport(const std::string &path, uint32_t first)
{
	Poco::JSON::Object::Ptr objParent;
	Poco::JSON::Array::Ptr arrayParent;
	ValueType type = m_parser->testPath(path, objParent, arrayParent);
	if (type == listValue)
	{
		return true;
	}
	else
	{
		Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a %s data when call [%s].", path.data(), type == nullValue ? "valid" : "list", __FUNCTION__);
		return false;
	}
}

bool DataCluster::checkListItem(const std::string &path, int32_t index)
{
	Poco::JSON::Object::Ptr objParent;
	Poco::JSON::Array::Ptr arrayParent;
	ValueType type = m_parser->testPath(path, objParent, arrayParent);
	if (type == listValue)
	{
		return true;
	}
	else
	{
		Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a %s data when call [%s].", path.data(), type == nullValue ? "valid" : "list", __FUNCTION__);
		return false;
	}
}

bool DataCluster::focusListItem(const std::string &path, int32_t index)
{
	Poco::JSON::Object::Ptr objParent;
	Poco::JSON::Array::Ptr arrayParent;
	ValueType type = m_parser->testPath(path, objParent, arrayParent);
	if (type == listValue)
	{
		return true;
	}
	else
	{
		Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a %s data when call [%s].", path.data(), type == nullValue ? "valid" : "list", __FUNCTION__);
		return false;
	}
}

std::string DataCluster::getNodeName(const std::string &path)
{
	size_t n = path.rfind('.');
	return n == std::string::npos ? path : path.substr(n + 1);
}

void DataCluster::syncTo(Poco::JSON::Object::Ptr toBeSyncRoot, std::vector<Types::Image> &images, std::vector<Types::ListAttachment> &listattachments)
{
	recurveSyncData(m_parser->getDataRoot(), toBeSyncRoot, images, listattachments, "");
}

void DataCluster::recurveSyncData(Poco::JSON::Object::Ptr native, Poco::JSON::Object::Ptr toBeSync, std::vector<Types::Image> &images, std::vector<Types::ListAttachment> &listattachments, const std::string &sParent)
{
	std::vector<std::string> names;
	toBeSync->getNames(names);
	for (int i = 0; i != names.size(); ++i)
	{
		const std::string &sName = names[i];
		const Poco::Dynamic::Var &vNative = native->get(sName);
		const Poco::Dynamic::Var &vTobeSync = toBeSync->get(sName);
		std::string sCombine = sParent + "." + sName;
		if (vNative.isEmpty())
		{
			Log::warn(DATAFW_MODEL_LOG_TAG, "model has no data named [%s] when sync to view, remove it.", sCombine.substr(1).data());
			toBeSync->remove(sName);
		}
		else if (vNative.type() == typeid(Poco::JSON::Object::Ptr) && vTobeSync.type() == typeid(Poco::JSON::Object::Ptr))
		{
			recurveSyncData(vNative.extract<Poco::JSON::Object::Ptr>(), vTobeSync.extract<Poco::JSON::Object::Ptr>(), images, listattachments, sCombine);
		}
		else
		{
			ValueType nativeType = m_parser->testType(vNative);
			ValueType toBeSyncType = m_parser->testType(vTobeSync);
			if (toBeSyncType != nativeType && (toBeSyncType != intValue && nativeType != realValue))
				Log::warn(DATAFW_MODEL_LOG_TAG, "data[%s] with different type when sync to view, nativeType: %s, toBeSyncType: %s", sCombine.substr(1).data(), toString(nativeType).data(), toString(toBeSyncType).data());

			switch (nativeType)
			{
			case realValue:			toBeSync->set(sName, vNative + DATAFW_REAL_DIFF_VALUE);		break;
			case intValue:
			case booleanValue:
			case enumValue:
			case stringValue:		toBeSync->set(sName, vNative);							break;
			case imageValue:
			{
				Poco::JSON::Array::Ptr array = vNative.extract<Poco::JSON::Array::Ptr>();
				if (array->size() == 2)
				{
					Types::Image img = array->get(1).extract<Types::Image>();
					img.setPath(sCombine.substr(1).data());
					images.push_back(img);
				}
				break;
			}
			case listValue:
			{
				Poco::JSON::Array::Ptr nativeList = vNative.extract<Poco::JSON::Array::Ptr>();
				Poco::JSON::Array::Ptr toBeSyncList = vTobeSync.extract<Poco::JSON::Array::Ptr>();
				Types::ListAttachment listattachment;
				listattachment.setListpath(sCombine.substr(1));
				std::vector<Types::Image> listimages;
				for (int32_t i = 1; i != nativeList->size(); ++i)
				{
					Poco::JSON::Object::Ptr nativeItem = nativeList->getObject(i);
					Poco::JSON::Object::Ptr toBeSyncItem = new Poco::JSON::Object();
					toBeSyncList->add(toBeSyncItem);
					for (Poco::JSON::Object::ConstIterator iter = nativeItem->begin(); iter != nativeItem->end(); ++iter)
					{
						const std::string &sElementName = iter->first;
						const Poco::Dynamic::Var &nativeElement = iter->second;
						switch (m_parser->testType(nativeElement))
						{
						case realValue:			toBeSyncItem->set(sElementName, nativeElement + DATAFW_REAL_DIFF_VALUE);	break;
						case intValue:
						case booleanValue:
						case enumValue:
						case stringValue:		toBeSyncItem->set(sElementName, nativeElement);						break;
						case imageValue:
						{
							Poco::JSON::Array::Ptr img = nativeElement.extract<Poco::JSON::Array::Ptr>();
							listimages.push_back(img->size() == 1 ? Types::Image() : img->get(1).extract<Types::Image>());
							toBeSyncItem->set(sElementName, Parser::makeDefault(imageValue));
							break;
						}
						default:
							Log::warn(DATAFW_MODEL_LOG_TAG, "list data[%s] has unsurpported type[%s] when sync to view", (sCombine.substr(1) + "." + sElementName).data(), nativeElement.type().name());
							break;
						}
					}
				}
				if (!listimages.empty())
				{
					listattachment.setImages(listimages);
					listattachments.push_back(listattachment);
				}
				break;
			}
			default:
				Log::warn(DATAFW_MODEL_LOG_TAG, "model data[%s] has unsurpported type[%s] when sync to view", sCombine.substr(1).data(), vNative.type().name());
				break;
			}
		}
	}
}
