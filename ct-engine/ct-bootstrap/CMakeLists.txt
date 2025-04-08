cmake_minimum_required(VERSION 4.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MODULE_NAME chibi-tech)

set(SOURCE
    chibi-tech.c
)

add_executable(${MODULE_NAME} ${SOURCE})
set_property(TARGET ${MODULE_NAME} PROPERTY C_STANDARD 23)

target_compile_definitions(${MODULE_NAME} PRIVATE CT_IMPORT)

target_include_directories(${MODULE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${MODULE_NAME} PRIVATE ct-core ct-runtime)
