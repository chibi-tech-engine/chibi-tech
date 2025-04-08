cmake_minimum_required(VERSION 4.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Core engine components
add_subdirectory(ct-core)
add_subdirectory(ct-runtime)
add_subdirectory(ct-bootstrap)

# Engine Plugins
add_subdirectory(ct-plugins)
