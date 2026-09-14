#pragma once

using numerical = int;

/// @brief Approximate an integral using trapeses.
/// @param function A function that takes a floating-point number. 
/// @param lower The lower bound of the integral.
/// @param upper The upper bound of the integral.
/// @return The approximate value of integrating the functions over the bounds [lower, upper].
template <typename T> 
numerical approximateIntegral(T function, numerical lower, numerical upper) {
    return 1;
}