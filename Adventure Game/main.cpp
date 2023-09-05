#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include "nlohmann/json.hpp"
#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>

// Global Variables
bool dev_mode = true;

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
class Party;

std::map<int, std::string> items_all;

float clamp(float value, float min, float max) {
    return std::max(min, std::min(max, value));
}

enum PartyRole {
    INITIATE,
    MEMBER,
    OFFICER,
    LEADER,
    RULER
};


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
        max_value = inValue;
        return *this;
    }

    void changeType(int typeValue) {
        type = typeValue;
    }

    Attribute& operator-=(float inValue) {
        value -= inValue;
        return *this;
    }

    float max() const {
        return max_value;
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
    HP = 1, MANA = 2, STAMINA = 3, DEFENSE = 4, PHYS_ATK = 5, MAG_ATK = 6, SPD = 7, INTELLIGENCE = 8, LUCK = 9,
    CRIT_CHANCE = 10,CRIT_DAMAGE = 11,
};

float calculatePower(float hp, float mana, float stamina, float defense, float phys_atk, float mag_atk, float speed, float intelligence) {
    float unified1 = (5 * mag_atk * (mana / 30) + phys_atk) * std::pow(1.5f, (speed / 100) - 1);
    float unified2 = unified1 * (stamina / 100) + (hp + 3 * defense) * std::pow(1.1f, stamina / 100);

    return (unified2 * std::pow(2, (intelligence - 100) / 20) - 1) / 10;
}

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
        stat.cc.changeType(10);
        stat.cd.changeType(11);

    }
