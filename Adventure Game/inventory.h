//
// Created by William on 9/6/23.
//

#ifndef FRONTIER_GAME_INVENTORY_H
#define FRONTIER_GAME_INVENTORY_H
#include "weapons.h"

class Inventory {
    friend std::ostream& operator<<(std::ostream &os, const Inventory &inv) {
        os << "Inventory\n";
        for (const auto& pair : inv.items) {
            int slot = pair.first;
            int item_id = pair.second.first->ID();
            int count = pair.second.second;

            // Look up the item name in the global map
            //std::string item_name = getItem(item_id, items_all);

            //os << "Slot " << slot << ": " << item_name << " [x" << count << "]\n";
        }
        return os;
    }
public:
    Inventory();

    bool inventoryFull() const;

    bool addItemsInOrder(int id, int amount);

    bool addStackableItemsInOrder(int id, int amount);

    bool addNonStackableItemsInOrder(int id, int amount);

    bool addDefaultItemsInOrder(int id, int amount);

    bool addUniquesInOrder(int id, int amount);

    bool removeItemsInOrder(int id, int amount);

    void expandInventory(int slots);

    std::string merchantStyle();

    void show() const;

    // Assignment operator
    Inventory& operator=(const Inventory& other);

private:
    int max_slots;
    int used_slots;
    const int max_stack_size = 128;
    std::map<int, std::pair<std::shared_ptr<Object>, int>> items;  // Slot # -> {Object*, Count}
};

class Slots {
private:
    std::unique_ptr<Item> slots[11];
public:
    // default no-argument constructor
    Slots() {
        for (auto& slot : slots) {
            slot = nullptr;
        }
    }

    // single-argument constructor initializes right_hand and sets all other pointers to nullptr
    explicit Slots(std::unique_ptr<Item> right_hand_item) {
        slots[0] = std::move(right_hand_item); // Right hand is the first slot (index 0)
        for (int i = 1; i < 11; ++i) {
            slots[i] = nullptr;
        }
    }

    Item& hookSlotItem(int slot) {
        return *(slots[slot-1]); // SlotID starts from 1
    }

    static bool logic_games(int item_type, int desired_slot) {
        switch (desired_slot) {
            case 0:  // Right hand
            case 1:  // Left hand
                return true;

            case 2:  // Helmet
                return item_type == 6;

            case 3:  // Scarf
                return item_type == 7;

            case 4:  // Chestplate
                return item_type == 8;

            case 5:  // Rerebrace
                return item_type == 9;

            case 6:  // Leggings
                return item_type == 10;

            case 7:  // Right shoe
            case 8:  // Left shoe
                return item_type == 11;

            case 9:  // Right ring
            case 10: // Left ring
                return item_type == 12;

            default:
                return false;
        }
    }

    bool equip_item(std::unique_ptr<Item> item, int desired_slot);

    // Copy constructor
    Slots(const Slots& other) {
        for (int i = 0; i < 11; ++i) {
            if (other.slots[i]) {
                slots[i] = std::make_unique<Item>(*other.slots[i]);
            } else {
                slots[i] = nullptr;
            }
        }
    }

// Copy assignment operator
    Slots& operator=(const Slots& other) {
        if (this == &other) {
            return *this; // Handle self-assignment
        }

        // Deep copy
        for (int i = 0; i < 11; ++i) {
            if (other.slots[i]) {
                slots[i] = std::make_unique<Item>(*other.slots[i]);
            } else {
                slots[i] = nullptr;
            }
        }

        return *this;
    }
};

#endif //FRONTIER_GAME_INVENTORY_H
