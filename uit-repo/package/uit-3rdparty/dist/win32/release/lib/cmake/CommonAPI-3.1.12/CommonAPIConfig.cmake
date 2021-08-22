# Config file for the CommonAPI package
# It defines the following variables
# COMMONAPI_INCLUDE_DIRS - include directories for CommonAPI

# Compute paths
get_filename_component(COMMONAPI_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(COMMONAPI_INCLUDE_DIRS "${COMMONAPI_CMAKE_DIR}/../../../include/CommonAPI-3.1")

# Our library dependencies (contains definitions for IMPORTED targets)
include("${COMMONAPI_CMAKE_DIR}/CommonAPITargets.cmake")

set(COMMONAPI_VERSION 3.1.12)
set(COMMONAPI_VERSION_STRING "3.1.12")
