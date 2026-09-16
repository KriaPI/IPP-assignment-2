#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <pthread.h>
#include <iostream>

using numerical = double;

struct IntegralBounds {
    numerical lower;
    numerical upper;
};

struct ThreadData {
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
    const numerical width = (upper - lower) / static_cast<numerical>(trapezes);
    
    auto a {integrand(lower)};
    for (size_t i = 0; i < trapezes; ++i)
     {
        auto currentLower {lower + (static_cast<numerical>(i) * width)};
        auto b {integrand(currentLower + width)};
        
        auto area {(a + b) * width / 2};
        result += area;
        a = b;
    }

    return result;
}

void* approximateIntegralWrapper(void* data) {
    const ThreadData& inputs {*static_cast<ThreadData*>(data)};
    auto partialSum {approximateIntegral(inputs.integrand, inputs.bounds.lower, inputs.bounds.upper, inputs.trapezes)};    
    
    pthread_mutex_lock(inputs.sharedLock);
    *inputs.sum += partialSum;
    pthread_mutex_unlock(inputs.sharedLock);

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
    // TODO: use a smart pointer instead (shared or )
    auto data { std::make_unique<ThreadData[]>(threadCount)};
    pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER;
    
    const size_t minimumTrapezesPerThread {trapezes / threadCount};
    // This remainder should be spread across each thread.
    auto remainingTrapezes {trapezes % threadCount};
    auto boundLength {bounds.upper - bounds.lower};
    auto trapezeWidth {boundLength / static_cast<numerical>(trapezes)};

    auto currentLower {bounds.lower};

    for (size_t i = 0; i < threads.size(); ++i) {
        auto trapezesPerThread {remainingTrapezes > 0 ? minimumTrapezesPerThread + 1: minimumTrapezesPerThread};
        // This is required to avoid integer underflow of unsigned integers!
        if (remainingTrapezes > 0) {
            --remainingTrapezes;
        }
        auto currentUpper {currentLower + (static_cast<numerical>(trapezesPerThread) * trapezeWidth)};

        data[i] = {
            integrand,
            IntegralBounds{.lower = currentLower, .upper = currentUpper},
            trapezesPerThread,
            &result,
            &sharedLock
        };
        currentLower = currentUpper;
        pthread_create(std::addressof(threads[i]), nullptr, approximateIntegralWrapper, &data[i]);
    }

    for (const auto& thread: threads) {
        pthread_join(thread, nullptr);
    }

    return result;
}