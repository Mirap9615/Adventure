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

