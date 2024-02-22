#include "LongNumber.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <string>


// helper function to remove trailing zeros from a string
void removeTrailingZeros(std::string& str) {
    size_t pos = str.find_last_not_of('0');
    if (pos != std::string::npos) {
        str.erase(pos + 1);
    } else {
        str = "";
    }
}

// constructors
LongNumber::LongNumber() {}

// constructor from string
LongNumber::LongNumber(const std::string& number_str) {
    if (number_str.empty()) {
        // if empty string
        integer = "0";
        decimal = "";
        negative = false;
        return;
    }

    // is negative?
    size_t start_pos = 0;
    if (number_str[0] == '-') {
        negative = true;
        start_pos = 1;
    } else {
        negative = false;
    }

    // get integer and decimal parts
    size_t dot_pos = number_str.find('.');
    if (dot_pos == std::string::npos) {
        integer = number_str.substr(start_pos);
        decimal = "";
    } else {
        integer = number_str.substr(start_pos, dot_pos - start_pos);
        decimal = number_str.substr(dot_pos + 1);
    }
    // Remove trailing && leading zeros
    integer.erase(0, std::min(integer.find_first_not_of('0'), integer.size() - 1));
    removeTrailingZeros(decimal);
}

// constructor from long double
LongNumber::LongNumber(long double value) {
    std::stringstream ss;
    ss << std::fixed << value; // no precision (default stringstream size)
    std::string number_str = ss.str();
    // is negative?
    if (number_str.front() == '-') {
        negative = true;
        number_str.erase(0, 1); // Remove the negative sign
    } else {
        negative = false;
    }

    // split the number
    removeTrailingZeros(number_str);
    size_t dot_pos = number_str.find('.');
    if (dot_pos == std::string::npos) {
        integer = number_str;
    } else {
        integer = number_str.substr(0, dot_pos);
        decimal = number_str.substr(dot_pos + 1);
    }
}

// user-defined floating-point literal
LongNumber operator"" _ln(long double value) {
    return {value}; // construct from long double
}

// convert LongNumber to string
std::string LongNumber::toString() const {
    std::string result;

    // append negative sign
    if (negative) {
        result += '-';
    }

    // append integer part
    result += integer;

    // append decimal part
    if (!decimal.empty()) {
        result += '.';
        result += decimal;
    }
    return result;
}


// Compound assignment operators

// assignment operator
LongNumber& LongNumber::operator=(const LongNumber& rhs) {
    if (this != &rhs) {
        integer = rhs.integer;
        decimal = rhs.decimal;
        negative = rhs.negative;
    }
    return *this;
}

// assignment addition operator
LongNumber& LongNumber::operator+=(const LongNumber& rhs) {
    // one of the numbers is negative => convert both to positive
    bool resultNegative = negative;
    if (negative || rhs.negative) {
        LongNumber temp = *this;
        temp.negative = false;
        LongNumber rhsTemp = rhs;
        rhsTemp.negative = false;
        if (temp < rhsTemp) {
            *this = rhs;
            *this -= temp;
            negative = resultNegative;
            return *this;
        }
    }

    // decimal addition operation
    int carry = 0;
    int maxDecimalLength = std::max(decimal.length(), rhs.decimal.length());
    for (int i = 0; i < maxDecimalLength; ++i) {
        int digitSum = carry;
        if (i < decimal.length()) {
            digitSum += decimal[decimal.length() - 1 - i] - '0';
        }
        if (i < rhs.decimal.length()) {
            digitSum += rhs.decimal[rhs.decimal.length() - 1 - i] - '0';
        }
        carry = digitSum / 10;
        digitSum %= 10;
        if (i >= decimal.length()) {
            decimal.insert(0, 1, '0');
        }
        decimal[decimal.length() - 1 - i] = digitSum + '0';
    }

    // carry if necessary
    if (carry > 0) {
        if (maxDecimalLength >= decimal.length()) {
            decimal.insert(0, 1, '0');
        }
        decimal[0] = carry + '0';
    }

    // add the integer parts
    int intCarry = 0;
    for (int i = integer.length() - 1, j = rhs.integer.length() - 1; i >= 0 || j >= 0 || intCarry > 0; --i, --j) {
        int digitSum = intCarry;
        if (i >= 0) {
            digitSum += integer[i] - '0';
        }
        if (j >= 0) {
            digitSum += rhs.integer[j] - '0';
        }
        intCarry = digitSum / 10;
        digitSum %= 10;
        if (i < 0) {
            integer.insert(0, 1, '0');
        }
        integer[i] = digitSum + '0';
    }

    // adjust the sign of the result
    negative = resultNegative;

    return *this;
}

