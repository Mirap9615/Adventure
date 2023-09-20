//
// Created by William on 9/6/23.
//

#include "weapons.h"

#include <utility>


// Enum for magic types

Weapon::Weapon(int id, std::string given_name, std::string description, const std::string& mainType, const std::string& subType,
               float base_atk_dmg, float base_durability, float base_range, bool magical, int given_magic_type, int enchantability)
: Item(id, std::move(given_name), description, mainType, subType, base_atk_dmg, base_durability, base_range, magical, given_magic_type, enchantability) {
}


std::unique_ptr<Weapon> create_new_weapon(int id, std::string given_name, std::string description, const std::string& mainType, const std::string& subType,
                                          float base_atk_dmg, float base_durability, float base_range, bool magical, int given_magic_type, int enchantability) {
    return std::make_unique<Weapon>(id, given_name, description, mainType, subType, base_atk_dmg, base_durability, base_range, magical, given_magic_type, enchantability);
}

/*
void loadWeapons() {
    std::ifstream weaponFile("weapons.json");
    nlohmann::json weaponData;
    weaponFile >> weaponData;

    for (const auto &entry: weaponData) {
        int id = entry["id"];
        std::string name = entry["name"];
        std::string description = entry["description"];
        float base_damage = entry["base attack damage"];
        float base_durability = entry["base durability"];
        float base_range = entry["base range"];
        bool magical = entry["magical"];
        int magic_type = entry["magic type"] == "all" ? 1 : 0; // 1 for 'all', 0 for 'none'
        int enchantability = entry["enchantability"];


        weapons_dict[id] = create_new_weapon(id, name, description, base_damage, base_durability, base_range, magical,
                                             magic_type, enchantability);
        weapons_dict[id]->setType(1);

    }

}
 */


int Weapon::getID() const {
    return id;
}

std::string Weapon::getName() const {
    return name;
}

std::string Weapon::getDescription() const {
    return description;
}

std::pair<float, float> Weapon::getDamageDetails() const {
    return std::pair<float, float>{damage.effectiveValue(), damage.max()};
}

std::pair<float, float> Weapon::getDurabilityDetails() const {
    return std::pair<float, float>{durability.effectiveValue(), durability.max()};
}

std::pair<float, float> Weapon::getRangeDetails() const {
    return std::pair<float, float>{range.effectiveValue(), range.max()};
}

bool Weapon::getMagical() const {
    return magical;
}

int Weapon::getMagicType() const {
    return magic_type;
}
int Weapon::getEnchantability() const {
    return enchantability;
}

/*
void printAllWeapons() {
    for (const auto& entry : weapons_dict) {
        const Weapon& weapon = *(entry.second);
        std::cout << weapon.getName() << " [" << weapon.getID() << "]\n";
        std::cout << "Description: " << weapon.getDescription() << "\n";

        auto damage_details = weapon.getDamageDetails();
        std::cout << "Damage: " << damage_details.first << " / " << damage_details.second << "\n";

        auto durability_details = weapon.getDurabilityDetails();
        std::cout << "Durability: " << durability_details.first << " / " << durability_details.second << "\n";

        auto range_details = weapon.getRangeDetails();
        std::cout << "Range: " << range_details.first << "\n";

        if (weapon.getMagical()) {
            auto magic_type = static_cast<MagicType>(weapon.getMagicType());
            std::cout << "Magic Type: " << magic_type << "\n";
        }

        std::cout << "Enchantability: " << weapon.getEnchantability() << "%\n";
        std::cout << "------------------------\n";
    }
}
 */