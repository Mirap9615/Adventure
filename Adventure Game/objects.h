//
// Created by M on 9/6/23.
//


#ifndef FRONTIER_GAME_OBJECTS_H
#define FRONTIER_GAME_OBJECTS_H

#include <string>
#include <iostream>
#include "qol.h"
#include <map>


class Object {
public:
    // Default constructor
    explicit Object(std::string  given_name = "None", std::string  given_description = "None");

    virtual void details() {
        std::cout << "The object exists." << std::endl;
    }

    void changeID(int newID) {
        id = newID;
    }

    int ID() {
        return id;
    };

protected:
    std::string name;
    int id;
    std::string description;
    bool stackable;
};

class Item : public Object  {
public:
    Item() : Object() {
        //std::cout << "An item was just created." << std::endl;
    }

    void details() override {
        std::cout << "The item exists." << std::endl;
    }
private:

};

class Weapon : public Item {
public:
    explicit Weapon(float damage_initial = 10, float durability_initial = 10, float bonus_damage_initial = 0, float boost_percentage_initial = 0);

    void details() override {
        std::cout << "This is a weapon. It gleams with destructive power." << std::endl;
    }

    float reportDamage() {
        return damage.effectiveValue();
    }

    void printStats() {
        std::cout << "Damage: " << damage << "\nDurability: " << durability << std::endl;
    }
private:
    Attribute damage;
    Attribute durability;
};


#endif //FRONTIER_GAME_OBJECTS_H
