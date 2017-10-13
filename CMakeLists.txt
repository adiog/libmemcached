cmake_minimum_required(VERSION 3.2)
project(libmemcached)

#set(CMAKE_CXX_COMPILER /usr/bin/g++-5)
#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "")
#ADD_DEFINITIONS("")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libmemcached-1.0)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

configure_file(mem_config.in mem_config.h)

FILE(GLOB_RECURSE PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/libmemcached/**.cc)
ADD_LIBRARY(libmemcached ${PROJECT_SOURCES})
#TARGET_LINK_LIBRARIES(libmemcached dl antlr4-runtime)


