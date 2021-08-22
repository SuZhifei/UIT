# Config file for the CommonAPI-SomeIP package
# It defines the following variables
#   COMMONAPI_SOMEIP_INCLUDE_DIRS - include directories for CommonAPI-SomeIP

# Compute paths
get_filename_component (COMMONAPI_SOMEIP_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set (COMMONAPI_SOMEIP_INCLUDE_DIRS "${COMMONAPI_SOMEIP_CMAKE_DIR}/../../../include/CommonAPI-3.1")

include ("${COMMONAPI_SOMEIP_CMAKE_DIR}/CommonAPI-SomeIPTargets.cmake")

set(COMMONAPI_SOMEIP_VERSION 3.1.12)
set(COMMONAPI_SOMEIP_VERSION_STRING "3.1.12")
