#ifndef PICALCULATOR_H
#define PICALCULATOR_H

#include "../LongNumber/LongNumber.h"

class PiCalculator {
public:
    static LongNumber calculatePi(int precision);
};

#endif // PICALCULATOR_H
