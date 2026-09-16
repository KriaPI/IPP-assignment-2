#include <iostream>
#include "integralApproximator.hpp"

int main (int argc, char** argv) {
    // TODO: implement command line argument parsing.

    const size_t trapezes {1000};
    //auto integrand = [] (auto x) {return 4 / (1 + (x * x));};
    auto integrand = [] (numerical x) {return x;};
    auto result = approximateIntegralThreaded(integrand, IntegralBounds{.lower = 0, .upper = 2}, trapezes, 6);

    return 0;
}