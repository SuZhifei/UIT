
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# 1. Project Name

project(libscreenfw)

# 2. Project Env (Include/Lib Path, C/CXX/LD FLAGS)
include_directories(${screenfw_CAPI_GEN_DIR})

# 3. Project Build

set(LIB_NAME "libscreenfw")

file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/include/screenfw/*.h" "*.h" "*.cpp")

add_library(${LIB_NAME} SHARED ${SRC_FILES})
set_target_properties(${LIB_NAME} PROPERTIES OUTPUT_NAME uit_screenfw)

if(WIN32)
	set(LINK_LIBS uit_core optimized PocoFoundation debug PocoFoundationd optimized PocoJSON debug PocoJSONd ${screenfw_CAPI_GEN_LIB} CommonAPI CommonAPI-SomeIP WS2_32)
else()
	set(LINK_LIBS uit_core screen PocoFoundation PocoJSON PocoUtil PocoNet ${screenfw_CAPI_GEN_LIB} CommonAPI CommonAPI-SomeIP)
endif()
target_link_libraries(${LIB_NAME} ${LINK_LIBS})

# 4. Project Install
if(WIN32)
    install(TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR}/debug CONFIGURATIONS debug)
    install(TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR}/release CONFIGURATIONS release)
else()
	file(GLOB CONFS conf/*.conf)
	install(FILES ${CONFS} DESTINATION ${CMAKE_INSTALL_ETCDIR})
	install(TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
