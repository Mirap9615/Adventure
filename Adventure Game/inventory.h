//
// Created by William on 9/6/23.
//

#ifndef FRONTIER_GAME_INVENTORY_H
#define FRONTIER_GAME_INVENTORY_H
#include <iostream>
#include <map>
#include "objects.h"

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
    Inventory() : max_slots(8), used_slots(0) {}

    bool inventoryFull() const {
        return (used_slots >= max_slots);
    }

    // variation 1: id-based item adding
    void addItems(int id, int amount, std::map<int, std::string>& items_all) {
        // Variable to keep track of remaining items to add
        int remaining = amount;

        // First, try to add the item to an existing stack
        for (auto& pair : items) {
            if (pair.second.second == id) {
                // Found a stack with the same item ID
                int canAdd = max_stack_size - pair.second.second;

                if (canAdd >= remaining) {
                    // The stack can accommodate all the new items
                    pair.second.first += remaining;
                    return;
                } else {
                    // Partially fill this stack and update remaining
                    pair.second.second += max_stack_size;
                    remaining -= canAdd;
                }
            }
        }

        // Check if a new slot is available for the remaining items.
        if (!inventoryFull()) {
            // Check if id is valid
            if (items_all.find(id) != items_all.end()) {
                items[++used_slots] = std::make_pair(new Object(), remaining); // Create a new Object here
                items[used_slots].first->changeID(id);
                std::cout << "Item " << items_all[id] << " x " << amount << " was added successfully!\n";
            } else {
                std::cout << "Invalid item ID. Couldn't add item.\n";
            }
        } else {
            std::cout << "Inventory is full. Couldn't add remaining " << remaining << " items.\n";
        }
    }

    void removeItems(int id, int amount, std::map<int, std::string>& items_all) {
        int remaining = amount;

        // Try to remove the item from existing stacks, starting from the first slot
        for (auto it = items.begin(); it != items.end(); ) {
            if (it->second.first->ID() == id) {
                // Found a stack with the same item ID
                if (it->second.second >= remaining) {
                    // The stack has enough items to fulfill the request
                    it->second.second -= remaining;
                    std::cout << "Item " << items_all[id] << " x " << remaining << " was removed successfully!\n";
                    return;
                } else {
                    // Remove all items in this stack and update remaining
                    remaining -= it->second.second;
                    std::cout << "Item " << items_all[id] << " x " << it->second.second << " was removed successfully!\n";

                    // Remove this slot from the map and update used_slots
                    delete it->second.first;  // Free the Object*
                    it = items.erase(it);  // Remove the slot
                    --used_slots;
                    continue;  // Skip the iterator increment
                }
            }
            ++it;  // Move to the next slot
        }

        // If we reached here, there were not enough items to fulfill the request
        if (remaining > 0) {
            std::cout << "Could not remove all items. " << remaining << " items remaining.\n";
        }
    }

    void expandInventory(int slots) {
        max_slots += slots;
    }

    // Assignment operator
    Inventory& operator=(const Inventory& other) {
        // 1. Check self assignment
        if (this == &other) {
            return *this;
        }

        // 2. Copy simple member variables
        this->max_slots = other.max_slots;
        this->used_slots = other.used_slots;

        // 3. Clear existing items
        for (auto& item : this->items) {
            delete item.second.first;  // Deallocate Object* pointers
        }
        this->items.clear();

        // 4. Copy items from 'other' object
        for (const auto& item : other.items) {
            auto objCopy = new Object(*(item.second.first));  // Deep copy, assuming Object has a copy constructor
            this->items[item.first] = {objCopy, item.second.second};
        }

        return *this;
    }
private:
    int max_slots;
    int used_slots;
    const int max_stack_size = 128;
    std::map<int, std::pair<Object*, int>> items;  // Slot # -> {Object*, Count}
};

#endif //FRONTIER_GAME_INVENTORY_H
