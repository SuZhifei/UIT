#include "core/Runtime.h"
#include <set>
#include <Poco/Path.h>
#include <Poco/File.h>

bool isSecondLevel(const std::string &name)
{
	static std::set<std::string> secondLeves{"lib", "etc", "bin", "temp", "share", "resource", "data"};
	return secondLeves.find(name) != secondLeves.end();
}

std::string uit::Runtime::getUitHomeDirectory()
{
	auto cur = Poco::Path(Poco::Path::current()).makeAbsolute();
	while (cur.depth() > 0 && !isSecondLevel(cur.directory(cur.depth() - 1)))
	{
		cur = cur.parent();
	}
	return cur.parent().toString();
}

std::string uit::Runtime::getUitLibDirectory()
{
	Poco::Path p(getUitHomeDirectory());
	p.pushDirectory("lib");
	return p.toString();
}

std::string uit::Runtime::getUitEtcDirectory()
{
	Poco::Path p(getUitHomeDirectory());
	p.pushDirectory("etc");
	return p.toString();
}

std::string uit::Runtime::getUitBinDirectory()
{
	Poco::Path p(getUitHomeDirectory());
	p.pushDirectory("bin");
	return p.toString();
}

std::string uit::Runtime::getUitTempDirectory()
{
	Poco::Path p(getUitHomeDirectory());
	p.pushDirectory("temp");
	return p.toString();
}

std::string uit::Runtime::getUitShareDirectory()
{
	Poco::Path p(getUitHomeDirectory());
	p.pushDirectory("share");
	return p.toString();
}

std::string uit::Runtime::getUitResourceDirectory()
{
	Poco::Path p(getUitHomeDirectory());
	p.pushDirectory("resource");
	return p.toString();
}

std::string uit::Runtime::getUitDataDirectory()
{
	Poco::Path p(getUitHomeDirectory());
	p.pushDirectory("data");
	return p.toString();
}
