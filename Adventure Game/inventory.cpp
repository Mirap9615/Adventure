//
// Created by William on 9/6/23.
//

#include "inventory.h"

Inventory::Inventory() : max_slots(8), used_slots(0) {}

bool Inventory::inventoryFull() const {
    return (used_slots >= max_slots);
}

void Inventory::addItems(int id, int amount, std::map<int, std::string>& items_all) {
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

void Inventory::removeItems(int id, int amount, std::map<int, std::string>& items_all) {
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

void Inventory::expandInventory(int slots) { max_slots += slots; }

Inventory& Inventory::operator=(const Inventory& other) {
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

bool Slots::equip_item(std::unique_ptr<Item> item, int desired_slot) {
    int item_type = item->getType();

    // play the logic games
    if (!logic_games(item_type, desired_slot)) {
        return false;
    }

    if (slots[desired_slot]) {
        // Handle existing item in the slot

        // For hand, foot, and ring slots, check the other slot of the same type
        if (desired_slot == 0 || desired_slot == 1) { // Hand slots
            int other_hand = (desired_slot == 0) ? 1 : 0;
            if (!slots[other_hand]) {
                slots[other_hand] = std::move(item);
                return true;
            }
        }
        else if (desired_slot == 7 || desired_slot == 8) { // Foot slots
            int other_foot = (desired_slot == 7) ? 8 : 7;
            if (!slots[other_foot]) {
                slots[other_foot] = std::move(item);
                return true;
            }
        }
        else if (desired_slot == 9 || desired_slot == 10) { // Ring slots
            int other_ring = (desired_slot == 9) ? 10 : 9;
            if (!slots[other_ring]) {
                slots[other_ring] = std::move(item);
                return true;
            }
        }
        else {
            return false;
        }

        return false;
    }

    // Equip the new item
    slots[desired_slot] = std::move(item);

    return true;
}