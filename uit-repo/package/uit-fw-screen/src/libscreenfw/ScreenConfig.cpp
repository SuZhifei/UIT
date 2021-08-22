#include "ScreenConfig.h"
#include "Poco/FileStream.h"
#include "Poco/JSON/Parser.h"
#include "uit/core/Log.h"
#include <uit/core/Runtime.h>

#ifdef SCREENFW_LOG_TAG
	#undef SCREENFW_LOG_TAG
#endif

#define SCREENFW_LOG_TAG		"screenfw|config"

using namespace uit::screenfw;
using Poco::JSON::JSONException;

const std::string SCREEN_CONF = uit::Runtime::getUitEtcDirectory() + "uit.screen.conf";
//const std::string SCREEN_CONF = "/svp/etc/uit.screen.conf";
const std::string SCREEN_CONF_VERSION = "1.0";

bool ScreenConfig::loadConf(){
	Poco::FileInputStream file(SCREEN_CONF);

	Poco::JSON::Parser jsonParser;
	try {
		auto jsonResult = jsonParser.parse(file);
		auto jsonObj = jsonResult.extract<Poco::JSON::Object::Ptr>();

		if (!jsonObj->has("screenfw")) {
			uit::Log::info(SCREENFW_LOG_TAG, "Invalid file: %s !", SCREEN_CONF.c_str());
			return false;
		}

		auto var = jsonObj->get("screenfw");
		auto obj = var.extract<Poco::JSON::Object::Ptr>();
		return parseConf(obj);
	} catch (JSONException& jsone) {
		uit::Log::error(SCREENFW_LOG_TAG, "%s", jsone.message().c_str());
		return false;
	}

	return true;
}

bool ScreenConfig::parseConf(Poco::JSON::Object::Ptr obj){
	if (!obj->has("version")){
		uit::Log::error(SCREENFW_LOG_TAG, "No version info!");
		return false;
	}

	std::string version = obj->get("version");
	if (version != SCREEN_CONF_VERSION){
		uit::Log::error(SCREENFW_LOG_TAG, "Unsupported version!");
		return false;
	}

	if (!obj->has("layers")){
		uit::Log::error(SCREENFW_LOG_TAG, "No layers!");
	}

	auto layersVar = obj->get("layers");
	auto layerObj = layersVar.extract<Poco::JSON::Array::Ptr>();

	for(auto it = layerObj->begin(); it != layerObj->end(); it++){
		auto layerSubObj = it->extract<Poco::JSON::Object::Ptr>();
		uint32_t layer_id = layerSubObj->get("id");
		m_layers.push_back(layer_id);
	}
	return true;
}

