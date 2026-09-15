#include <iostream>
#include "integralApproximator.hpp"

int main () {
    auto result = approximateIntegral([] (auto x) {return 4 / (1 + (x * x));}, 0, 1, 10000);
    std::cout << "result: " << result << "\n";
    return 0;
}