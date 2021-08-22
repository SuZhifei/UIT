# 0. CMake Defs
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_policy(VERSION 2.6)

# 1. Project Name
project(uit-3rdparty)

# 2. Env (Include/Lib Path, C/CXX/LD FLAGS)
if(WIN32)
	install(DIRECTORY dist/win32/release/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
	install(DIRECTORY dist/win32/release/etc/ DESTINATION ${CMAKE_INSTALL_ETCDIR})
	install(DIRECTORY dist/win32/debug/lib/ DESTINATION ${CMAKE_INSTALL_LIBDIR}/debug)
	install(DIRECTORY dist/win32/release/lib/ DESTINATION ${CMAKE_INSTALL_LIBDIR}/release)
	file(GLOB LIBS_DEBUG dist/win32/debug/bin/*.dll CONFIGURATIONS debug)
	file(GLOB LIBS_RELEASE dist/win32/release/bin/*.dll CONFIGURATIONS release)
	file(GLOB BINS_DEBUG dist/win32/debug/bin/* CONFIGURATIONS debug)
	file(GLOB BINS_RELEASE dist/win32/release/bin/* CONFIGURATIONS release)
	install(FILES ${LIBS_DEBUG} DESTINATION ${CMAKE_INSTALL_LIBDIR}/debug CONFIGURATIONS debug)
	install(FILES ${LIBS_RELEASE} DESTINATION ${CMAKE_INSTALL_LIBDIR}/release CONFIGURATIONS release)
	install(FILES ${BINS_DEBUG} DESTINATION ${CMAKE_INSTALL_BINDIR}/debug CONFIGURATIONS debug)
	install(FILES ${BINS_RELEASE} DESTINATION ${CMAKE_INSTALL_BINDIR}/release CONFIGURATIONS release)
else()
	install(DIRECTORY dist/$ENV{UIT_TARGET}/release/ DESTINATION ${CMAKE_INSTALL_PREFIX} USE_SOURCE_PERMISSIONS)
endif()

# 4. Sub Projects

#add_subdirectory(capicxx-core-runtime)
#add_subdirectory(vsomeip)
#add_subdirectory(capicxx-someip-runtime)

# 5. Project Install
