//
// Created by M on 9/6/23.
//

#include "objects.h"

#include <utility>


Object::Object(int id, std::string given_name, const std::string& given_type) : name(std::move(given_name)), id(id) {
    price = 0;
    // 0 Material 1 Weapon 2 Tools 3 Consumables 4 Food
    if (given_type == "material") {
        type = 0;
    } else if (given_type == "weapon") {
        type = 1;
    } else if (given_type == "tool" || given_type == "tools") {
        type = 2;
    } else if (given_type == "consumables" || given_type == "consumable") {
        type = 3;
    } else if (given_type == "food") {
        type = 4;
    }
    if (type != 0) {
        equippable = true;
        stackable = false;
    } else {
        equippable = false;
        stackable = true;
    }
}

Item::Item(int id, std::string given_name, const std::string& given_type) : Object(id, std::move(given_name), given_type) {

}

void printAllItems() {
    std::map<int, std::shared_ptr<Object>> itemList = ItemsAllHook();
    std::cout << "There are currently " << itemList.size() << " registered items." << std::endl;
    for (const auto& pair : itemList) {
        std::cout << "id: " << pair.first << " is " << pair.second->getName() << std::endl;
    }
}