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



    static std::pair<int,int> determineType(const std::string& given_mainType, const std::string& given_subType);

    Object(const Object& other);

    Object& operator=(const Object& other);

    void changeID(int newID) {
        id = newID;
    }

    virtual bool isItem() {
        return false;
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

    int getSubType() const {
        return subType;
    }

    void setMainType(int desiredType) {
        mainType = desiredType;
    }

    void setSubType(int desiredSubType) {
        subType = desiredSubType;
    }


protected:
    std::string name;
    int id;
    int mainType;
    int subType;
    int price;
    std::string description;
    bool stackable;
};

class Item : public Object  {
public:
    Item(int id, std::string given_name, std::string description, const std::string& mainType, const std::string& subType);

    Item(int id, std::string given_name, std::string description, const std::string& mainType, const std::string& subType,
         float base_atk_dmg, float base_durability, float base_range, bool magical, int magic_type, int enchantability);

    bool isItem() override {
        return true;
    }

    // copy constructor
    Item(const Item& other);

    // assignment operator
    Item& operator=(const Item& other);

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
