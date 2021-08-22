#----------------------------------------------------------------
# Generated CMake target import file for configuration "release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "vsomeip" for configuration "release"
set_property(TARGET vsomeip APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vsomeip PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvsomeip.so.2.10.21"
  IMPORTED_SONAME_RELEASE "libvsomeip.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS vsomeip )
list(APPEND _IMPORT_CHECK_FILES_FOR_vsomeip "${_IMPORT_PREFIX}/lib/libvsomeip.so.2.10.21" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
