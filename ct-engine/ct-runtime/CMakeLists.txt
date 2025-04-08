cmake_minimum_required(VERSION 4.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MODULE_NAME ct-runtime)

set(APPLICATION
    application/application.h
    application/application-config.h
)

set(CORE
    core/engine.c
    core/engine.h
)

set(SOURCES ${APPLICATION} ${CORE})

set(PUBLIC_HEADERS
    application/application.h
    application/application-config.h
    core/engine.h
)

add_library(${MODULE_NAME} SHARED)
set_property(TARGET ${MODULE_NAME} PROPERTY C_STANDARD 23)

target_sources(${MODULE_NAME} PRIVATE ${SOURCES})
target_sources(${MODULE_NAME} PUBLIC ${PUBLIC_HEADERS})

target_compile_definitions(${MODULE_NAME} PRIVATE CT_EXPORT)
target_include_directories(${MODULE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#link dependencies
include(CMake/flecs.cmake)

target_link_libraries(${MODULE_NAME} PUBLIC flecs::flecs)
target_link_libraries(${MODULE_NAME} PRIVATE ct-core)
