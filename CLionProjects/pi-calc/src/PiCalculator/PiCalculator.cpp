#include "../LongNumber/LongNumber.h"
#include "PiCalculator.h"
#include <iostream>

LongNumber pow_ll(const LongNumber& x, const int y) {
    LongNumber temp(1);
    for (int i = 0; i < y; ++i) {
        temp *= x;
    }
    return temp;
}

LongNumber PiCalculator::calculatePi(int precision) {
    LongNumber pi(0);

    for (int k = 0; k < precision; ++k) {

        long eight_k = k * 8;
        LongNumber term_1 = LongNumber(1) / pow_ll(16, k);
        LongNumber term_2 = LongNumber(4) / (eight_k + 1);
        LongNumber term_3 = LongNumber(2) / (eight_k + 4);
        LongNumber term_4 = LongNumber(1) / (eight_k + 5);
        LongNumber term_5 = LongNumber(1) / (eight_k + 6);
        pi += (term_1 * (term_2 - term_3 - term_4 - term_5));
    }

    return pi;
}
