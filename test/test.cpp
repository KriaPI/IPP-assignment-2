#include <gtest/gtest.h>
#include "integralApproximator.hpp"

TEST(Exercise1, approximateLineIntegral1) { 
    EXPECT_EQ(approximateIntegral([] (auto x) {return x;}, 0, 5, 1), 12.5); 
}

TEST(Exercise1, approximateLineIntegral2) { 
    EXPECT_EQ(approximateIntegral([] (auto x) {return x;}, 0, 5, 2), 12.5); 
}

TEST(Exercise1, approximateLineIntegral3) { 
    EXPECT_EQ(approximateIntegral([] (auto x) {return x;}, 0, 5, 3), 12.5); 
}

TEST(Exercise1, approximatePolynomialIntegral) { 
    EXPECT_EQ(
        approximateIntegral([] (auto x) {return 4 / (1 + (x * x));}, 0, 1, 1), 
    4); 
}