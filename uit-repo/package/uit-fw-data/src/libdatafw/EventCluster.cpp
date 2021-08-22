#include "EventCluster.h"
#include "datafw/Parser.h"
#include "Share.h"

using namespace uit::datafw;
void EventCluster::setParser(Parser *parser)
{
	m_parser = parser;
}

bool EventCluster::verify(const std::string &sEventName, Poco::JSON::Object::Ptr &args_in_out)
{
	Poco::JSON::Object::Ptr args = m_parser->findEvent(sEventName);
	if (!args)
		return false;

	Poco::JSON::Object::Ptr actualArgs = new Poco::JSON::Object();
	for (Poco::JSON::Object::ConstIterator iter = args_in_out->begin(); iter != args_in_out->end(); ++iter)
	{
		const std::string &sArg = iter->first;
		const Poco::Dynamic::Var &localArg = args->get(sArg);
		const Poco::Dynamic::Var &sourceArg = iter->second;
		ValueType localType = m_parser->testType(localArg);
		ValueType sourceType = m_parser->testType(sourceArg);
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
					if (localType == booleanValue)		v = sourceArg.convert<bool>();
					else if (localType == intValue)		v = sourceArg.convert<int>();
					else if (localType == realValue)	v = sourceArg.convert<double>();
				}
				catch (...){
					Log::warn(DATAFW_MODEL_LOG_TAG, "event [%s] has no %s arg[%s] when view want to post it, ignore this arg.", sEventName.data(), toString(sourceType).data(), sArg.data());
				}
				actualArgs->set(sArg, sourceArg);
			}
			else
			{
				Log::warn(DATAFW_MODEL_LOG_TAG, "event [%s] has no %s arg[%s] when view want to post it, ignore this arg.", sEventName.data(), toString(sourceType).data(), sArg.data());
			}
			break;
		}
		case stringValue:
		{
			if (sourceType == stringValue)
				actualArgs->set(sArg, sourceArg);
			else
				Log::warn(DATAFW_MODEL_LOG_TAG, "event [%s] has no %s arg[%s] when view want to post it, ignore this arg.", sEventName.data(), toString(sourceType).data(), sArg.data());
			break;
		}
		case enumValue:
		{
			std::string sEnumDefinition;
			std::vector<std::string> stringValues;
			m_parser->extractEnumDefinition(localArg, sEnumDefinition, stringValues);
			if (sourceType == intValue)
			{
				if (sourceArg >= 0 && sourceArg < stringValues.size())
					actualArgs->set(sArg, sourceArg);
				else
					Log::error(DATAFW_MODEL_LOG_TAG, "[%d] is out of range for enum [%s]'s values(0-%d) when call postEvent.", sourceArg.convert<int>(), sEnumDefinition.data(), stringValues.size() - 1);
			}
			else if (sourceType == stringValue)
			{
				int32_t e = m_parser->getEnumIntegerByStringValue(sEnumDefinition, sourceArg.toString());
				if (e != -1)
					actualArgs->set(sArg, sourceArg);
				else
					Log::error(DATAFW_MODEL_LOG_TAG, "[%s] is not a value of enum [%s], you can only use one of [%s] when postEvent.", 
						sourceArg.toString().data(), sEnumDefinition.data(), Poco::cat(std::string("|"), stringValues.begin(), stringValues.end()).data());
			}
			else
			{
				Log::warn(DATAFW_MODEL_LOG_TAG, "event [%s] must set enum arg[%s] with int or string type, ignore this arg.", sEventName.data(), sArg.data());
			}
			break;
		}
		default:
			Log::warn(DATAFW_MODEL_LOG_TAG, "event arg [%s]'s type is [%s], not a surppoted type in item, ignore this arg.", (sEventName + "." + sArg).data(), toString(localType).data());
			break;
		}
	}
	args_in_out = actualArgs;
	if(args_in_out->size() != args->size())
		Log::warn(DATAFW_MODEL_LOG_TAG, "event [%s]'s args are not match-all when view want to post it, origin arg count[%d], used arg count[%d]", sEventName.data(), args->size(), args_in_out->size());

	return true;
}
