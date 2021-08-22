
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

set(ENABLE_CAPI_GEN FALSE)

set(CAPI_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/capi-gen)
#message(STATUS "CAPI_GEN_DIR: ${CAPI_GEN_DIR}")

set(CAPI_MD5_CACHE_FILE ${CMAKE_CURRENT_BINARY_DIR}/capi-cache.md5)

function(capi_check_cache)
    if(NOT (EXISTS ${CAPI_MD5_CACHE_FILE}))
        set(ENABLE_CAPI_GEN TRUE PARENT_SCOPE)
    else()
        set(_capi_md5_offset 0)
        foreach(_capi ${CAPI_FIDLS} ${CAPI_DEPENDS})
            FILE(MD5 ${_capi} _capi_md5)
            FILE(READ ${CAPI_MD5_CACHE_FILE} _cache_capi_md5 LIMIT 33 OFFSET ${_capi_md5_offset})
            STRING(STRIP ${_cache_capi_md5} _cache_capi_md5)
            MATH(EXPR _capi_md5_offset "${_capi_md5_offset} + 33")
            if(NOT (${_capi_md5} STREQUAL ${_cache_capi_md5}))
                set(ENABLE_CAPI_GEN TRUE PARENT_SCOPE)
                break()
            endif()
        endforeach()
    endif()
endfunction()

function(capi_update_cache)
    FILE(REMOVE ${CAPI_MD5_CACHE_FILE})
    foreach(_capi ${CAPI_FIDLS} ${CAPI_DEPENDS})
        FILE(MD5 ${_capi} _capi_md5)
        FILE(APPEND ${CAPI_MD5_CACHE_FILE} "${_capi_md5} ")
    endforeach()
endfunction()

function(capi_gen_core)
    foreach(_fidl ${CAPI_FIDLS})
        execute_process(
            COMMAND $ENV{CAPI_CORE_GEN} -d ${CAPI_GEN_DIR} ${_fidl}
            RESULT_VARIABLE _result
        )

        if(_result)
             message(FATAL_ERROR "[${PROJECT_NAME}] - CAPI_CORE_GEN fail on ${_fidl}")
        endif()
    endforeach()
endfunction()

function(capi_gen_someip)
    foreach(_fidl ${CAPI_FIDLS})
        string(REPLACE ".fidl" ".fdepl" _fdepl ${_fidl})
        execute_process(
            COMMAND $ENV{CAPI_SOMEIP_GEN} -d ${CAPI_GEN_DIR} ${_fdepl}
            RESULT_VARIABLE _result
        )

        if(_result)
             message(FATAL_ERROR "[${PROJECT_NAME}] - CAPI_SOMEIP_GEN fail on ${_fidl}")
        endif()
    endforeach()
endfunction()

capi_check_cache()
message(STATUS "[${PROJECT_NAME}] - ENABLE_CAPI_GEN: ${ENABLE_CAPI_GEN}")

if (ENABLE_CAPI_GEN)
    capi_gen_core()
    capi_gen_someip()
    capi_update_cache()
endif()

# Env (Include/Lib Path, C/CXX/LD FLAGS)

include_directories(
    ${CAPI_GEN_DIR}
    ${COMMONAPI_SOMEIP_INCLUDE_DIRS}
    ${VSOMEIP_INCLUDE_DIRS}
    ${COMMONAPI_INCLUDE_DIRS}
)

link_directories(
    ${COMMONAPI_SOMEIP_LIBDIR}
)

# Build

FILE(GLOB_RECURSE CAPI_LIB_SRCS ${CAPI_GEN_DIR}/*.cpp)
#message(STATUS "CAPI_LIB_SRCS: ${CAPI_LIB_SRCS}")

add_library (${CAPI_LIB_NAME}-someip SHARED
        ${CAPI_LIB_SRCS}
        )

target_link_libraries(${CAPI_LIB_NAME}-someip CommonAPI CommonAPI-SomeIP)

# Install
if(WIN32)
    install(TARGETS ${CAPI_LIB_NAME}-someip DESTINATION ${CMAKE_INSTALL_LIBDIR}/debug CONFIGURATIONS debug)
    install(TARGETS ${CAPI_LIB_NAME}-someip DESTINATION ${CMAKE_INSTALL_LIBDIR}/release CONFIGURATIONS release)
else()
	install(TARGETS ${CAPI_LIB_NAME}-someip DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()