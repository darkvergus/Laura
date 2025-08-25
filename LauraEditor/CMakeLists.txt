project(LauraEditor VERSION 1.1.0 LANGUAGES CXX)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/imgui-docking)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/implot)

add_executable(LauraEditor)
file(GLOB_RECURSE EDITOR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
target_sources(LauraEditor PRIVATE ${EDITOR_SOURCES})
target_include_directories(LauraEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(LauraEditor PUBLIC imgui implot LauraEngine)

# Define paths to editor resources
target_compile_definitions(LauraEditor PUBLIC
    $<$<BOOL:${BUILD_INSTALL}>:CMAKE_EDITOR_RESOURCES_PATH="./editor_res/">
    $<$<NOT:$<BOOL:${BUILD_INSTALL}>>:CMAKE_EDITOR_RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/">
)

# Ensure Runtime gets built
add_dependencies(LauraEditor LauraRuntime)

set_target_properties(LauraEditor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>
)

# Install Step
install(TARGETS LauraEditor # executable
    RUNTIME DESTINATION .
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res/ # resources
        DESTINATION editor_res)



# Compiler definitions / flags
if(MSVC)
    if (NOT ENABLE_CONSOLE)
        # Build as a GUI app (no console)
        set_target_properties(LauraEditor PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    endif()
endif()

