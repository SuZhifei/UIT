# 1. Project Name
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_policy(VERSION 2.6)

project(uit-env)

# 2. Platform Env (Include/Lib Path, C/CXX/LD FLAGS)



# 3. Sub Projects


# 5. Project Install
install(DIRECTORY $ENV{UIT_TARGET}/ DESTINATION ${CMAKE_INSTALL_ETCDIR} USE_SOURCE_PERMISSIONS)