public:
    Stats() : hp(HP,100), mana(MANA,100), stamina(STAMINA,100),
    defense(DEFENSE,10), phys_atk(PHYS_ATK, 4), mag_atk(MAG_ATK, 0),
    spd(SPD, 100), intelligence(INTELLIGENCE ,100), luck(LUCK, 0),
    cc(CRIT_CHANCE, 5), cd(CRIT_DAMAGE, 50) {}

    float power() const {
        return calculatePower(hp.effectiveValue(), mana.effectiveValue(),stamina.effectiveValue(),
                              defense.effectiveValue(),phys_atk.effectiveValue(),mag_atk.effectiveValue(),
                              spd.effectiveValue(), intelligence.effectiveValue());
    }

    Attribute hp; Attribute mana; Attribute stamina; Attribute defense; Attribute phys_atk;
    Attribute mag_atk; Attribute spd; Attribute intelligence; Attribute luck; Attribute cc; Attribute cd;

    void board_change(const std::vector<float>& in_stats) {
        Attribute* attributes[] = {&hp, &mana, &stamina, &defense, &phys_atk, &mag_atk, &spd, &intelligence, &luck, &cc, &cd};
        size_t numAttributes = sizeof(attributes) / sizeof(attributes[0]);

        for (size_t i = 0; i < std::min(in_stats.size(), numAttributes); ++i) {
            *(attributes[i]) = in_stats[i];
        }
    }
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

    bool inventoryFull() const {
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


float calculateMercifulDamage(float baseDamage, float opponentHealth) {
    float actualDamage = baseDamage;
    while (opponentHealth - actualDamage <= 0 && actualDamage >= 1) {
        actualDamage /= 2;
    }
    return actualDamage;
}


class Organism {
    friend Party;
public:
    explicit Organism(std::string inputName) : name(std::move(inputName)), carcerisStrength(100, 100, 0, 0), level(0),
                                               dead(false), balance(0) {
    }

    Organism(std::string inputName, const std::vector<float> &in_stats) : Organism(std::move(inputName)) {
        stats.board_change(in_stats);
    }

    std::string getName() {
        return name;
    }

    virtual void behavior() {
        if (dead) return;
        std::cout << "As a organism, " << name << " is capable of running and rolling." << std::endl;
    }

    virtual void cast() {
        if (dead) return;
        std::cout << "As a mere organism, " << name << " cannot cast any magic voluntarily." << std::endl;
    }

    virtual void increaseMagicalAtk(float inputValue) {
        if (dead) return;
    }

    void addItemsToInventory(int id, int amount) {
        if (dead) return;
        std::cout << "[" << name << "]" << " ";
        inventory.addItems(id, amount, items_all);
    }

    void removeItemsFromInventory(int id, int amount) {
        if (dead) return;
        std::cout << "[" << name << "]" << " ";
        inventory.removeItems(id, amount, items_all);
    }

    void expandInventory(int amount) {
        if (dead) return;
        inventory.expandInventory(amount);
    }

    void printInventory() {
        if (dead) return;
        std::cout << inventory << std::endl;
    };

    bool checkForMercy(Organism &opponent) {
        float opponentHealth = opponent.stats.hp.effectiveValue();
        float opponentMaxHealth = opponent.stats.hp.max();
        float selfHealth = stats.hp.effectiveValue();

        float threshold = std::max(5.0f, opponentMaxHealth *
                                         0.1f);  // Threshold for low health: 5 or 10% of max health, whichever is higher

        // Check for the condition to let go
        if (opponentHealth <= threshold && opponentHealth < 0.5 * selfHealth) {
            std::cout << name << " let " << opponent.name << " go in their pitiful state, only having "
                      << opponentHealth << " health out of " << opponentMaxHealth << " health.\n";
            opponent.stats.hp = threshold;  // Optionally, set the opponent's health to the threshold value
            return true;
        }
        return false;
    }

    void fight(Organism &rhs) {
        if (dead) return;
        float power_lhs = this->stats.power();
        float power_rhs = rhs.stats.power();
        std::cout << this->name << " versus " << rhs.name << std::endl;

        if (power_lhs > power_rhs) {
            std::cout << this->name << " wins the fight.\n";
        } else if (power_rhs > power_lhs) {
            std::cout << rhs.name << " wins the fight.\n";
        } else {
            std::cout << "It's a draw.\n";
        }
        stats.spd.effectiveValue();
    }

    float normal_attack_damage() const {
        return (this->stats.mag_atk.effectiveValue() > this->stats.phys_atk.effectiveValue()) ?
               this->stats.mag_atk.effectiveValue() : this->stats.phys_atk.effectiveValue();
    }

    float calculate_crit(float normal_attack_damage) {
        // RNG setup
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dis(0.0, 100.0); // 0 to 100%

        float crit_chance_lhs =
                this->stats.cc.effectiveValue() + ((this->stats.intelligence.effectiveValue() - 100) / 10);

        if (dis(gen) < crit_chance_lhs) {
            // Critical hit
            normal_attack_damage *= 1 + this->stats.cd.effectiveValue() / 100.0f;
        }
        return normal_attack_damage;
    }

    void turn_combat(Organism &rhs, int mode = 0) {
        // mode 0 = fight to the death, mode 1 = fight to 30% hp.
        if (dead || rhs.dead) return;
        // RNG setup
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dis(0.0, 100.0); // 0 to 100%

        float progress_lhs = this->stats.spd.effectiveValue();
        float progress_rhs = rhs.stats.spd.effectiveValue();

        float power_lhs = this->stats.power();
        float power_rhs = rhs.stats.power();

        std::cout << this->name << "[" << int(power_lhs) << " power] versus " << rhs.name << "[" << int(power_rhs)
                  << " power] " << std::endl;

        while (this->stats.hp.effectiveValue() > 0 && rhs.stats.hp.effectiveValue() > 0) {
            checkForMercy(rhs);
            rhs.checkForMercy(*this);
            progress_lhs += this->stats.spd.effectiveValue();
            progress_rhs += rhs.stats.spd.effectiveValue();

            if (dev_mode) {
                std::cout << "[" << this->name << " Progress: " << progress_lhs << ", HP: "
                          << this->stats.hp.effectiveValue() << "] ";
                std::cout << "[" << rhs.name << " Progress: " << progress_rhs << ", HP: "
                          << rhs.stats.hp.effectiveValue() << "]\n";
            }

            if (progress_lhs >= 1000) {
                // This organism attacks
                float damage = calculate_crit(normal_attack_damage());

                if (mode == 1) {
                    float mercifulDamage = calculateMercifulDamage(damage, rhs.stats.hp.effectiveValue());
                    rhs.stats.hp -= mercifulDamage;
                    if (mercifulDamage != damage) {
                        std::cout << this->name << " attacked " << rhs.name << " with mercy, only dealing "
                                  << mercifulDamage << " damage! (normal: " << damage << ")\n";
                    } else {
                        std::cout << this->name << " attacks " << rhs.name << " for " << damage << " damage.\n";
                    }
                } else {
                    rhs.stats.hp -= damage;
                }

                progress_lhs = 0;
            }


            if (progress_rhs >= 1000) {
                // The opponent attacks

                float damage = rhs.calculate_crit(rhs.normal_attack_damage());

                if (mode == 1) {
                    float mercifulDamage = calculateMercifulDamage(damage, stats.hp.effectiveValue());
                    stats.hp -= mercifulDamage;
                    if (mercifulDamage != damage) {
                        std::cout << rhs.name << " attacked " << name << " with mercy, only dealing "
                                  << mercifulDamage << " damage! (normal: " << damage << ")\n";
                    } else {
                        std::cout << rhs.name << " attacks " << this->name << " for " << damage << " damage.\n";
                    }
                } else {
                    stats.hp -= damage;
                }
                progress_rhs = 0;
            }

            if (mode == 0) {
                // Declare the winner and loser
                if (this->stats.hp.effectiveValue() <= 0 && rhs.stats.hp.effectiveValue() <= 0) {
                    std::cout << "Both " << this->name << " and " << rhs.name << " have been defeated.\n";
                    if (rhs.checkIfDead() && checkIfDead()) return;
                    else {
                        rhs.assignXPGainAndPrint(power_lhs, power_rhs);
                        assignXPGainAndPrint(power_rhs, power_lhs);
                    }
                } else if (this->stats.hp.effectiveValue() <= 0) {
                    checkIfDead();
                    rhs.balance += balance;
                    balance = 0;
                    std::cout << rhs.name << " wins and " << this->name << " loses.\n";
                    rhs.assignXPGainAndPrint(power_lhs, power_rhs);

                } else if (rhs.stats.hp.effectiveValue() <= 0) {
                    rhs.checkIfDead();
                    balance += rhs.balance;
                    rhs.balance = 0;
                    std::cout << this->name << " wins and " << rhs.name << " loses.\n";
                    assignXPGainAndPrint(power_rhs, power_lhs);

                }
            }
            if (mode == 1) {
                // 30% = end fight
                if ((this->stats.hp.effectiveValue() <= this->stats.hp.max() * 0.3 ||
                     rhs.stats.hp.effectiveValue() <= rhs.stats.hp.max() * 0.3
                     || this->stats.hp.effectiveValue() < 3 || rhs.stats.hp.effectiveValue() < 3)) {
                    // Someone's HP fell below 30% in mode 1
                    if (this->stats.hp.effectiveValue() <= this->stats.hp.max() * 0.3 ||
                        this->stats.hp.effectiveValue() < 3) {
                        // this organism lost in mode 1
                        std::cout << rhs.name << " wins and takes all the balance from " << this->name << ".\n";
                        rhs.balance += balance;
                        balance = 0;
                        checkIfDead();
                        return;
                    } else {
                        // rhs organism lost in mode 1
                        std::cout << this->name << " wins and takes all the balance from " << rhs.name << ".\n";
                        balance += rhs.balance;
                        rhs.balance = 0;
                        rhs.checkIfDead();
                        return;
                    }
                }
            }
        }
    }


    void attemptToPickpocket(Organism &rhs) {
        if (dead) return;
        if (rhs.dead) {
            std::cout << name << " just attempted to pickpocket a dead corpse!" << std::endl;
            return;
        }
        // Initialize the random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 1); // generate a float between 0 and 1

        // Calculate the base success rate
        float baseSuccessRate = 0.2;  // 20%

        // Modify success rate based on intelligence difference
        float successRate = baseSuccessRate + 0.005f * (this->stats.intelligence.effectiveValue() -
                                                        rhs.stats.intelligence.effectiveValue());
        successRate = clamp(successRate, 0.0f, 1.0f); // Make sure it stays between 0 and 1

        // Attempt the pickpocket
        if (dis(gen) < successRate) {
            // Successful pickpocket
            this->balance += rhs.balance;
            rhs.balance = 0;
            std::cout << this->name << " successfully pickpocketed " << rhs.balance << " from " << rhs.name << "!\n";
        } else {
            // Failed pickpocket
            std::cout << this->name << " failed to pickpocket " << rhs.name << ".\n";

            // Determine if combat is initiated
            if (dis(gen) < 0.4) {
                // 40% chance to initiate combat
                std::cout << rhs.name << " caught " << this->name << " and initiates combat!\n";
                rhs.turn_combat(*this, 1);
            } else {
                // 60% chance to let go without consequence
                std::cout << rhs.name << " let " << this->name << " go without consequence.\n";
            }
        }
    }


    float calculatePower() const {
        if (dead) return 0;
        return stats.power();
    }

    // Polynomial function to calculate XP required for next level
    float xpRequiredForLevelUp() const {
        if (dead) return 0;
        float a = 0.1, b = 10, c = 40;
        return a * std::pow(level, 2) + b * level + c;
    }

    void addXP(float xpGain) {
        if (dead) return;
        xp += xpGain;

        // Check for level up
        while (xp >= xpRequiredForLevelUp()) {
            // elegant implementation allows for multiple level-ups at once
            xp -= xpRequiredForLevelUp();
            level++;
        }
    }

    float calculateXPGain(float opponentPower, float selfPower) const {
        if (dead) return 0;
        float baseXP = opponentPower;

        // Unbalanced matchup penalty
        if (selfPower > 5 * opponentPower) {
            float k = 0.2;  // This constant can be tuned
            float reductionFactor = std::exp(-k * (selfPower / opponentPower - 5));
            baseXP *= reductionFactor;
        }

        return baseXP;
    }

    void assignXPGainAndPrint(float opponentPower, float selfPower) {
        if (dead) return;
        float gainedXP = calculateXPGain(opponentPower, selfPower);
        addXP(gainedXP);
        std::cout << name << " gained " << gainedXP << " xp! ";
        std::cout << "[Level " << level << ", " << xp << "/" << xpRequiredForLevelUp() << " to level " << (level + 1)
                  << "]" << std::endl;
    }

    bool checkIfDead() {
        if (dead) return true;
        if (stats.hp.effectiveValue() <= 0) {
            std::cout << name << " has died!" << std::endl;
            dead = true;
            return true;
        }
        return false;
    }

    void fullHeal() {
        float beforeHealth = stats.hp.effectiveValue();
        stats.hp = stats.hp.max();

        if (dead) {
            std::cout << name << " was dead, but now they have " << stats.hp.max() << "/" << stats.hp.max()
                      << " health!" << std::endl;
            dead = false;
        } else {
            float afterHealth = stats.hp.max();
            float healthGain = afterHealth - beforeHealth;
            std::cout << name << " had " << beforeHealth << " health, but now they have " << afterHealth << "/"
                      << stats.hp.max() << " health! (+" << healthGain << ")" << std::endl;
        }
    }

    float currentPower() {
        return stats.power();
    }

    void attackOpponent(Organism &opponent, int mode) {
        // This is an example using some hypothetical member variables and methods
        float damage = calculate_crit(normal_attack_damage());

        if (mode == 1) {
            float mercifulDamage = calculateMercifulDamage(damage, opponent.stats.hp.effectiveValue());
            opponent.stats.hp -= mercifulDamage;

            // Only display "with mercy" if the merciful damage is less than the original damage
            if (mercifulDamage < damage) {
                std::cout << this->name << " attacked " << opponent.name << " with mercy, only dealing "
                          << mercifulDamage << " damage! (normal: " << damage << ")\n";
            } else {
                std::cout << this->name << " attacks " << opponent.name << " for " << mercifulDamage << " damage.\n";
            }
        } else {
            opponent.stats.hp -= damage;
            std::cout << this->name << " attacks " << opponent.name << " for " << damage << " damage.\n";
        }
    }






protected:
    bool magic_able = false;
    bool dead;
    Attribute carcerisStrength;
    Stats stats;
    std::string name;
    Inventory inventory;
    int level;
    float xp;
    float balance;
};

class Magical : public Organism {
public:
    explicit Magical(const std::string& given_name) : Organism(given_name) {
        magic_able = true;
        carcerisStrength = 150;
    }
    Magical(std::string inputName, const std::vector<float>& in_stats) : Organism(std::move(inputName)) {
        stats.board_change(in_stats);
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
    Sicut(const std::string& inputName, const std::vector<float>& in_stats) : Magical(inputName), charm(100, 100, 0, 0) {
        stats.board_change(in_stats);
    }

    void behavior() override {
        std::cout << "As a sicut, " << name << " could potentially be performing mindful magic, while running. Probably would not roll, at least not in public." << std::endl;
    }

protected:
    double reputation;
    Attribute charm;
};

class Protagonist : public Sicut {
public:
    explicit Protagonist(const std::string& given_name) : Sicut(given_name) {}
    Protagonist(const std::string& inputName, const std::vector<float>& in_stats) : Sicut(inputName) {
        stats.board_change(in_stats);
    }

    void behavior() override {
        std::cout << "As the protagonist, " << name << " is capable of performing world-changing feats, perhaps of magic, and potentially while running. Probably would not roll, maybe not even in private." << std::endl;
    }
private:
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

static std::unique_ptr<Organism> createOrganism(const int type, const std::string& name, const std::vector<float>& stats = {}) {
    if (type == 0) { // Protagonist
        return std::make_unique<Protagonist>(name, stats);
    } else if (type == 1) { // Sicut
        return std::make_unique<Magical>(name, stats);
    } else if (type == 2) { // Magical
        return std::make_unique<Magical>(name, stats);
    } else { // Organism
        return std::make_unique<Organism>(name, stats);
    }
}

static bool compareByRank(const std::pair<Organism*, PartyRole>& a, const std::pair<Organism*, PartyRole>& b) {
    return a.second > b.second;
}

std::string partyRoleToString(PartyRole role) {
    switch(role) {
        case INITIATE:
            return "Initiate";
        case MEMBER:
            return "Member";
        case OFFICER:
            return "Officer";
        case LEADER:
            return "Leader";
        case RULER:
            return "Ruler";
        default:
            return "Unknown";
    }
}

class Party {
private:
    std::vector<std::pair<Organism*, PartyRole>> members;
    std::string name;
    std::vector<std::string> history;

public:
    // Constructor
    Party(std::string input_name ) :name(std::move(input_name)) {}

    // Add a member to the party with a specific role

    void addMember(Organism* organism, PartyRole role = INITIATE) {
        members.emplace_back(organism, role);
        std::stringstream ss;
        ss << "Member added: " << organism << " as " << role;
        history.push_back(ss.str());
    }

    // Remove a member from the party
    void removeMember(Organism* organism) {
        members.erase(std::remove_if(members.begin(), members.end(),
                                     [&](const std::pair<Organism*, PartyRole>& member) {
                                         return member.first == organism;
                                     }),
                      members.end());
    }

    // Check if an organism is in the same party
    bool isInSameParty(Organism* organism) {
        for (const auto& member : members) {
            if (member.first == organism) {
                return true;
            }
        }
        return false;
    }

    // Method for promoting a member
    void promoteMember(Organism* issuer, Organism* target, PartyRole newRole) {
        for (auto & it : members) {
            Organism* curr_name = it.first;
            PartyRole& role = it.second;

            // Issuer must have a higher rank and cannot promote beyond their own rank
            if (curr_name == issuer && role > MEMBER && role >= newRole) {
                for (auto & member : members) {
                    Organism* curr_target = member.first;
                    PartyRole& curr_role = member.second;

                    if (curr_target == target && curr_role < role) {
                        curr_role = newRole;
                        std::cout << issuer->getName() << " promoted " << target->getName() << " to " << newRole << std::endl;
                        std::stringstream ss;
                        ss << issuer->getName() << " promoted " << target->getName() << " to " << newRole;
                        history.push_back(ss.str());
                        return;
                    }
                }
            }
        }
        std::cout << "Cannot promote: Either issuer or target not found, or issuer doesn't have the authority." << std::endl;
    }

    void removeMember(Organism* issuer, Organism* target) {
        for (auto it = members.begin(); it != members.end(); ++it) {
            Organism* member = it->first;
            PartyRole& role = it->second;

            if (member == issuer && role > MEMBER) {
                for (auto it2 = members.begin(); it2 != members.end(); ++it2) {
                    Organism* mem = it2->first;
                    PartyRole& rol = it2->second;

                    if (mem == target && rol < role) {
                        members.erase(it2);
                        std::cout << "Member " << target << " was removed by " << issuer << std::endl;
                        std::stringstream ss;
                        ss << issuer << " removed " << target;
                        history.push_back(ss.str());
                        return;
                    }
                }
            }
        }
        std::cout << "Cannot remove: Either issuer or target not found, or issuer doesn't have a high enough rank." << std::endl;
    }

    // Party merger
    Party& operator+=(Party& rhs) {
        for (auto& it : rhs.members) {
            Organism* member = it.first;
            PartyRole role = it.second;
            this->members.emplace_back(std::make_pair(member, role));
        }
        std::stringstream ss;
        ss << "Merged party " << rhs.name << " into " << this->name;
        history.push_back(ss.str());
        std::cout << "Merged party " << rhs.name << " into " << this->name << std::endl;
        rhs.members.clear(); // Clear the rhs party's member list
        return *this;
    }


    // Get the role of a party member
    PartyRole getRole(Organism* organism) {
        for (const auto& member : members) {
            if (member.first == organism) {
                return member.second;
            }
        }
        return INITIATE;  // Default if not found
    }

    void listMembers() {
        // Sort members by rank
        std::sort(members.begin(), members.end(), compareByRank);

        std::cout << "Members of Party " << name << ":" << std::endl;
        for (const auto& member : members) {
            std::cout << member.first->getName() << " (" << partyRoleToString(member.second) << ")" << std::endl;
        }
    }

    // Deep breaths everyone.
    bool party_duel(Party& rhs, int mode = 0) {
        std::vector<Organism*> partyA, partyB;

        // Populate partyA and partyB vectors with organisms from both parties (up to 4 each)
        int count = 0;
        for (auto& member : members) {
            if (count >= 4) break;
            partyA.push_back(member.first);
            count++;
        }
        count = 0;
        for (auto& member : rhs.members) {
            if (count >= 4) break;
            partyB.push_back(member.first);
            count++;
        }

        // Initialize a vector to store progress for each combatant
        std::vector<float> progressA(partyA.size(), 0.0f);
        std::vector<float> progressB(partyB.size(), 0.0f);

        // Main combat loop
        while (true) {
            // Update progress based on speed
            for (size_t i = 0; i < partyA.size(); ++i) {
                progressA[i] += partyA[i]->stats.spd.effectiveValue();
                if (progressA[i] >= 1000) {
                    // Find the weakest opponent in partyB
                    auto weakest_it = std::min_element(partyB.begin(), partyB.end(), [](Organism* a, Organism* b) {
                        return a->currentPower() < b->currentPower();
                    });

                    // Attack the weakest opponent (assuming attack logic in attackOpponent method)
                    partyA[i]->attackOpponent(**weakest_it, mode);

                    // Reset progress
                    progressA[i] = 0;
                }
            }

            // Similar logic for partyB
            for (size_t i = 0; i < partyB.size(); ++i) {
                progressB[i] += partyB[i]->stats.spd.effectiveValue();
                if (progressB[i] >= 1000) {
                    auto weakest_it = std::min_element(partyA.begin(), partyA.end(), [](Organism* a, Organism* b) {
                        return a->currentPower() < b->currentPower();
                    });
                    partyB[i]->attackOpponent(**weakest_it, mode);
                    progressB[i] = 0;
                }
            }

            // Remove dead combatants
            partyA.erase(std::remove_if(partyA.begin(), partyA.end(), [](Organism* org) { return org->stats.hp.effectiveValue() <= 0; }), partyA.end());
            partyB.erase(std::remove_if(partyB.begin(), partyB.end(), [](Organism* org) { return org->stats.hp.effectiveValue() <= 0; }), partyB.end());

            // Check win conditions
            if (partyA.empty()) {
                return false;
            } else if (partyB.empty()) {
                return true;
            }
        }
    }


};

int main() {
    //loadItems();
    //printAllItems(items_all);
    //std::cout << calculatePower(1400, 1000, 1600, 100, 55, 100, 300, 160);


    std::vector<std::unique_ptr<Organism>> organisms;
    std::cout << std::endl;
    std::unique_ptr<Organism> mae = std::make_unique<Protagonist>("Mae");
    std::unique_ptr<Organism> Pentalon = std::make_unique<Sicut>("Pentalon", std::vector<float>{325, 305, 700, 51, 16, 13, 138, 123});

    organisms.push_back(std::move(mae)); // 0
    organisms.push_back(createOrganism(4, "Zebra")); // 1
    organisms.push_back(createOrganism(1, "John")); // 2
    organisms.push_back(std::move(Pentalon)); // 3
    organisms.push_back(createOrganism(1,"Aruvious", {430, 560, 1150, 85, 28, 45, 150, 145})); // 4
    organisms.push_back(createOrganism(0, "Girl", {1400, 1000, 1600, 100, 55, 100, 300, 160})); // 5
    organisms.push_back(createOrganism(0, "Girlier", {1400, 1000, 1600, 100, 55, 100, 300, 160})); // 6
    organisms[5]->turn_combat(*organisms[6]);
    organisms[0]->turn_combat(*organisms[5]);
    organisms[5]->fullHeal();


    Organism* girlptr = organisms[5].get();
    Organism* maeptr = organisms[0].get();
    Organism* aruptr = organisms[4].get();

    // Add more organisms for testing party_duel
    organisms.push_back(createOrganism(1, "Vulcan", {1200, 900, 1300, 90, 50, 80, 250, 140})); // 7
    organisms.push_back(createOrganism(2, "Athena", {1100, 950, 1250, 88, 52, 85, 245, 145})); // 8
    organisms.push_back(createOrganism(3, "Thor", {1300, 850, 1350, 92, 48, 75, 255, 135}));  // 9
    organisms.push_back(createOrganism(4, "Apollo", {1150, 920, 1280, 89, 51, 82, 248, 142})); // 10

// Pointers for easy reference
    Organism* vulcanptr = organisms[7].get();
    Organism* athenaptr = organisms[8].get();
    Organism* thorptr = organisms[9].get();
    Organism* apolloptr = organisms[10].get();

// Setup Salam party
    Party Salam("Salam");
    Salam.addMember(girlptr, LEADER);
    Salam.addMember(maeptr, OFFICER);
    Salam.addMember(vulcanptr, MEMBER);
    Salam.addMember(athenaptr, INITIATE);

// Setup Epoch party
    Party Epoch("Epoch");
    Epoch.addMember(aruptr, LEADER);
    Epoch.addMember(thorptr, OFFICER);
    Epoch.addMember(apolloptr, MEMBER);
    Epoch.addMember(organisms[6].get(), INITIATE); // Girlier

// List members before the duel
    std::cout << "\nMembers of Salam before the duel:\n";
    Salam.listMembers();
    std::cout << "\nMembers of Epoch before the duel:\n";
    Epoch.listMembers();

// Execute the party duel
    std::cout << "\nStarting the duel between Salam and Epoch...\n";
    bool winner = Salam.party_duel(Epoch, 1);

// Announce the winner
    if (winner) {
        std::cout << "\nThe winner is Salam!\n";
    } else {
        std::cout << "\nThe winner is Epoch!\n";
    }

// List members after the duel
    std::cout << "\nMembers of Salam after the duel:\n";
    Salam.listMembers();
    std::cout << "\nMembers of Epoch after the duel:\n";
    Epoch.listMembers();


};
