//
// Created by William on 9/6/23.
//

#ifndef FRONTIER_GAME_WEAPONS_H
#define FRONTIER_GAME_WEAPONS_H
#include "objects.h"


class Weapon : public Item {
public:
    Weapon(int id, const std::string& name, const std::string& type, const std::string& description, float base_damage,
           float base_durability, float base_range, bool magical, const std::string& magic_type, int enchantability);

    float reportDamage() {
        return damage.effectiveValue();
    }

    int getID() const;

    std::string getName() const;

    std::string getDescription() const;

    std::pair<float, float> getDamageDetails() const;

    std::pair<float, float> getDurabilityDetails() const;

    std::pair<float, float> getRangeDetails() const;

    bool getMagical() const;

    int getMagicType() const;

    int getEnchantability() const;

private:
    Attribute damage;
    Attribute durability;
    Attribute range;
    bool magical;
    int magic_type;
    int enchantability;
};

std::unique_ptr<Weapon> create_new_weapon(int id, const std::string& name, const std::string& type, const std::string& description, float base_damage, float base_durability, float base_range, bool magical, const std::string& magic_type, int enchantability);

#endif //FRONTIER_GAME_WEAPONS_H

/*
void loadWeapons();
 */

void printAllWeapons();
