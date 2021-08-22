#----------------------------------------------------------------
# Generated CMake target import file for configuration "release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CommonAPI" for configuration "release"
set_property(TARGET CommonAPI APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(CommonAPI PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libCommonAPI.so.3.1.12"
  IMPORTED_SONAME_RELEASE "libCommonAPI.so.3.1.12"
  )

list(APPEND _IMPORT_CHECK_TARGETS CommonAPI )
list(APPEND _IMPORT_CHECK_FILES_FOR_CommonAPI "${_IMPORT_PREFIX}/lib/libCommonAPI.so.3.1.12" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
