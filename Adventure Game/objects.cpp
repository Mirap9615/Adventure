//
// Created by M on 9/6/23.
//

#include "objects.h"

#include <utility>


Object::Object(int id, std::string given_name, std::string  description, const std::string& given_mainType, const std::string& given_subType)
: name(std::move(given_name)), id(id), description(std::move(description)) {
    if (given_mainType == "material") {
        mainType = 0;
    }
    subType = given_subType;
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

