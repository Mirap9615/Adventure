//
// Created by M on 9/6/23.
//

#include "objects.h"

#include <utility>


Object::Object(int id, std::string given_name, std::string  description, const std::string& given_mainType, const std::string& given_subType) // NOLINT
: name(std::move(given_name)), id(id), description(std::move(description)) {
    std::tie(mainType, subType) = determineType(given_mainType, given_subType);
    stackable = true;
    price = 0;
}

Item::Item(int id, std::string given_name, std::string description, const std::string& mainType, const std::string& subType) :
Object(id, std::move(given_name), std::move(description), mainType, subType),
damage(10,-1), durability(11, -1), range(12, 100) {
    stackable = false;
    magical = false;
    magic_type = 0;
    enchantability = 50;
}

Item::Item(int id, std::string given_name, std::string description, const std::string& mainType, const std::string& subType,
     float base_atk_dmg, float base_durability, float base_range, bool magical, int given_magic_type, int enchantability) :
     Object(id, std::move(given_name), std::move(description), mainType, subType),
     damage(10,base_atk_dmg), durability(11, base_durability), range(12, base_range),
     magical(magical), magic_type(given_magic_type), enchantability(enchantability) { stackable = false; }

std::pair<int,int> Object::determineType(const std::string& given_mainType, const std::string& given_subType) {
    int curr_mainType = -1;
    int curr_subType = -1;
    if (given_mainType == "material") {
        curr_mainType = 0;
        if (given_subType == "material") {
            curr_subType = 0;
        } else if (given_mainType == "weapon") {
            curr_mainType = 1;
        } else if (given_mainType == "tool") {
            curr_mainType = 2;
        } else if (given_mainType == "consumable") {
            curr_mainType = 3;
        } else if (given_mainType == "armor") {
            curr_mainType = 4;
            if (given_subType == "hand") {
                curr_subType = 0;
            } else if (given_subType == "helmet") {
                curr_subType = 1;
            } else if (given_subType == "scarf") {
                curr_subType = 2;
            } else if (given_subType == "chestplate") {
                curr_subType = 3;
            } else if (given_subType == "rerebrace") {
                curr_subType = 4;
            } else if (given_subType == "leggings") {
                curr_subType = 5;
            } else if (given_subType == "shoes") {
                curr_subType = 6;
            } else if (given_subType == "ring") {
                curr_subType = 7;
            }
        }
    }
    return std::make_pair(curr_mainType, curr_subType);
}