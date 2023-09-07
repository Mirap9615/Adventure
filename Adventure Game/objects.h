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
    explicit Object(std::string  given_name = "None");

    void changeID(int newID) {
        id = newID;
    }

    void editDescription(const std::string& desired_message) {
        description = desired_message;
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
    Item() : Object() {}

private:

};




#endif //FRONTIER_GAME_OBJECTS_H
