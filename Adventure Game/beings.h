//
// Created by M on 9/6/23.
//

#ifndef FRONTIER_GAME_BEINGS_H
#define FRONTIER_GAME_BEINGS_H
// 1. Class-Independent
enum AttributeType {
    HP = 1, MANA = 2, STAMINA = 3, DEFENSE = 4, PHYS_ATK = 5, MAG_ATK = 6, SPD = 7, INTELLIGENCE = 8, LUCK = 9,
    CRIT_CHANCE = 10,CRIT_DAMAGE = 11,
};

float clamp(float value, float min, float max);

// 2. Classes
class Organism;
class Magical;
class Monster;
class Sicut;
class Protagonist;

// 3. Class-Dependent
struct CombatResult {
    bool isOver;
    Organism* winner;
    Organism* loser;
};


float calculateSlate(const Organism& org);
#endif //FRONTIER_GAME_BEINGS_H
