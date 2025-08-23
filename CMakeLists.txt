cmake_minimum_required(VERSION 3.2...3.20)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
project(LauraEditor VERSION 1.1.0)

# LAURA EDITOR TARGET ///////////////////
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/LauraEditor/Thirdparty/imgui-docking") # UI
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/LauraEditor/Thirdparty/implot") # Graphs and plotting
add_subdirectory(Laura)
option(ENABLE_CONSOLE "Enable console" ON)
set(EXECUTABLE_NAME "LauraEditor")
add_executable(${EXECUTABLE_NAME})
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY CXX_STANDARD 20)
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/LauraEditor/src/*.cpp")
# in case visual studio again adds some files to the EDITOR_SOURCES dont remove the entire line, just the path to the file !!!
target_sources(${EXECUTABLE_NAME} PRIVATE ${EDITOR_SOURCES})
target_include_directories(${EXECUTABLE_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/LauraEditor/src/")
target_link_libraries(${EXECUTABLE_NAME} PUBLIC imgui implot LauraEngine)

target_compile_definitions(${EXECUTABLE_NAME} PUBLIC
    $<$<CONFIG:Debug>:EDITOR_RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/LauraEditor/res/">
    $<$<CONFIG:RelWithDebInfo>:EDITOR_RESOURCES_PATH="./res/">
)

if(MSVC)
	target_compile_definitions(${EXECUTABLE_NAME} PUBLIC _CRT_SECURE_NO_WARNINGS)
	set_property(TARGET ${EXECUTABLE_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	if (NOT ENABLE_CONSOLE)
		set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	endif()
endif()

# LAURA RUNTIME TARGET ///////////////////
set(EXECUTABLE_NAME "LauraRuntime")
add_executable(${EXECUTABLE_NAME})
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY CXX_STANDARD 20)
file(GLOB_RECURSE RUNTIME_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/LauraRuntime/src/*.cpp")
target_sources(${EXECUTABLE_NAME} PRIVATE ${RUNTIME_SOURCES})
target_include_directories(${EXECUTABLE_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/LauraRuntime/src/")
target_link_libraries(${EXECUTABLE_NAME} PUBLIC LauraEngine)

target_compile_definitions(${EXECUTABLE_NAME} PUBLIC RUNTIME_PROJECTFILE_PATH="C:/Users/X13JG/Desktop/project1/project1.lrproj")

if(MSVC)
	target_compile_definitions(${EXECUTABLE_NAME} PUBLIC _CRT_SECURE_NO_WARNINGS)
	set_property(TARGET ${EXECUTABLE_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	if (NOT ENABLE_CONSOLE)
		set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	endif()
endif()
