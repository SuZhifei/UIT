#pragma once
#include "Def.h"

namespace uit {

class UIT_API Runtime
{
public:
	//获取运行时uit的home目录，在
	static std::string getUitHomeDirectory();

	//获取运行时uit的lib目录
	static std::string getUitLibDirectory();

	//获取运行时uit的配置目录
	static std::string getUitEtcDirectory();

	//获取运行时uit的bin目录
	static std::string getUitBinDirectory();

	//获取运行时uit的临时目录
	static std::string getUitTempDirectory();

	//获取运行时uit共享目录
	static std::string getUitShareDirectory();

	//获取运行时uit资源目录
	static std::string getUitResourceDirectory();

	//获取运行时数据目录
	static std::string getUitDataDirectory();
};

}