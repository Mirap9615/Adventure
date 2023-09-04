#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>

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
public:
    Inventory() : max_slots(8), used_slots(0) {}

    bool inventoryFull() {
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


/*

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
     Weapon sword;
    sword.printStats();
    std::cout << std::endl;
    Weapon soul_sabre(50, 50);
    soul_sabre.printStats();
    std::cout << std::endl;
    Weapon enhanced_soul_sabre(50, 50, 25, 100);
    enhanced_soul_sabre.printStats();
    Attribute one(35,10,0,0);
   Attribute two(11,12,13,14);
   std::cout << (one < two) << std::endl;
   std::shared_ptr<Organism> entity1 = std::make_shared<Protagonist>("Mae");
    std::shared_ptr<Organism> entity2 = entity1;
    std::weak_ptr<Organism> entity3 = entity2;
*/

void printAllItems(const std::map<int, std::string>& itemList) {
    std::cout << "There are currently " << itemList.size() << " registered items." << std::endl;

    for (const auto& pair : itemList) {
        std::cout << "id: " << pair.first << " is " << pair.second << std::endl;
    }
}

int main() {
    std::vector<std::unique_ptr<Organism>> organisms;

    Object dirt("Dirt", "Some Dirt");
    items_all[1] = "Dirt";
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
};

