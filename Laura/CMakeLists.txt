cmake_minimum_required(VERSION 3.16)

project(LauraEngine)

add_library(LauraEngine STATIC)

file(GLOB_RECURSE LAURA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE LAURA_PLATFORM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/platform/*.cpp")
target_sources(LauraEngine PRIVATE ${LAURA_SOURCES} ${LAURA_PLATFORM_SOURCES})


target_compile_definitions(LauraEngine PUBLIC CORE_RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/") # This is useful to get an ASSETS_PATH in your IDE during development but you should comment this if you compile a release version and uncomment the next line
#target_compile_definitions(LauraEngine PUBLIC CORE_RESOURCES_PATH="./resources/") # Uncomment this line to setup the ASSETS_PATH macro to the final assets directory when you share the game and move the resources to the build folder

target_compile_definitions(LauraEngine PUBLIC APP_RESOURCES_PATH="${CMAKE_SOURCE_DIR}/app/resources/") # This is useful to get an ASSETS_PATH in your IDE during development but you should comment this if you compile a release version and uncomment the next line
#target_compile_definitions(LauraEngine PUBLIC APP_RESOURCES_PATH="./resources/") # Uncomment this line to setup the ASSETS_PATH macro to the final assets directory when you share the game and move the resources to the build folder


target_include_directories(LauraEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
											  "${CMAKE_CURRENT_SOURCE_DIR}"
											  "${CMAKE_SOURCE_DIR}/app/include") # this needs to go later (Laura needs to be independent from the editor)

set(LRPCH_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/lrpch.h")
set(LRPCH_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/lrpch.cpp")
add_library(LauraEnginePCH STATIC ${LRPCH_SOURCE})
target_precompile_headers(LauraEnginePCH PUBLIC ${LRPCH_HEADER})

target_link_libraries(LauraEngine PUBLIC glfw 
										 libglew_static 
										 imgui 
										 glm 
										 deltaLib 
										 assimp 
										 stb_image 
										 spdlog::spdlog
										 LauraEnginePCH
										 )

