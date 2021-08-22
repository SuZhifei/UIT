
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# 1. Project Name

project(libdatafw)

# 2. Project Env (Include/Lib Path, C/CXX/LD FLAGS)
include_directories(${datafw_CAPI_GEN_DIR})

# 3. Project Build

set(LIB_NAME "libdatafw")

file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/include/datafw/*.h" "*.h" "*.cpp")

add_library(${LIB_NAME} SHARED ${SRC_FILES})
set_target_properties(${LIB_NAME} PROPERTIES OUTPUT_NAME uit_datafw)

if(WIN32)
	set(LINK_LIBS uit_core optimized PocoFoundation debug PocoFoundationd optimized PocoJSON debug PocoJSONd ${datafw_CAPI_GEN_LIB} CommonAPI CommonAPI-SomeIP WS2_32)
else()
	set(LINK_LIBS uit_core PocoFoundation PocoJSON PocoUtil PocoNet ${datafw_CAPI_GEN_LIB} CommonAPI CommonAPI-SomeIP)
endif()
target_link_libraries(${LIB_NAME} ${LINK_LIBS})

# 4. Project Install
if(WIN32)
    install(TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR}/debug CONFIGURATIONS debug)
    install(TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR}/release CONFIGURATIONS release)
else()
	install(TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
