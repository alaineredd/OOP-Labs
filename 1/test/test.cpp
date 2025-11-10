#include <gtest/gtest.h>

#include "gcd.h"

TEST(test01, Positives) {
    EXPECT_EQ(gcd::gcd(48, 18), 6);
    EXPECT_EQ(gcd::gcd(20, 100), 20);
    EXPECT_EQ(gcd::gcd(5, 2), 1);
}

TEST(test02, Negatives) {
    EXPECT_EQ(gcd::gcd(-10, -2), 2);
    EXPECT_EQ(gcd::gcd(-10, 2), 2);
    EXPECT_EQ(gcd::gcd(-5, -35), 5);
}

TEST(test03, Zeros) {
    EXPECT_EQ(gcd::gcd(0, 0), 0);
    EXPECT_EQ(gcd::gcd(10, 0), 10);
    EXPECT_EQ(gcd::gcd(0, 7), 7);
}

TEST(test04, SameNumbers) {
    EXPECT_EQ(gcd::gcd(15, 15), 15);
}

TEST(test05, LargeNumbers) {
    EXPECT_EQ(gcd::gcd(123456789, 987654321), 9);
    EXPECT_EQ(gcd::gcd(1000000000, 500000000), 500000000);
}