// assignment subtraction operator
LongNumber& LongNumber::operator-=(const LongNumber& rhs) {
    bool resultNegative = negative || (*this < rhs);

    // signs are different => perform addition
    if (negative != rhs.negative) {
        *this += rhs;
        return *this;
    }

    std::string resultInteger;
    std::string resultDecimal;

    // subtract decimal parts
    int borrow = 0;
    int lhsDecLen = decimal.length();
    int rhsDecLen = rhs.decimal.length();
    for (int i = std::max(lhsDecLen, rhsDecLen) - 1; i >= 0; --i) {
        int diff = ((*this < rhs) ? ((isdigit(rhs.decimal[i])) ? rhs.decimal[i] : '0') : ((isdigit(decimal[i])) ? decimal[i] : '0')) - '0' - borrow;
        diff -= (((rhs < *this) ? ((isdigit(rhs.decimal[i])) ? rhs.decimal[i] : '0') : ((isdigit(decimal[i])) ? decimal[i] : '0')) - '0');

        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        resultDecimal = std::to_string(diff) + resultDecimal;
    }

    // subtract integer parts
    int lhsIntLen = integer.length();
    int rhsIntLen = rhs.integer.length();
    for (int i = 0; i < std::max(lhsIntLen, rhsIntLen); ++i) {
        int diff = ((*this < rhs) ? rhs.integer[rhsIntLen - i - 1] : integer[lhsIntLen - i - 1]) - '0';
        if (i < std::min(lhsIntLen, rhsIntLen)) {
            diff -= ((rhs < *this) ? rhs.integer[rhsIntLen - i - 1] : integer[lhsIntLen - i - 1]) - '0';
        }
        diff -= borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        resultInteger = std::to_string(diff) + resultInteger;
    }

    // Remove trailing && leading zeros
    resultInteger.erase(0, std::min(resultInteger.find_first_not_of('0'), resultInteger.size() - 1));
    removeTrailingZeros(resultDecimal);

    // Set the sign of the result
    negative = (resultInteger.empty() && resultDecimal.empty()) ? false : resultNegative;
    // Update the result
    integer = resultInteger.empty() ? "0" : resultInteger;
    decimal = resultDecimal.empty() ? "" : resultDecimal;

    return *this;
}

// helper function for string multiplication
std::string multiplyStrings(const std::string& num1, const std::string& num2) {
    int m = num1.size();
    int n = num2.size();
    std::vector<int> result(m + n, 0);

    for (int i = m - 1; i >= 0; --i) {
        for (int j = n - 1; j >= 0; --j) {
            int mul = (num1[i] - '0') * (num2[j] - '0');
            int sum = mul + result[i + j + 1];
            result[i + j + 1] = sum % 10; // update position
            result[i + j] += sum / 10;    // carry to next
        }
    }

    std::string product;
    for (int digit : result) {
            product.push_back(digit + '0');
    }

    return product.empty() ? "0" : product;
}

// assignment multiplication operator
LongNumber& LongNumber::operator*=(const LongNumber& rhs) {
    // multiplication for integer part
    std::string result = multiplyStrings(integer + decimal, rhs.integer + rhs.decimal);
    integer = result.substr(0, result.length() - decimal.length() - rhs.decimal.length());
    decimal = result.substr(result.length() - decimal.length() - rhs.decimal.length());
    negative = (negative != rhs.negative);

    integer.erase(0, std::min(integer.find_first_not_of('0'), integer.size() - 1));
    removeTrailingZeros(decimal);
    return *this;
}

