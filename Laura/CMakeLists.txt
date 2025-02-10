cmake_minimum_required(VERSION 3.16)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
project(LauraEngine VERSION 1.1.0)

### BUILD OPTIONS ###

########################################
### SELECT A PLATFORM TO COMPILE FOR ###
########################################

option(LR_PLATFORM_WINDOWS "Enable Windows platform support" ON)
option(LR_PLATFORM_LINUX "Enable Linux platform support (currently unsupported)" OFF)

# CMake compatible thirdparty libs: Thanks to https://github.com/meemknight/cmakeSetup
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(ONLY_LIBS ON)								# GLEW wont generate executable targets
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/glfw-3.3.2")			# Window creation
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/glew-cmake")			# OpenGL loader
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/stb_image")			# Loading textures/images
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/stb_truetype")		    # Loading ttf fonts
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/glm")				    # Math
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/imgui-docking")		# UI
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/assimp")				# Model loading
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/spdlog")				# Logging/Debugging
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/entt")				    # ECS
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/font-awesome-headers") # Icons
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/yaml-cpp")             # YAML Serialization
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/implot")                # Graphs and plotting

add_library(LauraEngine STATIC)
set_property(TARGET LauraEngine PROPERTY CXX_STANDARD 20)
file(GLOB_RECURSE LAURA_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
target_sources(LauraEngine PRIVATE ${LAURA_SOURCES})
target_include_directories(LauraEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")

# Setting up the Precompiled Header
add_library(lrpchLib STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/lrpch.cpp")
target_precompile_headers(lrpchLib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/lrpch.h")
target_link_libraries(lrpchLib PUBLIC glm spdlog::spdlog)

# only these should be public
target_link_libraries(LauraEngine PUBLIC glm imgui spdlog::spdlog entt fontAwesome yaml-cpp implot) 
# these should be private but it does not work with private for some reason
target_link_libraries(LauraEngine PUBLIC assimp glfw libglew_static stb_image stb_truetype) 

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(LauraEngine PUBLIC LR_RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/res/")
    message(STATUS "LR_RESOURCES_PATH: ${CMAKE_CURRENT_SOURCE_DIR}/src/res/")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_compile_definitions(LauraEngine PUBLIC LR_RESOURCES_PATH="./res/")
    message(STATUS "LR_RESOURCES_PATH: ./resources/ (put res folder next to the .exe file)")
else()
	message(!!!! ERROR "Unsupported build type: ${CMAKE_BUILD_TYPE}" !!!!)
endif()


if(LR_PLATFORM_WINDOWS)
    target_compile_definitions(LauraEngine PUBLIC LR_PLATFORM_WINDOWS)
endif()
if(LR_PLATFORM_LINUX)
    message(WARNING "LR_PLATFORM_LINUX is currently unsupported.")
    # target_compile_definitions(LauraEngine PUBLIC LR_PLATFORM_LINUX)
endif()

