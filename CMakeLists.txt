cmake_minimum_required(VERSION 3.16)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
project(LauraEditor VERSION 1.1.0)

add_subdirectory(Laura)

option(ENABLE_CONSOLE "Enable console" ON)

set(EXECUTABLE_NAME "LauraEditor")
add_executable(${EXECUTABLE_NAME})
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY CXX_STANDARD 20)

file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/LauraEditor/src/*.cpp")
# in case visual studio again adds some files to the EDITOR_SOURCES dont remove the entire line, just the path to the file !!!
target_sources(${EXECUTABLE_NAME} PRIVATE ${EDITOR_SOURCES})
target_include_directories(${EXECUTABLE_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/LauraEditor/src/")

target_link_libraries(${EXECUTABLE_NAME} PUBLIC LauraEngine)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${EXECUTABLE_NAME} PUBLIC EDITOR_RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/LauraEditor/res/")
    message(STATUS "EDITOR_RESOURCES_PATH: ${CMAKE_CURRENT_SOURCE_DIR}/LauraEditor/res/")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_compile_definitions(${EXECUTABLE_NAME} PUBLIC EDITOR_RESOURCES_PATH="./res/")
    message(STATUS "EDITOR_RESOURCES_PATH: ./resources/ (put res folder next to the .exe file)")
else()
	message(!!!! ERROR "Unsupported build type: ${CMAKE_BUILD_TYPE}" !!!!)
endif()


if(MSVC)
	target_compile_definitions(${EXECUTABLE_NAME} PUBLIC _CRT_SECURE_NO_WARNINGS)
	set_property(TARGET ${EXECUTABLE_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	if (NOT ENABLE_CONSOLE)
		set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	endif()
endif()
