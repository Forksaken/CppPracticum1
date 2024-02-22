#include <iostream>
#include <chrono>
#include "PiCalculator/PiCalculator.h"

int main() {
    /*int precision;
    std::cout << "Enter the precision: ";
    std::cin >> precision;

    auto start = std::chrono::high_resolution_clock::now();

    LongNumber pi = PiCalculator::calculatePi(precision);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Pi with " << precision << " digits: " << pi.toString() << std::endl;
    std::cout << "Time taken: " << elapsed.count() << " seconds" << std::endl;
    return 0;*/
    LongNumber x(8);
    LongNumber y(2);
    x /= y;
    std::cout << x.toString() << std::endl;

}
