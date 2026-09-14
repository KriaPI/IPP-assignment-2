#include <gtest/gtest.h>

#include "placeHolder.hpp"

TEST(placeHolderTest, placeHolder1) { EXPECT_EQ(uselessFunction(2, 2), 4); }

TEST(placeHolderTest, placeHolder2) { EXPECT_EQ(uselessFunction(3, 2), 5); }

TEST(placeHolderTest, placeHolder3) { EXPECT_EQ(uselessFunction(3, 6), 9); }