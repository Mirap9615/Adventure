#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include "nlohmann/json.hpp"

// Global Variables
bool dev_mode = false;

// Pre declaration
class Attribute;
class Object;
class Item;
class Weapon;
class Organism;
class Magical;
class Sicut;
class Protagonist;
class Inventory;

std::map<int, std::string> items_all;

class Attribute {
    friend std::ostream &operator<<(std::ostream &os, const Attribute &rhs) {
        os << "current " << rhs.value << ", max " << rhs.max_value << ", bonus " << rhs.bonus_flat << ", enhanced by "
           << rhs.boost_percentage << "%" << ", netting " << rhs.effectiveValue();
        return os;
    }

    friend bool operator<(const Attribute &lhs, const Attribute &rhs) {
        if (lhs.type != rhs.type) {
            std::cerr << "Warning: do not compare apples to oranges! Defaulting to false.\n";
            return false;
        }
        if (lhs.effectiveValue() < rhs.effectiveValue()) {
            return true;
        }
        return false;
    }

public:
    // Constructor for two-input scenarios (for stats):
    Attribute(int type, float inValue) : type(type), value(inValue) {
        max_value = value;
        bonus_flat = 0;
        boost_percentage = 0;
    }

    // Constructor for multi-input scenarios (for vanity stats):
    Attribute(float inValue, float max_value_in, float bonus_flat_in, float bonus_percentage_in) {
        value = inValue;
        max_value = max_value_in;
        bonus_flat = bonus_flat_in;
        boost_percentage = bonus_percentage_in;
        type = 0;
    }


    float effectiveValue() const {
        //std::cout << "[value: " << value << "; bonus flat: " << bonus_flat << "; bonus percentage " << boost_percentage << "]";
        return (value + bonus_flat) * ((1 + boost_percentage) / 1);
    }

    Attribute& operator+=(float inValue) {
        value += inValue;
        return *this;
    }

    Attribute& operator=(float inValue) {
        value = inValue;
        return *this;
    }

    void changeType(int typeValue) {
        type = typeValue;
    }

private:
    int type; // 0 means unclassified, 1 to 9 are stat types
    // 10 is damage, 11 is durability
    float value;
    float max_value;
    float bonus_flat;
    float boost_percentage;
};

enum AttributeType {
    HP = 1, MANA = 2, STAMINA = 3, DEFENSE = 4, PHYS_ATK = 5, MAG_ATK = 6, SPD = 7, INTELLIGENCE = 8, LUCK = 9
};

class Stats {
    friend void changeAllTypes(Stats& stat) {
        stat.hp.changeType(1);
        stat.mana.changeType(2);
        stat.stamina.changeType(3);
        stat.defense.changeType(4);
        stat.phys_atk.changeType(5);
        stat.mag_atk.changeType(6);
        stat.spd.changeType(7);
        stat.intelligence.changeType(8);
        stat.luck.changeType(9);

    }
public:
    Stats() : hp(HP,100), mana(MANA,100), stamina(STAMINA,100),
    defense(DEFENSE,10), phys_atk(PHYS_ATK, 4), mag_atk(MAG_ATK, 0),
    spd(SPD, 100), intelligence(INTELLIGENCE ,100), luck(LUCK, 0) {}

    Attribute hp; Attribute mana; Attribute stamina; Attribute defense; Attribute phys_atk;
    Attribute mag_atk; Attribute spd; Attribute intelligence; Attribute luck;
private:

};

class Object {
public:
    explicit Object(std::string  given_name = "None", std::string  given_description = "None")
    : name(std::move(given_name)), description(std::move(given_description)) {
        stackable = true;
        //std::cout << "An object was just created." << std::endl;
    }
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
    explicit Weapon(float damage_initial = 10, float durability_initial = 10, float bonus_damage_initial = 0, float boost_percentage_initial = 0 )
    : damage(damage_initial, damage_initial, bonus_damage_initial, boost_percentage_initial),
    durability(durability_initial, durability_initial, 0, 0) {
        damage.changeType(10); durability.changeType(11);
        stackable = false;
    }

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

class Inventory {
    friend std::ostream& operator<<(std::ostream &os, const Inventory &inv) {
        os << "Inventory\n";
        for (const auto& pair : inv.items) {
            int slot = pair.first;
            int item_id = pair.second.first->ID();
            int count = pair.second.second;

            // Look up the item name in the global map
            std::string item_name = items_all[item_id];

            os << "Slot " << slot << ": " << item_name << " [x" << count << "]\n";
        }
        return os;
    }
public:
    Inventory() : max_slots(8), used_slots(0) {}

