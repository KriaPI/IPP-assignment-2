#pragma once
#include <functional>
#include <algorithm>
#include <vector>

using numerical = double;

struct threadData {
    numerical& sum;  
    numerical lower;
    numerical upper;
    size_t trapezes;
    std::function<numerical(numerical)> integrand;
};

/// @brief Approximate an integral using trapeses.
/// @param function A function that takes a floating-point number. 
/// @param lower The lower bound of the integral.
/// @param upper The upper bound of the integral.
/// @return The approximate value of integrating the functions over the bounds [lower, upper].
template <typename T> 
numerical approximateIntegral(T integrand, numerical lower, numerical upper, size_t trapezes) {
    numerical result {0}; 
    const numerical chunkSize = (upper - lower) / static_cast<numerical>(trapezes);
    
    for (numerical currentLower = lower; currentLower < upper; currentLower += chunkSize) {
        // TODO: avoid duplicate function calls.
        auto a {integrand(currentLower)};
        auto b {integrand(currentLower + chunkSize)};

        auto rectangle {std::min(a, b) * chunkSize};
        auto triangleHeight {std::abs(b - a)};
        auto triangle {triangleHeight * chunkSize / 2};
        auto trapeze = rectangle + triangle;
        result += trapeze;
    }

    return result;
}

void* approximateIntegralWrapper(void* data) {
    threadData& inputs {*static_cast<threadData*>(data)};
    auto result {approximateIntegral(inputs.integrand, inputs.lower, inputs.upper, inputs.trapezes)};    
    // TODO: write to inputs.sum.

    pthread_exit(0);
}

/// @brief Approximate an integral using trapeses.
/// @param function A function that takes a floating-point number and returns a floating-point number. 
/// @param lower The lower bound of the integral.
/// @param upper The upper bound of the integral.
/// @param trapezes The number of trapezes used to approximate the integral over the bounds.
/// @param threads The number of threads used to calculate the integral.
/// @return The approximate value of integrating the functions over the bounds [lower, upper].
template <typename T> 
numerical approximateIntegralThreaded(T integrand, numerical lower, numerical upper, size_t trapezes, int threadCount) {
    // Idea: 
    // Divide the work and spawn a new thread for each chunk
    // In each thread, compute the result and set the shared variable. 
    // Join threads.
    
    numerical result {0};
    std::vector<pthread_t> threads(threadCount);
    std::vector<threadData> data(threadCount);
    int index = 0;
    for (const auto& thread: threads) {
        // TODO: divide work (think about the fact that trapezes may be differently size if we 
        // assume that the intervals are of equal size).
        // TODO: pass data and function.
        
        pthread_create(&thread, nullptr, &approximateIntegralWrapper, nullptr);
    }

    for (const auto& thread: threads) {
        pthread_join(thread);
    }

    return approximateIntegral(integrand, lower, upper, trapezes);
}