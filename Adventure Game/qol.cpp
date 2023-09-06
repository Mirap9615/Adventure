//
// Created by M on 9/6/23.
//
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include "nlohmann/json.hpp"
#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>
#include <thread> // for sleep
#include <sstream> // for string stream operations
#include "qol.h"

// Attribute 2-value constructor
Attribute::Attribute(int type, float inValue) : type(type), value(inValue) {
    max_value = value;
    bonus_flat = 0;
    boost_percentage = 0;
}

// Attribute 4-value constructor
Attribute::Attribute(float inValue, float max_value_in, float bonus_flat_in, float bonus_percentage_in) {
    value = inValue;
    max_value = max_value_in;
    bonus_flat = bonus_flat_in;
    boost_percentage = bonus_percentage_in;
    type = 0;
}

float Attribute::effectiveValue() const {
    return (value + bonus_flat) * ((1 + boost_percentage) / 1);
}

Attribute& Attribute::operator+=(float inValue) {
    value += inValue;
    return *this;
}

Attribute& Attribute::operator=(float inValue) {
    value = inValue;
    max_value = inValue;
    return *this;
}

void Attribute::changeType(int typeValue) {
    type = typeValue;
}

Attribute& Attribute::operator-=(float inValue) {
    value -= inValue;
    return *this;
}

float Attribute::max() const {
    return max_value;
}

