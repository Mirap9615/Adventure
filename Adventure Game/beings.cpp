//
// Created by M on 9/6/23.
//

#include <cmath>
#include <algorithm>
#include "beings.h"

// 1. Class-Independent
float clamp(float value, float min, float max) {
    return std::max(min, std::min(max, value));
}

// 2. Classes

float calculateSlate(const Organism& org) {
    float power = org.calculatePower();
    float slate = power * 10 * std::pow(power / 14.6, 2);
    return slate;
}