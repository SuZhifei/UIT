# 1. Project Name

project(doc)

# 2. Platform Env (Include/Lib Path, C/CXX/LD FLAGS)



# 3. Sub Projects


# 5. Project Install
file(GLOB CONFS *.ini* *.properties *.conf*)
install(FILES ${CONFS} DESTINATION ${CMAKE_INSTALL_ETCDIR})

