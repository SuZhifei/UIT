# 1. Project Name

project(etc)

# 2. Platform Env (Include/Lib Path, C/CXX/LD FLAGS)



# 3. Sub Projects


# 5. Project Install
file(GLOB CONFS *.ini* *.conf* *.propert* *.json)
install(FILES ${CONFS} DESTINATION ${CMAKE_INSTALL_ETCDIR})
install(DIRECTORY data-event DESTINATION ${CMAKE_INSTALL_ETCDIR})

