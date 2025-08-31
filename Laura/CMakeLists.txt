project(LauraEngine VERSION 1.1.0 LANGUAGES CXX)

# ---- Third-party libraries ----
# Disable unnecessary GLFW components and add libraries
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(ONLY_LIBS ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/glew-cmake)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/stb_image)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/stb_truetype)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/glm)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/assimp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/spdlog)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/entt)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/yaml-cpp)

# ---- Engine library target ----
add_library(LauraEngine STATIC)
set_target_properties(LauraEngine PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
target_compile_features(LauraEngine PUBLIC cxx_std_20)  # Require C++20:contentReference[oaicite:8]{index=8}
target_include_directories(LauraEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src )
file(GLOB_RECURSE LAURA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
target_sources(LauraEngine PRIVATE ${LAURA_SOURCES})

# Optionally, set up precompiled header directly on the target (CMake ≥3.16)
target_precompile_headers(LauraEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/lrpch.h")  # see CMake docs:contentReference[oaicite:9]{index=9}

target_link_libraries(LauraEngine PUBLIC glm spdlog::spdlog entt yaml-cpp) 
target_link_libraries(LauraEngine PUBLIC assimp glfw libglew_static stb_image stb_truetype) 


# DEFINES 
target_compile_definitions(LauraEngine PUBLIC
    $<$<BOOL:${BUILD_INSTALL}>:BUILD_INSTALL> # if ON -> #define BUILD_INSTALL
)
target_compile_definitions(LauraEngine PUBLIC
    $<$<BOOL:${BUILD_INSTALL}>:CMAKE_ENGINE_RESOURCES_PATH="./engine_res/">
    $<$<NOT:$<BOOL:${BUILD_INSTALL}>>:CMAKE_ENGINE_RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/">
)
if(WIN32)
    target_compile_definitions(LauraEngine PUBLIC LR_PLATFORM_WINDOWS)
endif()

# --target install (copy res near the executable)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res/
        DESTINATION engine_res)