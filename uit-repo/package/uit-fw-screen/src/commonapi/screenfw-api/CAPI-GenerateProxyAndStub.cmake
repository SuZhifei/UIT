
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
            COMMAND $ENV{CAPI_CORE_GEN} -d ${CAPI_GEN_DIR} -sk ${_fidl}
            RESULT_VARIABLE _result
        )

        if(_result)
             message(FATAL_ERROR "[${PROJECT_NAME}] - CAPI_CORE_GEN fail on ${_fidl}")
        endif()
    endforeach()
endfunction()

capi_check_cache()
message(STATUS "[${PROJECT_NAME}] - ENABLE_CAPI_GEN: ${ENABLE_CAPI_GEN}")

if (ENABLE_CAPI_GEN)
    capi_gen_core()
    capi_update_cache()
endif()

# Env (Include/Lib Path, C/CXX/LD FLAGS)

include_directories(
    ${COMMONAPI_INCLUDE_DIRS}
    ${CAPI_GEN_DIR}
)

# Build
set(CAPI_LIB_NAME uit_${PROJECT_NAME}-core)

FILE(GLOB_RECURSE CAPI_LIB_SRCS ${CAPI_GEN_DIR}/*.cpp)
#message(STATUS "CAPI_LIB_SRCS: ${CAPI_LIB_SRCS}")

add_library (${CAPI_LIB_NAME} ${CAPI_LIB_SRCS})

target_link_libraries(${CAPI_LIB_NAME})

# Export
set(${PROJECT_NAME}_CAPI_GEN_DIR ${CAPI_GEN_DIR} CACHE PATH "CAPI Generated Code Dir")
set(${PROJECT_NAME}_CAPI_GEN_LIB ${CAPI_LIB_NAME} CACHE PATH "CAPI Generated Stub Lib Name")
