#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "vsomeip" for configuration "Release"
set_property(TARGET vsomeip APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vsomeip PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/vsomeip.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/vsomeip.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS vsomeip )
list(APPEND _IMPORT_CHECK_FILES_FOR_vsomeip "${_IMPORT_PREFIX}/lib/vsomeip.lib" "${_IMPORT_PREFIX}/bin/vsomeip.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
