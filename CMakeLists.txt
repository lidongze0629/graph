cmake_minimum_required(VERSION 3.5)
project(JNI_Grape)

set(CMAKE_C_STANDARD 11)

include_directories(graph)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")

# --------------------------------------
# find JNI
# --------------------------------------

find_package(JNI)
if (JNI_FOUND)
    message(STATUS "JNI found")
    include_directories(SYSTEM ${JAVA_INCLUDE_PATH})
    include_directories(SYSTEM ${JAVA_INCLUDE_PATH2})
else()
    message(STATUS "JNI not found")
    set(JAVA_JVM_LIBRARY "")
endif()

file(GLOB_RECURSE CORE_SRC_FILES "graph/*.cc")
add_library(graph_core SHARED ${CORE_SRC_FILES})

if (JNI_FOUND)
    target_link_libraries(graph_core ${JAVA_JVM_LIBRARY})
endif()

add_executable(graph-engine jsssp.cc)
target_link_libraries(graph-engine graph_core)
set_target_properties(graph-engine PROPERTIES LINK_FLAGS "-fPIC -rdynamic")

add_executable(sssp sssp.cc)
target_link_libraries(sssp graph_core)
set_target_properties(sssp PROPERTIES LINK_FLAGS "-fPIC -rdynamic")