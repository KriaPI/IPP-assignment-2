add_executable(exercise1 exercise-1/integralApproximator.cpp exercise-1/integralApproximator.hpp)
add_library(integralApproximator INTERFACE)
target_include_directories(integralApproximator INTERFACE exercise-1)

add_executable(exercise4 
    exercise-4/benchmark_example.cpp 
    exercise-4/benchmark.hpp
    exercise-4/sorted_list.hpp
)