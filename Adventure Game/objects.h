//
// Created by M on 9/6/23.
//


#ifndef FRONTIER_GAME_OBJECTS_H
#define FRONTIER_GAME_OBJECTS_H
#include "qol.h"

class Object {
public:
    // Default constructor
    Object(int id, std::string given_name, std::string description, const std::string& mainType, const std::string& subType);

    void changeID(int newID) {
        id = newID;
    }

    bool isStackable() const {
        return stackable;
    }

    void editDescription(const std::string& desired_message) {
        description = desired_message;
    }

    std::string getName() {
        return name;
    }

    int ID() const {
        return id;
    };

    int getMainType() const {
        return mainType;
    }

    std::string getSubType() const {
        return subType;
    }

    void setMainType(int desiredType) {
        mainType = desiredType;
    }

    void setSubType(const std::string& desiredType) {
        subType = desiredType;
    }


protected:
    std::string name;
    int id;
    int mainType;
    std::string subType;
    int price;
    std::string description;
    bool stackable;
};

class Item : public Object  {
public:
    Item(int id, std::string given_name, std::string description, const std::string& mainType, const std::string& subType);

    Item(int id, std::string given_name, std::string description, const std::string& mainType, const std::string& subType,
         float base_atk_dmg, float base_durability, float base_range, bool magical, int magic_type, int enchantability);
protected:
    Attribute damage;
    Attribute durability;
    Attribute range;
    bool magical;
    int magic_type;
    int enchantability;
private:

};

#endif //FRONTIER_GAME_OBJECTS_H
