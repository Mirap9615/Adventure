//
// Created by M on 9/6/23.
//


#ifndef FRONTIER_GAME_OBJECTS_H
#define FRONTIER_GAME_OBJECTS_H
#include "qol.h"


class Object {
public:
    // Default constructor
    Object(int id, std::string given_name);

    void changeID(int newID) {
        id = newID;
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

    int getType() const {
        return type;
    }

    void setType(int desiredType) {
        type = desiredType;
    }

protected:
    std::string name;
    int id;
    int type;
    int price;
    std::string description;
    bool stackable;
};

class Item : public Object  {
public:
    Item(int id, std::string given_name);

private:

};

#endif //FRONTIER_GAME_OBJECTS_H
