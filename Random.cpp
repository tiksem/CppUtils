//
// Created by Semyon Tikhonenko on 2018-12-31.
//

#include "Random.h"
#include <random>

namespace CppUtils {
    namespace Random {
        double RandomDoubleInClosedRange(double min, double max) {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_real_distribution<> distribution(min, std::nextafter(max, std::numeric_limits<double>::max()));
            return distribution(gen);
        }

        int RandomIntInRange(int min, int max) {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<> distribution(min, max);
            return distribution(gen);
        }
    }
}
