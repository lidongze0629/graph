cmake_minimum_required(VERSION 3.5)
project(JNI_Grape)

set(CMAKE_C_STANDARD 11)

include_directories(graph)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")

file(GLOB_RECURSE CORE_SRC_FILES "*.cc")
add_library(graph_core SHARED ${CORE_SRC_FILES})

add_executable(graph-engine main.cc)
target_link_libraries(graph-engine graph_core)
set_target_properties(graph-engine PROPERTIES LINK_FLAGS "-fPIC -rdynamic")