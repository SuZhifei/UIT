# 1. Project Name

project(doc)

# 2. Platform Env (Include/Lib Path, C/CXX/LD FLAGS)



# 3. Sub Projects


# 5. Project Install
file(GLOB DOCS *.doc* *.xls* *.ppt* *.pdf)
install(FILES ${DOCS} DESTINATION ${CMAKE_INSTALL_DOCDIR})
