#include "../src/LongNumber/LongNumber.h"
#include <iostream>
#include "../lib/gtest/gtest.h"

TEST(LongNumberTest, Comparison) {
// Test less than operator
LongNumber num1("1234567890.1234567890");
LongNumber num2("9876543210.9876543210");
ASSERT_TRUE(num1 < num2);

// Test greater than operator
LongNumber num3("-0.0000000000000000000000001");
LongNumber num4("0");
ASSERT_TRUE(num3 > num4);

// Test equality operator
LongNumber num5("1234567890.12345678901234");
LongNumber num6("1234567890.12345678901234");
ASSERT_TRUE(num5 == num6);
}

TEST(LongNumberTest, StringConversion) {
// Test converting LongNumber to string
LongNumber num1("1234567890.1234567890");
std::string expected_str = "1234567890.1234567890";
ASSERT_EQ(num1.toString(), expected_str);

// Test creating LongNumber from string
std::string num_str = "9876543210.9876543210";
LongNumber expected_num("9876543210.9876543210");
ASSERT_EQ(LongNumber(num_str), expected_num);
}

TEST(LongNumberTest, ZeroDivision) {
// Test division by zero
LongNumber num1("1234567890.1234567890");
LongNumber num2("0.0");
ASSERT_THROW(num1 / num2, std::invalid_argument);
}

TEST(LongNumberTest, LargeNumbers) {
// Test with large numbers
LongNumber num1("12345678901234567890.1234567890");
LongNumber num2("98765432109876543210.9876543210");
LongNumber expected_sum("111111111011111111100.1111111100");
ASSERT_EQ(num1 + num2, expected_sum);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
