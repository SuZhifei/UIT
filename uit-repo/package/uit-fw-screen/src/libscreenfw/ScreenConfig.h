#pragma once
#include "Poco/JSON/Object.h"

namespace uit {
	namespace screenfw {

		class ScreenConfig
		{
		public:
			bool loadConf();
			std::vector<uint32_t>		m_layers;

		private:
			bool parseConf(Poco::JSON::Object::Ptr obj);

			
		};

		
	}
}
