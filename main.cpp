#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

// Global Variables
bool dev_mode = false;



class Attribute {
    friend std::ostream &operator<<(std::ostream &os, const Attribute &rhs) {
        os << "current " << rhs.value << ", max " << rhs.max_value << ", bonus " << rhs.bonus_flat << ", enhanced by "
           << rhs.boost_percentage << "%" << ", netting " << rhs.effectiveValue();
        return os;
    }

    friend bool operator<(const Attribute &lhs, const Attribute &rhs) {
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
        return (value + bonus_flat) * (1 + boost_percentage / 100.0);
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
    int type; // 0 means unclassified, 1 to 9 are stat typs
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

private:
    Attribute hp; Attribute mana; Attribute stamina; Attribute defense; Attribute phys_atk;
    Attribute mag_atk; Attribute spd; Attribute intelligence; Attribute luck;
};



class Object {
public:
    Object(const std::string& given_name = "None", const std::string& given_description = "None")
    : name(given_name), description(given_description) {
        //std::cout << "An object was just created." << std::endl;
    }
    virtual void details() {
        std::cout << "The object exists." << std::endl;
    }
protected:
    std::string name;
    int id;
    std::string description;
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
    Weapon(float damage_initial = 10, float durability_initial = 10, float bonus_damage_initial = 0, float boost_percentage_initial = 0 )
    : damage(damage_initial, damage_initial, bonus_damage_initial, boost_percentage_initial),
    durability(durability_initial, durability_initial, 0, 0) {
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

class Organism {
public:
    Organism(const std::string& inputName) : name(inputName) {
    }
private:
    Stats stats;
    std::string name;
};
/*
class Inventory {
public:
    int MAX_SLOTS = 8;
    static const int MAX_STACK_SIZE = 128;

    std::map<std::string, int> items;
    int totalSlots;

    Inventory() : totalSlots(0) {}

    bool addItem(const Item& item, int count = 1) {
        if (totalSlots >= MAX_SLOTS && items.find(item.name) == items.end()) {
            return false; // No free slots
        }

        int& itemCount = items[item.name];
        itemCount += count;
        if (item.stackable) {
            while (itemCount > MAX_STACK_SIZE) {
                itemCount -= MAX_STACK_SIZE;
                totalSlots++;
            }
        } else {
            totalSlots += count;
        }

        return true;
    }

    bool removeItem(const Item& item, int count = 1) {
        auto it = items.find(item.name);
        if (it == items.end() || it->second < count) {
            return false; // Item not found or insufficient quantity
        }

        it->second -= count;
        if (it->second == 0) {
            items.erase(it);
        }

        if (item.stackable) {
            totalSlots -= count / MAX_STACK_SIZE;
        } else {
            totalSlots -= count;
        }

        return true;
    }

    int checkItem(const Item& item) const {
        auto it = items.find(item.name);
        return it != items.end() ? it->second : 0;
    }

    std::string viewInventory() const {
        std::string inventoryText;
        for (const auto& itemPair : items) {
            inventoryText += itemPair.first + ": " + std::to_string(itemPair.second) + "\n";
        }
        return inventoryText;
    }

    void expandInventory(int slots) {
        totalSlots += slots;
    }

};

class Organism {
public:
    bool dead;
    int money;
    int level;
    std::string type;
    std::string habitat;
    std::vector<std::string> current_quests;
    Stats stats;
    Inventory inventory;

    Organism() : dead(false), money(0), level(0), type("None"), habitat("None") {}

    void set_stats(const std::vector<int>& attributes = {50, 10, 50, 1, 1, 0, 50, 50, 50}) {
        if (attributes.size() < 9) {
            // Error handling: insufficient number of attributes
            return;
        }
        stats.hp.value = stats.hp.max_value = attributes[0];
        stats.mana.value = stats.mana.max_value = attributes[1];
        stats.stamina.value = stats.stamina.max_value = attributes[2];
        stats.defense.value = stats.defense.max_value = attributes[3];
        stats.phys_atk.value = stats.phys_atk.max_value = attributes[4];
        stats.mag_atk.value = stats.mag_atk.max_value = attributes[5];
        stats.spd.value = stats.spd.max_value = attributes[6];
        stats.intelligence.value = stats.intelligence.max_value = attributes[7];
        stats.luck = attributes[8];
    }

    void viewAndConsumeItem() {
        std::cout << viewInventory() << "Select an item to view (enter index): ";
        int index;
        std::cin >> index;

        if (index < 0 || index >= inventory.items.size()) {
            std::cout << "Invalid index.\n";
            return;
        }

        auto it = inventory.items.begin();
        std::advance(it, index);
        const Item& item = it->first; // Assuming a map of Items to quantities

        std::cout << "Name: " << item.name << "\nDescription: " << item.description << '\n';
        if (item.consumable) {
            std::cout << "Do you want to consume this item? (y/n): ";
            char choice;
            std::cin >> choice;

            if (choice == 'y') {
                if (item.name == "Inventory Expander") {
                    inventory.expandInventory(8);
                }
                inventory.removeItem(item);
                std::cout << "Item consumed.\n";
            }
        }
    }

    std::string viewInventory() const {
        return inventory.viewInventory();
    }
};

void routeA() {
    Item invExpander("Inventory Expander", "Expands inventory by 8 slots", true, true);
    Organism player;
    player.inventory.addItem(invExpander, 5);

    player.viewAndConsumeItem();
    Organism mark;
    mark.stats.hp.getEffectiveValue();
*/


int main() {
    Weapon sword;
    sword.printStats();
    std::cout << std::endl;
    Weapon soul_sabre(50, 50);
    soul_sabre.printStats();
    std::cout << std::endl;
    Weapon enhanced_soul_sabre(50, 50, 25, 100);
    enhanced_soul_sabre.printStats();
}

