cmake_minimum_required(VERSION 4.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MODULE_NAME ct-core)

# Engine Plugins
set(CONTAINERS
    containers/hashtable.h
    containers/strings.c
    containers/strings.h
)

set(DEBUG
    debug/ct-assert.h
)

set(MEMORY
    memory/allocators/allocator.h
    memory/allocators/dynamic-allocator.c
    memory/allocators/dynamic-allocator.h
    memory/ct-memory.c
    memory/ct-memory.h
)

set(PLATFORM
    platform/ct-platform.h
    platform/ct-platform-nix.c
    platform/ct-platform-win32.c
)

set(SOURCES
    defines.h
    types.h
    ${CONTAINERS}
    ${DEBUG}
    ${MEMORY}
    ${PLATFORM}
)

set(PUBLIC_HEADERS
    containers/hashtable.h
    containers/strings.h
    debug/ct-assert.h
    memory/allocators/allocator.h
    memory/allocators/dynamic-allocator.h
    memory/ct-memory.h
    platform/ct-platform.h
    defines.h
    types.h
)

add_library(${MODULE_NAME} SHARED)
set_property(TARGET ${MODULE_NAME} PROPERTY C_STANDARD 23)

target_sources(${MODULE_NAME} PRIVATE ${SOURCES})
target_sources(${MODULE_NAME} PUBLIC ${PUBLIC_HEADERS})

target_compile_definitions(${MODULE_NAME} PRIVATE CT_EXPORT)
target_precompile_headers(${MODULE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/pch/pch.h)
target_include_directories(${MODULE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
