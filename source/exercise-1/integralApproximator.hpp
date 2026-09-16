#pragma once
#include <functional>
#include <algorithm>
#include <cmath>
#include <vector>
#include <pthread.h>

using numerical = double;

struct IntegralBounds {
    numerical lower;
    numerical upper;
};

struct threadData {
    std::function<numerical(numerical)> integrand;
    IntegralBounds bounds;
    size_t trapezes;
    numerical* sum; 
    pthread_mutex_t* sharedLock; 
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
    auto result {approximateIntegral(inputs.integrand, inputs.bounds.lower, inputs.bounds.upper, inputs.trapezes)};    
    // TODO: write to inputs.sum.

    pthread_exit(nullptr);
}


/// @brief Approximate an integral using trapeses.
/// @param function A function that takes a floating-point number and returns a floating-point number. 
/// @param lower The lower bound of the integral.
/// @param upper The upper bound of the integral.
/// @param trapezes The number of trapezes used to approximate the integral over the bounds.
/// @param threads The number of threads used to calculate the integral.
/// @return The approximate value of integrating the functions over the bounds [lower, upper].
template <typename T> 
numerical approximateIntegralThreaded(T integrand, IntegralBounds bounds, size_t trapezes, int threadCount) {
    // Idea: 
    // Divide the work and spawn a new thread for each chunk
    // In each thread, compute the result and set the shared variable. 
    // Join threads.
    
    numerical result {0};
    std::vector<pthread_t> threads(threadCount);
    std::vector<threadData> data{};
    pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER;
    
    const size_t minimumTrapezesPerThread {trapezes / threadCount};
    // This remainder should be spread across each thread.
    auto remainingTrapezes {trapezes % threadCount};
    auto boundLength {bounds.upper - bounds.lower};
    auto trapezeWidth {boundLength / static_cast<numerical>(trapezes)};

    auto currentLower {bounds.lower};

    for (const auto& thread: threads) {
        auto trapezesPerThread {remainingTrapezes > 0 ? minimumTrapezesPerThread + 1: minimumTrapezesPerThread};
        --remainingTrapezes;
        auto currentUpper {currentLower + (static_cast<numerical>(trapezesPerThread) * trapezeWidth)};

        auto& threadData = data.emplace_back({
            .integrand = integrand,
            .bounds = IntegralBounds{.lower = currentLower, .upper = currentUpper},
            .trapezes = trapezesPerThread,
            .sum = &result,
            .sharedLock = &sharedLock
        });
        currentLower = currentUpper;
        pthread_create(std::addressof(thread), nullptr, approximateIntegralWrapper, &threadData);
    }

    for (const auto& thread: threads) {
        pthread_join(thread, nullptr);
    }

    return result;
}