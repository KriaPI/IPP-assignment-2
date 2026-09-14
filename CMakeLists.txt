cmake_minimum_required(VERSION 3.25)
project(Template LANGUAGES CXX)
enable_testing()

# Set C++ language standard and required variables
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(defaults)
include(default_compiler_flags)
include(sanitizers)
include(coverage)

# Add packages
find_package(GTest CONFIG REQUIRED)

add_subdirectory(source)
add_subdirectory(test)

include(project_tools)
project_enable_clang_format()
project_enable_clang_tidy()