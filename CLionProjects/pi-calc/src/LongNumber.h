
#ifndef PI_CALC_LONGNUMBER_H
#define PI_CALC_LONGNUMBER_H

#include <string>
#include <vector>

class LongNumber {
    std::string integer; // int part
    std::string decimal; // decimal part
    bool negative;  // is negative


public:
    // constructors
    LongNumber();
    LongNumber(const std::string& number_str);
    LongNumber(long double number);

    // conversion to string
    std::string toString() const;

    // assignment operations
    LongNumber& operator=(const LongNumber& rhs);
    LongNumber& operator+=(const LongNumber& rhs);
    LongNumber& operator-=(const LongNumber& rhs);
    LongNumber& operator*=(const LongNumber& rhs);
    LongNumber& operator/=(const LongNumber& rhs);

    // arithmetic operations
    friend LongNumber operator+(LongNumber lhs, const LongNumber& rhs);
    friend LongNumber operator-(LongNumber lhs, const LongNumber& rhs);
    friend LongNumber operator*(LongNumber lhs, const LongNumber& rhs);
    friend LongNumber operator/(LongNumber lhs, const LongNumber& rhs);

    // comparison operations
    friend bool operator==(const LongNumber& lhs, const LongNumber& rhs);
    friend bool operator!=(const LongNumber& lhs, const LongNumber& rhs);
    friend bool operator<(const LongNumber& lhs, const LongNumber& rhs);
    friend bool operator>(const LongNumber& lhs, const LongNumber& rhs);

};

// User-defined floating-point literal
LongNumber operator"" _ln(long double value);

#endif //PI_CALC_LONGNUMBER_H
