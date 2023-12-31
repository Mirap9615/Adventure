//
// Created by William on 9/6/23.
//

#include "inventory.h"

Inventory::Inventory() : max_slots(8), used_slots(0) {}

bool Inventory::inventoryFull() const {
    return (used_slots >= max_slots);
}

void Inventory::show() const {
    std::string border(70, '=');
    std::cout << border << std::endl;

    for (int slot = 0; slot < max_slots; slot += 2) {
        std::string leftItem, rightItem;

        // Process left item
        auto it = items.find(slot);
        if (it != items.end() && it->second.first) {
            std::string itemName = it->second.first->getName();
            if (itemName.length() > 30) {
                itemName = itemName.substr(0, 27) + "...";
            }
            leftItem = "[" + std::to_string(slot + 1) + "]: " + itemName + " {x" + std::to_string(it->second.second) + "}";
        } else {
            leftItem = "[" + std::to_string(slot + 1) + "]: empty";
        }
        leftItem = leftItem + std::string(35 - leftItem.length(), ' ');

        // Process right item
        it = items.find(slot + 1);
        if (it != items.end() && it->second.first) {
            std::string itemName = it->second.first->getName();
            if (itemName.length() > 30) {
                itemName = itemName.substr(0, 27) + "...";
            }
            rightItem = "[" + std::to_string(slot + 2) + "]: " + itemName + " {x" + std::to_string(it->second.second) + "}";
        } else {
            rightItem = "[" + std::to_string(slot + 2) + "]: empty";
        }

        std::cout << leftItem << "|   " << rightItem << std::endl;
    }

    std::cout << border << std::endl;
}


bool Inventory::addItemsInOrder(int id, int amount) {
    if (id > 0) {
        std::cout << all_items[id]->getName() << " is stackable.";
        return addDefaultItemsInOrder(id, amount);
    } else {
        return false;
    }
}

bool Inventory::addStackableItemsInOrder(int id, int amount) {
    // Variable to keep track of remaining items to add
    int remaining = amount;

    // Tries to find an existing stack with space
    for (auto& pair : items) {
        if (pair.second.first->ID() == id) {
            // Found a stack with the same item ID
            int canAdd = max_stack_size - pair.second.second;

            if (canAdd >= remaining) {
                // The stack can accommodate all the new items
                pair.second.second += remaining;
                return true;
            } else {
                // Partially fill this stack and update remaining
                pair.second.second = max_stack_size;
                remaining -= canAdd;
            }
        }
    }

    while (!inventoryFull()) {
        int curr_slot = used_slots;
        if (remaining >= max_stack_size) {
            remaining -= max_stack_size;
            items[curr_slot] = std::make_pair(all_items[id], max_stack_size);
            used_slots++;
        } else if (remaining > 0) {
            items[curr_slot] = std::make_pair(all_items[id], remaining);
            remaining = 0;
            used_slots++;
        }
        if (remaining == 0) {
            return true;
        }
    }
    std::cout << "Inventory is full. Couldn't add remaining " << remaining << " items.\n";
    return false;
}

bool Inventory::addNonStackableItemsInOrder(int id, int amount) {
    // Variable to keep track of remaining items to add
    std::cout << " we are here." << std::endl;
    int remaining = amount;

    // Since these items are non stackable, we only need to find new slots
    while (!inventoryFull()) {
        int curr_slot = used_slots;
        if (remaining > 0) {
            remaining -= 1;
            items[curr_slot] = std::make_pair(all_items[id], 1);
            used_slots++;
        }
        if (remaining == 0) {
            return true;
        }
    }
    std::cout << "Inventory is full. Couldn't add remaining " << remaining << " items.\n";
    return false;
}

bool Inventory::addDefaultItemsInOrder(int id, int amount) {
    /*
     * Tries to add the given item to an existing stack with space, but if that does not exist,
     * finds the first available slot to add the item into. If inventory full, we say that.
     */

    // Trivial case: unmatchable id
    if (all_items.find(id) == all_items.end()) {
        std::cout << "Unable to add item id = " << id << " , bad id\n";
        return false;
    }

    if (all_items[id]->isStackable()) {
        return addStackableItemsInOrder(id, amount);
    } else {
        return addNonStackableItemsInOrder(id, amount);
    }
}

