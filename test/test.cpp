#include <gtest/gtest.h>
#include "integralApproximator.hpp"

TEST(Exercise1, approximateLineIntegral) { 
    EXPECT_EQ(approximateIntegral([] {}, 0, 1), 1); 
}
