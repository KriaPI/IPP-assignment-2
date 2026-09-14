cmake_minimum_required(VERSION 3.25)
project(Template LANGUAGES CXX)
enable_testing()

# Set C++ language standard and required variables
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(defaults)
include(default_compiler_flags)
include(sanitizers)
include(coverage)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(source)
add_subdirectory(test)

include(project_tools)
project_enable_clang_format()
project_enable_clang_tidy()