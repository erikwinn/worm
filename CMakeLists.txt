project(worm)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(WORM_INCLUDE_DIRS
${CMAKE_SOURCE_DIR}/src 
${CMAKE_SOURCE_DIR}/src/sql
${CMAKE_SOURCE_DIR}/src/orm
${CMAKE_SOURCE_DIR}/src/sql/drivers
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(MySQL)
find_package(Sqlite3)
find_package(Ctemplate REQUIRED)
find_package(Boost REQUIRED)

#ADD_DEFINITIONS(-Wall -O2)
ADD_DEFINITIONS(-Wall -g)

include_directories( ${Boost_INCLUDE_DIRS} ${WORM_INCLUDE_DIRS} ${SQLITE3_INCLUDE_DIR})

add_subdirectory(src)
install(FILES src/orm/templates/class_declaration.tpl DESTINATION share/worm)
