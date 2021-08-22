/*此文件有#define，include需谨慎*/
#pragma once
#include <memory>
#include <CommonAPI/CommonAPI.hpp>
#include <uit/core/Log.h>
#include <uit/core/Runtime.h>

//commonapi.ini配置服务的格式为:
// [domain]:[fidl package].[fidl Server name]:[fidl version]:[server instance]
//local:uit.datafw.ControllerServer:v1_0:uit.datafw.controller = uit_datafw-someip.dll

#define COMMONAPI_CONFIG_ENV					std::string("COMMONAPI_CONFIG=" + uit::Runtime::getUitEtcDirectory() + "commonapi.ini").data()
#define VSOMEIP_CONFIGURATION_ENV				std::string("VSOMEIP_CONFIGURATION=" + uit::Runtime::getUitEtcDirectory() + "vsomeip-dataservice.json").data()
#define COMMONAPI_LIBRARY_BASE_PROPERTY			"LibraryBase"					//固定的，不可修改
#define	UIT_SERVER_DOMAIN						"local"							//对应commonapi.ini中的domain
#define UIT_DATAFW_ROUTING						"uit.svc.data"					//对应vsomeip.json中routing字段的值（该字段也可以指定为vsomeipd）
#define UIT_VIEW_SERVER_INSTANCE				"uit.datafw.view"				//对应commonapi.ini的server instance
#define UIT_CONTROLLER_SERVER_INSTANCE			"uit.datafw.controller"			//对应commonapi.ini的server instance
#define UIT_DATAFW_LIBBASE						"uit_datafw"					//*-dbus(someip).so(dll)的名字的中间部分（暂时不知该值是否有对应字段）
#define DATAFW_ENUM_DEFINITION_FILE				"EnumDefinition.json"
#define DATAFW_ITEMTEMPLATE_DEFINITION_FILE		"ItemTemplate.json"
#define DATAFW_DATA_DEFINITION_FILE				"Data.json"
#define DATAFW_EVENT_DEFINITION_FILE			"Event.json"
#define DATAFW_IMAGE_DEFINITION_STRING			"Image"
#define DATAFW_ENUM_DEFINITION_POSTFIX			"Enum"
#define DATAFW_ITEMTEMPLATE_DEFINITION_POSTFIX	"ItemTemplate"
#define DATAFW_MODEL_LOG_TAG					"datafw|model"
#define DATAFW_VIEW_LOG_TAG						"datafw|view"
#define DATAFW_CONTROLLER_LOG_TAG				"datafw|controller"
#define DATAFW_PARSER_LOG_TAG					"datafw|parser"
#define DATAFW_SOCKETDEBUGGER_LOG_TAG			"datafw|socket-helper"
#define DATAFW_SERVER_INIT_TIMEOUT				5 * 1000
#define DATAFW_CONTROLLER_CONNECT_TIMEOUT		5 * 1000
#define DATAFW_VIEW_CONNECT_TIMEOUT				5 * 1000
#define DATAFW_WAIT_SLEEP_STRIP					10
#define	DATAFW_STRING_SIZE_LIMIT				5 * 1024 * 1024
#define	DATAFW_IMAGE_SIZE_LIMIT					5 * 1024 * 1024
#define DATAFW_LIST_ITEM_COUNT_LIMIT			100 * 1000
#define DATAFW_REAL_DIFF_VALUE					0.000001

namespace uit { namespace datafw{

static void initEnv()
{
	auto commonapi_ini = getenv("COMMONAPI_CONFIG");
	auto vsomeip_json = getenv("VSOMEIP_CONFIGURATION");
	if (!commonapi_ini)
		putenv((char *)(COMMONAPI_CONFIG_ENV));
	if (!vsomeip_json)
		putenv((char *)(VSOMEIP_CONFIGURATION_ENV));
}

template<typename ServerImpT>
bool registerServer(std::shared_ptr<ServerImpT> serverImp, const std::string &domain, const std::string &instance, const std::string &libbase, uint32_t timeout, uint32_t strip = DATAFW_WAIT_SLEEP_STRIP)
{
	int32_t nTimeRemain = timeout;
	while (!CommonAPI::Runtime::get()->registerService(domain, instance, serverImp, UIT_DATAFW_ROUTING))
	{
		if (nTimeRemain <= 0)
			return false;

		std::this_thread::sleep_for(std::chrono::milliseconds(strip < nTimeRemain ? strip : nTimeRemain));
		nTimeRemain -= strip;
	}
	return true;
}

//建立客户端代理（可变参数模板）
template<template<typename ...> class proxyT, typename ... AttrT >
std::shared_ptr< proxyT<AttrT...> > buildProxy(const std::string &domain, const std::string &instance, const std::string &libbase, uint32_t timeout, bool &bTimeout, uint32_t strip = DATAFW_WAIT_SLEEP_STRIP)
{
	auto proxy = CommonAPI::Runtime::get()->buildProxy<proxyT, AttrT...>(domain, instance, "proxy.connection");
	if (!proxy)
	{
		bTimeout = false;
		return nullptr;
	}

	int32_t nTimeRemain = timeout;
	while (!proxy->isAvailable())
	{
		if (nTimeRemain <= 0)
		{
			bTimeout = true;
			return nullptr;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(strip < nTimeRemain ? strip : nTimeRemain));
		nTimeRemain -= strip;
	}
	bTimeout = false;
	return proxy;
}


static void onCallErrorCallback(const CommonAPI::CallStatus &callStatus, const std::string &funcName)
{
	switch (callStatus)
	{
	case CommonAPI::CallStatus::SUCCESS:				break;
	case CommonAPI::CallStatus::OUT_OF_MEMORY:			uit::Log::error(DATAFW_CONTROLLER_LOG_TAG, "proxy call [%s] error, out_of_memory.", funcName.data());							break;
	case CommonAPI::CallStatus::CONNECTION_FAILED:		uit::Log::error(DATAFW_CONTROLLER_LOG_TAG, "proxy call [%s] error, connection_failed.", funcName.data());						break;
	case CommonAPI::CallStatus::NOT_AVAILABLE:			uit::Log::error(DATAFW_CONTROLLER_LOG_TAG, "proxy call [%s] error, not_available, maybe model is dead.", funcName.data());		break;
	case CommonAPI::CallStatus::REMOTE_ERROR:			uit::Log::error(DATAFW_CONTROLLER_LOG_TAG, "proxy call [%s] error, remote_error, maybe model is too busy.", funcName.data());	break;
	case CommonAPI::CallStatus::INVALID_VALUE:			uit::Log::error(DATAFW_CONTROLLER_LOG_TAG, "proxy call [%s] error, invalid_value.", funcName.data());							break;
	case CommonAPI::CallStatus::SUBSCRIPTION_REFUSED:	uit::Log::error(DATAFW_CONTROLLER_LOG_TAG, "proxy call [%s] error, subscripion_refused.", funcName.data());					break;
	case CommonAPI::CallStatus::UNKNOWN:				uit::Log::error(DATAFW_CONTROLLER_LOG_TAG, "proxy call [%s] error, unknown reson.", funcName.data());							break;
	default:											uit::Log::error(DATAFW_CONTROLLER_LOG_TAG, "proxy call [%s] error, default.", funcName.data());								break;
	}
}

class JsonException
{
};

}}
