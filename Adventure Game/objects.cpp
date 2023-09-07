//
// Created by M on 9/6/23.
//

#include "objects.h"

Object::Object(std::string given_name) : name(std::move(given_name)) {
    stackable = true;
    id = 0;
}