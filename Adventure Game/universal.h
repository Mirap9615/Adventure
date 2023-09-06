//
// Created by William on 9/6/23.
//


#ifndef FRONTIER_GAME_UNIVERSAL_H
#define FRONTIER_GAME_UNIVERSAL_H
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include "nlohmann/json.hpp" // for JSON
#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>
#include <thread> // for sleep
#include <sstream> // for string stream operations


enum AttributeType {
    HP = 1, MANA = 2, STAMINA = 3, DEFENSE = 4, PHYS_ATK = 5, MAG_ATK = 6, SPD = 7, INTELLIGENCE = 8, LUCK = 9,
    CRIT_CHANCE = 10,CRIT_DAMAGE = 11,
};


enum PartyRole {
    INITIATE,
    MEMBER,
    OFFICER,
    LEADER,
    RULER
};

// Prints the given string with a set amount of delay for every letter
void printSlowly(const std::string& text, int delay_ms = 15);

float clamp(float value, float min, float max);

std::map<int, std::string>& ItemsAllHook();

#endif //FRONTIER_GAME_UNIVERSAL_H
