# 1. Project Name

project(resource)

# 2. Platform Env (Include/Lib Path, C/CXX/LD FLAGS)



# 3. Sub Projects


# 5. Project Install
file(GLOB RES *.jpg)
install(FILES ${RES} DESTINATION ${CMAKE_INSTALL_RESOURCEDIR}/dataservice)

