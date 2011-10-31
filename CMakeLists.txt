project(worm)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(WORM_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/src 
${CMAKE_SOURCE_DIR}/src/sql
${CMAKE_SOURCE_DIR}/src/sql/drivers
)

INCLUDE(build/FindMySQL.cmake)

find_package(Boost)

#ADD_DEFINITIONS(-Wall -O2)
ADD_DEFINITIONS(-Wall -g)

include_directories( ${Boost_INCLUDE_DIRS} ${WORM_INCLUDE_DIRS})

add_subdirectory(src)
install(FILES src/orm/templates/class_declaration.tpl DESTINATION share/worm)
