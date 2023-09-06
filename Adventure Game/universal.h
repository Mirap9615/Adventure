//
// Created by William on 9/6/23.
//


#ifndef FRONTIER_GAME_UNIVERSAL_H
#define FRONTIER_GAME_UNIVERSAL_H

#include <string>
#include <map>


//std::map<int, std::string> items_all;

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

float clamp(float value, float min, float max);

#endif //FRONTIER_GAME_UNIVERSAL_H
