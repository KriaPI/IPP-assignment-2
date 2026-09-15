#pragma once

using numerical = double;

/// @brief Approximate an integral using trapeses.
/// @param function A function that takes a floating-point number. 
/// @param lower The lower bound of the integral.
/// @param upper The upper bound of the integral.
/// @return The approximate value of integrating the functions over the bounds [lower, upper].
template <typename T> 
numerical approximateIntegral(T function, numerical lower, numerical upper, size_t trapezes) {
    numerical result {0}; 
    const numerical chunkSize = (upper - lower) / static_cast<numerical>(trapezes);

    for (numerical currentLower = lower; currentLower < upper; currentLower += chunkSize) {
        auto a {function(currentLower)};
        auto b {function(currentLower + chunkSize)};

        auto square {std::min(a, b) * chunkSize};
        numerical triangleHeight {std::abs(b - a)};
        auto triangle {triangleHeight * chunkSize / 2};
        auto trapeze = square + triangle;
        result += trapeze;
    }

    return result;
}