//helper function to compare number strings
bool strnumcmp(std::string a, std::string b) {
    if (a.length() != b.length()) {
        return a.length() < b.length();
    }

    for (int i = 0; i < a.length(); ++i) {
        if (a[i] < b[i]) {
            return true;
        } else if (a[i] > b[i]) {
            return false;
        }
    }

    return false;
}

// assignment division operator
LongNumber& LongNumber::operator/=(const LongNumber& rhs) {
    // ensure rhs is not zero
    if (rhs == LongNumber(0)) {
        throw std::invalid_argument("Division by zero");
    }

    int precision = 100;

    LongNumber remainder = *this;
    std::string divisor = rhs.integer + rhs.decimal;
    remainder.negative = false;

    // move the point
    if (!(rhs.decimal.empty())) {
        for(int i = 0; i < rhs.decimal.length(); ++i) {
            remainder.integer.push_back((isdigit(remainder.decimal[0])) ? remainder.decimal[0] : '0');
            remainder.decimal.erase(remainder.decimal.begin());
        }
    }
    // Initialize quotient
    LongNumber quotient;
    quotient = LongNumber(0);

    // Perform long division
    while (!(remainder < divisor)) {
        int digit = 0;
        while (!(remainder < (LongNumber(divisor) * (digit + 1)))) {
            digit++;
        }
        quotient.integer += std::to_string(digit);

        // Subtract divisor * digit from remainder
        remainder -= (LongNumber(divisor) * digit);
    }

    // Perform long division for the decimal part
    if (remainder != LongNumber(0)) {
        for (int i = 0; i < precision; i++) {
            if (remainder == LongNumber(0)){
                break;
            }

            // moving the point to the right
            if (!remainder.decimal.empty()) {
                remainder.integer.push_back(remainder.decimal[0]);
                remainder.decimal.erase(remainder.decimal.begin());
            } else {
                remainder.integer.push_back('0');
            }

            remainder.integer.erase(0, std::min(remainder.integer.find_first_not_of('0'), remainder.integer.size() - 1));

            int digit = 0;
            while (!(strnumcmp(remainder.integer, divisor))) {
                digit++;
                remainder -= divisor;
            }
            quotient.decimal += std::to_string(digit);
        }
    }
    // Adjust sign of quotient
    quotient.negative = (negative != rhs.negative);

    // Update *this with the quotient
    // Remove trailing && leading zeros
    quotient.integer.erase(0, std::min(quotient.integer.find_first_not_of('0'), quotient.integer.size() - 1));
    removeTrailingZeros(quotient.decimal);

    *this = quotient;

    return *this;
}



// Arithmetic operators
LongNumber operator+(LongNumber lhs, const LongNumber& rhs) {
    return lhs += rhs;
}

LongNumber operator-(LongNumber lhs, const LongNumber& rhs) {
    return lhs -= rhs;
}

LongNumber operator*(LongNumber lhs, const LongNumber& rhs) {
    return lhs *= rhs;
}

LongNumber operator/(LongNumber lhs, const LongNumber& rhs) {
    return lhs /= rhs;
}


// Comparison operators

// equality comparison
bool operator==(const LongNumber& lhs, const LongNumber& rhs) {
   return (lhs.negative == rhs.negative && lhs.integer == rhs.integer && lhs.decimal == rhs.decimal);
}

// inequality comparison
bool operator!=(const LongNumber& lhs, const LongNumber& rhs) {
    return (!(lhs == rhs));
}

//less than
bool operator<(const LongNumber& lhs, const LongNumber& rhs) {
    // Compare signs
    if (lhs.negative != rhs.negative) {
        return lhs.negative;
    }

    // Compare integer parts
    if (lhs.integer != rhs.integer) {
        return lhs.negative ? strnumcmp(rhs.integer, lhs.integer) : strnumcmp(lhs.integer, rhs.integer);
    }

    // Compare decimal parts
    return strnumcmp(lhs.decimal, rhs.decimal);
}

// greater than
bool operator>(const LongNumber& lhs, const LongNumber& rhs) {
    return (rhs < lhs);
}