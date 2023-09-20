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

void Protagonist::showAll() {
    std::string border(70, '=');
    std::cout << border << std::endl;

    // Print basic details
    std::cout << name << std::endl;
    std::cout << "Player ID: " << player_id << std::endl;
    std::cout << "Reputation: " << reputation << ", Charm: " << charm << std::endl;
    std::cout << "Level: " << level << ", " << percentToNextXPLevel() << "% until next level" << std::endl;
    std::cout << "Balance: " << std::fixed << std::setprecision(0) << balance << " slate" << std::endl;

    // Print inventory
    std::cout << border << std::endl;
    std::cout << "Inventory:" << std::endl;
    inventory.show();

    // Print equipped items
    std::cout << "Equipped Items:" << std::endl;
    slots.show();

    std::cout << border << std::endl;
}