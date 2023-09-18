//
// Created by William on 9/6/23.
//

#ifndef FRONTIER_GAME_MONSTER_H
#define FRONTIER_GAME_MONSTER_H
#include "beings.h"

class Monster : public Magical {
public:
    // name, stats, type, habitat
    Monster(const std::string& given_name, const std::vector<float>& in_stats, std::string given_type, std::string  given_habitat)
            : Magical(given_name, in_stats), type(std::move(given_type)), habitat(std::move(given_habitat)) {
    }

    // Getter for attributes
    std::string getType() const {
        return type;
    }

    // Getter for habitat type
    std::string getHabitat() const {
        return habitat;
    }

    void behavior() override {
        Magical::behavior();  // Call base class behavior
        std::cout << "As a monster of type " << type << ", " << getName() << " resides in " << habitat << "." << std::endl;
    }

    // assignment operator overload
    Monster& operator=(const Monster& other) {
        if (this == &other) {
            return *this; // return *this to deal with self-assignment
        }

        // Copy the base class members
        Magical::operator=(other);

        // Copy the derived class members
        type = other.type;
        habitat = other.habitat;

        return *this;
    }

protected:
    std::string type;     // Monster type (e.g., "slime", "humanoid", etc.)
    std::string habitat;  // Habitat where the monster is found (e.g., "forest", "ocean", etc.)
};

#endif //FRONTIER_GAME_MONSTER_H
