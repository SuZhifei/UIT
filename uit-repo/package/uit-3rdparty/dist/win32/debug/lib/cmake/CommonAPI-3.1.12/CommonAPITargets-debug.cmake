#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CommonAPI" for configuration "Debug"
set_property(TARGET CommonAPI APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(CommonAPI PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/CommonAPI.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/CommonAPI.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS CommonAPI )
list(APPEND _IMPORT_CHECK_FILES_FOR_CommonAPI "${_IMPORT_PREFIX}/lib/CommonAPI.lib" "${_IMPORT_PREFIX}/bin/CommonAPI.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
