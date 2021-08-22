# 0. CMake Defs
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_policy(VERSION 2.6)

# 1. Project Name
project(uit-lib-core)

# 2. Env (Include/Lib Path, C/CXX/LD FLAGS)

include_directories(include)
include_directories(${CMAKE_INSTALL_INCLUDEDIR})
link_directories(${CMAKE_INSTALL_LIBDIR})

# 4. Sub Projects

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(etc)
add_subdirectory(example)

# 5. Project Install
