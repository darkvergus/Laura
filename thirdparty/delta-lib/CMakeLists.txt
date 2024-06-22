cmake_minimum_required(VERSION 3.1)
project(deltaLib)

add_library(deltaLib STATIC)

file(GLOB_RECURSE DELTA_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
target_sources(deltaLib PRIVATE ${DELTA_LIB_SOURCES})

target_include_directories(deltaLib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(deltaLib PUBLIC glfw)