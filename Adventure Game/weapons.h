//
// Created by William on 9/6/23.
//

#ifndef FRONTIER_GAME_WEAPONS_H
#define FRONTIER_GAME_WEAPONS_H
#include "objects.h"


class Weapon : public Item {
public:
    Weapon(int id, std::string given_name, std::string description, const std::string& mainType, const std::string& subType,
           float base_atk_dmg, float base_durability, float base_range, bool magical, int given_magic_type, int enchantability);

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

};

std::unique_ptr<Weapon> create_new_weapon(int id, std::string given_name, std::string description, const std::string& mainType, const std::string& subType,
                                          float base_atk_dmg, float base_durability, float base_range, bool magical, int given_magic_type, int enchantability);

#endif //FRONTIER_GAME_WEAPONS_H

//void loadWeapons();

//void printAllWeapons();
