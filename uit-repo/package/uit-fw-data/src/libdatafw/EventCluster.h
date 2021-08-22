#pragma once
#include <Poco/JSON/Object.h>
#include "datafw/Parser.h"

namespace uit { namespace datafw {

class EventCluster final
{
public:
	void setParser(Parser *parser);

	bool verify(const std::string &sEventName, Poco::JSON::Object::Ptr &args_in_out);

private:
	Parser						*m_parser;
};

}}