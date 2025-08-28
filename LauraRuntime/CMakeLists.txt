project(LauraRuntime LANGUAGES CXX)

add_executable(LauraRuntime)

target_compile_features(LauraRuntime PRIVATE cxx_std_20)
file(GLOB_RECURSE RUNTIME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
target_sources(LauraRuntime PRIVATE ${RUNTIME_SOURCES})
target_include_directories(LauraRuntime PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(LauraRuntime PUBLIC LauraEngine)

# compile into the same config folder as the editor
set_target_properties(LauraRuntime PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/runtime
    #RUNTIME_OUTPUT_DIRECTORY C:/Dev/LauraExports/tst123
)

install(TARGETS LauraRuntime # executable
    RUNTIME DESTINATION runtime
)

if(MSVC)
    target_compile_definitions(LauraRuntime PUBLIC _CRT_SECURE_NO_WARNINGS)
    if(NOT ENABLE_CONSOLE)
        set_target_properties(LauraRuntime PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    endif()
endif()

