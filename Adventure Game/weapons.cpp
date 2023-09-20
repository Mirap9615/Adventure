//
// Created by William on 9/6/23.
//

#include "weapons.h"

std::map<int, std::unique_ptr<Weapon>> weapons_dict;


Weapon::Weapon(int given_id, const std::string& given_name, const std::string& given_type, const std::string& given_description, float given_base_damage,
               float given_base_durability, float given_base_range, bool given_magical, const std::string& given_magic_type, int given_enchantability)
: Item(given_id, given_name, "weapon"), damage(10, given_base_damage), durability(11, given_base_durability), range(12, given_base_range) {
    description = given_description; magical = given_magical; enchantability = given_enchantability;

    // determine magic type here
    if (given_magic_type == "none") {
        magic_type = 0;
    } else if (given_magic_type == "all") {
        magic_type = 1;
    }
}


std::unique_ptr<Weapon> create_new_weapon(int id, const std::string& name, const std::string& description, float base_damage, float base_durability, float base_range, bool magical, const std::string& magic_type, int enchantability) {
    return std::make_unique<Weapon>(id, name, "weapon", description, base_damage, base_durability, base_range, magical, magic_type, enchantability);
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