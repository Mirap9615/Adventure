//
// Created by M on 9/6/23.
//

#include "objects.h"

Object::Object(std::string given_name, std::string  given_description)
: name(std::move(given_name)), description(std::move(given_description)) {
stackable = true; }

Weapon::Weapon(float damage_initial, float durability_initial, float bonus_damage_initial, float boost_percentage_initial)
        : damage(damage_initial, damage_initial, bonus_damage_initial, boost_percentage_initial),
          durability(durability_initial, durability_initial, 0, 0) {
    damage.changeType(10); durability.changeType(11);
    stackable = false;
}