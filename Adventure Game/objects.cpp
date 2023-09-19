//
// Created by M on 9/6/23.
//

#include "objects.h"


Object::Object(int id, std::string given_name) : name(std::move(given_name)), id(id) {
    stackable = true;
    price = 0;
}

Item::Item(int id, std::string given_name) : Object(id, std::move(given_name)) {

}

void printAllItems() {
    std::map<int, std::shared_ptr<Object>> itemList = ItemsAllHook();
    std::cout << "There are currently " << itemList.size() << " registered items." << std::endl;
    for (const auto& pair : itemList) {
        std::cout << "id: " << pair.first << " is " << pair.second->getName() << std::endl;
    }
}