bool Inventory::removeItemsInOrder(int id, int amount) {
    int remaining = amount;

    // Try to remove the item from existing stacks, starting from the first slot
    for (auto it = items.begin(); it != items.end(); ) {
        if (it->second.first->ID() == id) {
            // Found a stack with the same item ID
            if (it->second.second >= remaining) {
                // The stack has enough items to fulfill the request
                it->second.second -= remaining;
                std::cout << "Item " << all_items[id]->getName() << " x " << remaining << " was removed successfully!\n";

                // Check if stack is empty
                if (it->second.second == 0) {
                    it = items.erase(it);
                    --used_slots;
                }
                return true;
            } else {
                // Remove all items in this stack and update remaining
                remaining -= it->second.second;
                std::cout << "Item " << all_items[id]->getName() << " x " << it->second.second << " was removed successfully!\n";

                // Remove this slot from the map and update used_slots
                it = items.erase(it);  // Remove the slot and get the iterator to the next element
                --used_slots;
                continue;  // Skip the iterator increment
            }
        } else {
            ++it;  // Move to the next slot
        }
    }

    // If we reached here, there were not enough items to fulfill the request
    if (remaining > 0) {
        std::cout << "Could not remove all items. " << remaining << " items remaining.\n";
        return false;
    }
    return true;
}


void Inventory::expandInventory(int slots) { max_slots += slots; }

// Assignment operator overload
Inventory& Inventory::operator=(const Inventory& other) {
    // 1. Check self assignment
    if (this == &other) {
        return *this;
    }

    // 2. Copy simple member variables
    this->max_slots = other.max_slots;
    this->used_slots = other.used_slots;

    // 3. Clear existing items (smart pointer so no need to use del)
    this->items.clear();

    // 4. Copy items from 'other' object one by one
    for (const auto& item : other.items) {
        auto objCopy = std::make_shared<Object>(*(item.second.first));  // Deep copy
        this->items[item.first] = {objCopy, item.second.second};
    }

    return *this;
}

bool Slots::equip_item(const std::shared_ptr<Object>& potential_item_from_inventory, int desired_slot) {
    std::shared_ptr<Item> itemPtr = std::dynamic_pointer_cast<Item>(potential_item_from_inventory);

    // trivial case
    if (!itemPtr) {
        // Failed cast, potential_item_from_inventory is not an item
        return false;
    }

    // first step: least expensive: logic games
    if (!logic_games(potential_item_from_inventory->getMainType(), potential_item_from_inventory->getSubType(),
                     desired_slot)) {
        // Failed logic games
        return false;
    }

    // Slot is full, but if the desired slot is a hand, foot or ring slot, we check if the other slot of the same type is empty.
    // If it is, then we equip the object to that slot instead.
    if (slots[desired_slot]) {

        if (desired_slot == 0 || desired_slot == 1) { // Hand slots
            int other_hand = (desired_slot == 0) ? 1 : 0;
            if (!slots[other_hand]) {
                slots[other_hand] = std::make_unique<Item>(*itemPtr);
                return true;
            }
        } else if (desired_slot == 7 || desired_slot == 8) { // Foot slots
            int other_foot = (desired_slot == 7) ? 8 : 7;
            if (!slots[other_foot]) {
                slots[other_foot] = std::make_unique<Item>(*itemPtr);
                return true;
            }
        } else if (desired_slot == 9 || desired_slot == 10) { // Ring slots
            int other_ring = (desired_slot == 9) ? 10 : 9;
            if (!slots[other_ring]) {
                slots[other_ring] = std::make_unique<Item>(*itemPtr);
                return true;
            }
        }
        return false;
    } else {
        slots[desired_slot] = std::make_unique<Item>(*itemPtr);
        return true;
    }
}


std::string Inventory::merchantStyle() {
    items[0];
    return "";
}

void Slots::show() const {
    std::string border(70, '=');
    std::cout << border << std::endl;

    // Define slot names
    std::vector<std::string> slotNames = {"Left Hand", "Right Hand", "Helmet", "Scarf", "Chestplate",
                                          "Rerebrace", "Leggings", "Right Shoe", "Left Shoe", "Right Ring", "Left Ring"};

    // Re-arrange the indices in the desired order
    std::vector<int> slotOrder = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    for (size_t i = 0; i < slotOrder.size(); i += 2) {
        std::string leftSlot, rightSlot;

        // Process left slot
        int leftIndex = slotOrder[i];
        if (slots[leftIndex]) {
            std::string itemName = slots[leftIndex]->getName();
            if (itemName.length() > 30) {
                itemName = itemName.substr(0, 27) + "...";
            }
            leftSlot = "[" + slotNames[leftIndex] + "]: " + itemName;
        } else {
            leftSlot = "[" + slotNames[leftIndex] + "]: None";
        }
        leftSlot = leftSlot + std::string(35 - leftSlot.length(), ' ');

        // Process right slot
        int rightIndex = slotOrder[i + 1];
        if (slots[rightIndex]) {
            std::string itemName = slots[rightIndex]->getName();
            if (itemName.length() > 30) {
                itemName = itemName.substr(0, 27) + "...";
            }
            rightSlot = "[" + slotNames[rightIndex] + "]: " + itemName;
        } else {
            rightSlot = "[" + slotNames[rightIndex] + "]: None";
        }

        std::cout << leftSlot << "|   " << rightSlot << std::endl;
    }

    std::cout << border << std::endl;
}
