
# 0. CMake Defs

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

cmake_policy(VERSION 2.6)

# 1. Project Name

project(simulator)

# 2. Env (Include/Lib Path, C/CXX/LD FLAGS)
#NA

# 4. Sub Projects

# 5. Project Install
if(WIN32 AND NOT MINGW)
	file(GLOB BATS *.bat)
	install(FILES ${BATS} DESTINATION ${CMAKE_INSTALL_BINDIR}/debug CONFIGURATIONS debug)
	install(DIRECTORY simulator DESTINATION ${CMAKE_INSTALL_BINDIR}/debug CONFIGURATIONS debug)
endif()