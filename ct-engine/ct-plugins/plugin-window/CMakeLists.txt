cmake_minimum_required(VERSION 4.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MODULE_NAME ct.plugin.window)

set(SOURCES
    window-glfw.c
    window-plugin.c
    window-plugin.h
)

add_library(${MODULE_NAME} SHARED ${SOURCES})
set_property(TARGET ${MODULE_NAME} PROPERTY C_STANDARD 23)

target_compile_definitions(${MODULE_NAME} PRIVATE CT_EXPORT)
target_include_directories(${MODULE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${MODULE_NAME} PRIVATE ct-core ct-runtime)

#link vendor dependencies
include(CMake/glfw.cmake)

target_link_libraries(${MODULE_NAME} PRIVATE glfw)