    bool inventoryFull() {
        return (used_slots >= max_slots);
    }

    // variation 1: id-based item adding
    void addItems(int id, int amount, std::map<int, std::string>& items_dict) {
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

    void removeItems(int id, int amount, std::map<int, std::string>& items_dict) {
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

private:
    int max_slots;
    int used_slots;
    const int max_stack_size = 128;
    std::map<int, std::pair<Object*, int>> items;  // Slot # -> {Object*, Count}
};

class Organism {
public:
    explicit Organism(std::string inputName) : name(std::move(inputName)), carcerisStrength(100, 100, 0, 0) {
    }

    virtual void behavior()  {
        std::cout << "As a organism, " << name << " is capable of running and rolling." << std::endl;
    }

    virtual void cast() {
        std::cout << "As a mere organism, " << name << " cannot cast any magic voluntarily." << std::endl;
    }

    virtual void increaseMagicalAtk(float inputValue) {
    }

    void addItemsToInventory(int id, int amount) {
        std::cout << "[" << name << "]" << " ";
        inventory.addItems(id, amount, items_all);
    }

    void removeItemsFromInventory(int id, int amount) {
        std::cout << "[" << name << "]" << " ";
        inventory.removeItems(id, amount, items_all);
    }

    void expandInventory(int amount) {
        inventory.expandInventory(amount);
    }

    void printInventory() {
        std::cout << inventory << std::endl;
    };

protected:
    bool magic_able = false;
    Attribute carcerisStrength;
    Stats stats;
    std::string name;
    Inventory inventory;
};

class Magical : public Organism {
public:
    explicit Magical(const std::string& given_name) : Organism(given_name) {
        magic_able = true;
        carcerisStrength = 150;
    }

    void behavior() override {
        std::cout << "As a magic-capable, " << name << " is capable of performing mindless magic, while running and rolling." << std::endl;
    }

    void cast() override {
        std::cout << "If " << name << " was to cast magic, they would average " << stats.mag_atk.effectiveValue() << " magical damage per spell." << std::endl;
    }

    void increaseMagicalAtk(float inputValue) override {
        stats.mag_atk += inputValue;
    }
private:
};

class Sicut : public Magical {
public:
    explicit Sicut(const std::string& given_name) : Magical(given_name), charm(100, 100, 0,0) {
        balance = 0;
        reputation = 0;
    }

    void behavior() override {
        std::cout << "As a sicut, " << name << " could potentially be performing mindful magic, while running. Probably would not roll, at least not in public." << std::endl;
    }

protected:
    double balance;
    double reputation;
    Attribute charm;
};

class Protagonist : public Sicut {
public:
    explicit Protagonist(const std::string& given_name) : Sicut(given_name), level(0) {}
    void behavior() override {
        std::cout << "As the protagonist, " << name << " is capable of performing world-changing feats, perhaps of magic, and potentially while running. Probably would not roll, maybe not even in private." << std::endl;
    }
private:
    double level;
    std::vector<std::string> current_quests;

};



void printAllItems(const std::map<int, std::string>& itemList) {
    std::cout << "There are currently " << itemList.size() << " registered items." << std::endl;

    for (const auto& pair : itemList) {
        std::cout << "id: " << pair.first << " is " << pair.second << std::endl;
    }
}

void loadItems() {
    std::ifstream file("items.json");
    nlohmann::json json;
    file >> json;

    for (const auto& item : json) {
        int id = item["id"];
        std::string name = item["name"];
        items_all[id] = name;
    }
}


int main() {
    loadItems();
    std::vector<std::unique_ptr<Organism>> organisms;

    printAllItems(items_all);

    std::cout << std::endl;
    std::unique_ptr<Organism> mae = std::make_unique<Protagonist>("Mae");
    std::unique_ptr<Organism> zebra = std::make_unique<Organism>("Zebra");
    std::unique_ptr<Organism> john = std::make_unique<Sicut>("John");

    organisms.push_back(std::move(mae));
    organisms.push_back(std::move(zebra));
    organisms.push_back(std::move(john));

    for (std::unique_ptr<Organism>& organism : organisms) {
        //organism->behavior();
        organism->cast();
    }
    organisms[0]->increaseMagicalAtk(15);
    for (std::unique_ptr<Organism>& organism : organisms) {
        //organism->behavior();
        organism->cast();
    }

    organisms[0]->addItemsToInventory(1, 50);
    organisms[0]->removeItemsFromInventory(2, 51);
    organisms[0]->addItemsToInventory(2, 50);
    organisms[0]->printInventory();

};

