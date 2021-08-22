# 1. Project Name

project(commonapi)

# 2. Platform Env (Include/Lib Path, C/CXX/LD FLAGS)
add_definitions("-DDEFAULT_SEND_TIMEOUT=10000")
if(NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
set(CAPI_FIDL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fidl)

# 3. Sub Projects
add_subdirectory(datafw-someip)
add_subdirectory(datafw-api)
