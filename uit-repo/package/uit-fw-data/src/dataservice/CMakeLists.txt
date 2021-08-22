
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# 1. Project Name

project(dataservice)

# 2. Project Env (Include/Lib Path, C/CXX/LD FLAGS)

# 3. Project Build

set(BIN_NAME "dataservice")

file(GLOB SRC_FILES "*.h" "*.cpp")

add_executable(${BIN_NAME} ${SRC_FILES})
set_target_properties(${BIN_NAME} PROPERTIES OUTPUT_NAME uit.svc.data)
target_link_libraries(${BIN_NAME} libdatafw)
if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL x86_64})
	target_link_libraries(${BIN_NAME} dl)
endif()

# 4. Project Install
if(WIN32)
    install(TARGETS ${BIN_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR}/debug CONFIGURATIONS debug)
    install(TARGETS ${BIN_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR}/release CONFIGURATIONS release)
	file(GLOB BATS *.bat)
	install(FILES ${BATS} DESTINATION ${CMAKE_INSTALL_BINDIR}/debug CONFIGURATIONS debug)
	install(FILES ${BATS} DESTINATION ${CMAKE_INSTALL_BINDIR}/release CONFIGURATIONS release)
else()
	install(TARGETS ${BIN_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
