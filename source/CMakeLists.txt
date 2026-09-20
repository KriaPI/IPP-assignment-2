add_executable(exercise1 exercise-1/integralApproximator.cpp exercise-1/integralApproximator.hpp)

add_library(integralApproximator INTERFACE)
target_include_directories(integralApproximator INTERFACE exercise-1)