#pragma once
#include <string>
#include <Poco/JSON/Object.h>
#include <v1/uit/datafw/Types.hpp>
#include "datafw/Parser.h"

using namespace v1::uit::datafw;
namespace uit { namespace datafw {

class DataCluster final
{
public:
	void setParser(Parser *parser);

	bool updateBoolean(const std::string &path, bool b);
	bool updateInteger(const std::string &path, int32_t n);
	bool updateEnum(const std::string &path, int32_t e);
	bool updateEnumStringValue(const std::string &path, const std::string sEnumStringValue);
	bool updateReal(const std::string &path, double d);
	bool updateString(const std::string &path, const std::string &str);
	bool updateImage(const std::string &path, const Types::Image &image);
	bool updateGroup(const std::vector<std::string> &paths, Poco::JSON::Array::Ptr values, std::vector<std::string> &paths_out, Poco::JSON::Array::Ptr values_out);

	bool updateListItems(const std::string &path, const std::vector<uint32_t> &indexs, const std::string &sItemJsonString, const std::vector<Types::Image> &images, std::vector<uint32_t> &indexs_out, std::string &sItemJsonString_out, std::vector<Types::Image> &images_out);
	bool resizeList(const std::string &path, uint32_t size);
	bool insertListItems(const std::string &path, uint32_t index, uint32_t count);
	bool removeListItems(const std::string &path, uint32_t index, uint32_t count);
	bool updateListViewport(const std::string &path, uint32_t first);
	bool checkListItem(const std::string &path, int32_t index);
	bool focusListItem(const std::string &path, int32_t index);

public:
	void syncTo(Poco::JSON::Object::Ptr toBeSyncRoot, std::vector<Types::Image> &images, std::vector<Types::ListAttachment> &listattachments);

private:
	std::string getNodeName(const std::string &path);
	void recurveSyncData(Poco::JSON::Object::Ptr native, Poco::JSON::Object::Ptr toBeSyncRoot, std::vector<Types::Image> &images, std::vector<Types::ListAttachment> &listattachments, const std::string &sParent);

private:
	Parser						*m_parser;
};

}}