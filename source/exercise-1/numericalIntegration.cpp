#include <iostream>
#include "integralApproximator.hpp"

int main () {
    const size_t trapezes {1000000};
    auto result = approximateIntegral([] (auto x) {return 4 / (1 + (x * x));}, 0, 1, trapezes);
    std::cout << "result: " << result << "\n";
    return 0;
}