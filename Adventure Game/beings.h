//
// Created by M on 9/6/23.
//


#ifndef FRONTIER_GAME_BEINGS_H
#define FRONTIER_GAME_BEINGS_H
#include "inventory.h"

// 0. Necessary
class Organism;
const float progress_threshold = 1000.0;
const float low_hp_threshold = 0.3;
// 1. Class-Independent
struct CombatResult {
    bool isOver;
    Organism* winner;
    Organism* loser;
};
// 2. Classes
class Organism {
public:
    explicit Organism(std::string inputName) : name(std::move(inputName)), carcerisStrength(100, 100, 0, 0), level(0),
                                               dead(false), balance(0) {
    }

    Organism(std::string inputName, const std::vector<float> &in_stats) : Organism(std::move(inputName)) {
        stats.board_change(in_stats);
    }

    // assignment operator overload:
    Organism& operator=(const Organism& other) {
        if (this == &other) {
            return *this; // Return *this to deal with self-assignment
        }

        // Copy member variables from 'other' object
        this->magic_able = other.magic_able;
        this->dead = other.dead;
        this->carcerisStrength = other.carcerisStrength;
        this->stats = other.stats;
        this->name = other.name;
        this->inventory = other.inventory;
        this->level = other.level;
        this->xp = other.xp;
        this->balance = other.balance;
        this->details = other.details;

        return *this;
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
        inventory.addItems(id, amount, ItemsAllHook());
    }

    void removeItemsFromInventory(int id, int amount) {
        if (dead) return;
        std::cout << "[" << name << "]" << " ";
        inventory.removeItems(id, amount, ItemsAllHook());
    }

    void expandInventory(int amount) {
        if (dead) return;
        inventory.expandInventory(amount);
    }

    void printInventory() {
        if (dead) return;
        std::cout << inventory << std::endl;
    };

    bool changeSingleStat(int type, float value) {
        // type: in the same order as always; value can be negative
        return stats.changeSingleStat(type, value);
    }

    bool setSingleStat(int type, float value) {
        // type: in the same order as always; value cannot be negative
        if (value >= 0) {
            return stats.setSingleStat(type, value);
        } else {
            std::cout << "Cannot set a stat to a negative value!" << std::endl;
            return false;
        }
    }

    float getSingleStat(int type) const {
        return stats.getSingleStatEffective(type);
    }

    float getSingleStatMax(int type) const {
        return stats.getSingleStatMax(type);
    }

    bool checkForMercy(Organism &opponent) {
        float opponentHealth = opponent.getSingleStat(HP);
        float opponentMaxHealth = opponent.stats.hp.max(); // Assuming you don't have a getter for the max value
        float selfHealth = getSingleStat(HP);

        float threshold = std::max(5.0f, opponentMaxHealth * 0.1f);
        // Threshold for low health: 5 or 10% of max health, whichever is higher

        // Check for the condition to let go
        if (opponentHealth <= threshold && opponentHealth < 0.5 * selfHealth) {
            std::cout << name << " let " << opponent.name << " go in their pitiful state, only having "
                      << opponentHealth << " health out of " << opponentMaxHealth << " health.\n";
            opponent.setSingleStat(HP, threshold); // Set the opponent's health to the threshold value
            return true;
        }
        return false;
    }

    float normal_attack_damage() const {
        float curr_mag_atk = getSingleStat(6); float curr_phys_atk = getSingleStat(5);
        return (curr_mag_atk > curr_phys_atk) ? curr_mag_atk : curr_phys_atk;
    }

    float calculate_crit(float normal_attack_damage) {
        // RNG setup
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dis(0.0, 100.0); // 0 to 100%

        float crit_chance_lhs = getSingleStat(CRIT_CHANCE) + ((getSingleStat(INTELLIGENCE) - 100) / 10);

        if (dis(gen) < crit_chance_lhs) {
            // Critical hit
            normal_attack_damage *= 1 + getSingleStat(CRIT_DAMAGE) / 100.0f;
        }
        return normal_attack_damage;
    }

    void postMatch(CombatResult &result, Organism &rhs, int turns, float damageDealtRhs, float damageDealtLhs, float powerLhs, float powerRhs, int mode) {
        std::stringstream ss;
        // Meta info
        if (Settings::getInstance().detailed_combat_text) {
            ss << " The match lasted " << turns << " turns.\n" << this->name << " dealt " << damageDealtLhs <<
            " damage total.\n" << rhs.name << " dealt " << damageDealtRhs << " damage total.\n";
        }
        if (result.winner == nullptr) {
            if (mode == 0) {
                ss << "Both " << this->name << " and " << rhs.name << " have died!\n";
            } else if (mode == 1) {
                std::cout << "Both " << this->name << " and " << rhs.name << " withdrew from the fight.\n";
            }
            return;
        }

        // Assign power
        float power_winner = result.winner == &rhs ? powerRhs : powerLhs;
        float power_loser = result.winner == &rhs ? powerLhs : powerRhs;

        // Turn-based summaries of the fight
        if (turns < 10) {
            ss << result.winner->name << " defeated " << result.loser->name << " swiftly.\n";
        } else if (turns < 25) {
            ss << result.winner->name << " wins and " << result.loser->name << " loses.\n";
        } else {
            ss << result.winner->name << " wins against " << result.loser->name << " in a grueling fight!\n";
        }

        // Transferring the balance
        if (result.loser->balance > 0) {
            float earnings = result.loser->balance;
            result.winner->balance += earnings;
            result.loser->balance = 0;
            if (Settings::getInstance().detailed_combat_text) {
                ss << result.winner->name << " obtained all " << earnings << " slate from " << result.loser->name << "!\n";
            }
        }

        // if it was a fight to the death
        if (mode == 0)  {
            if (Settings::getInstance().detailed_combat_text) {
                ss << result.loser->name << " has died!" << std::endl;
                ss << result.winner->name << " has " << result.winner->getSingleStat(1) << " / " << result.winner->getSingleStatMax(1) << " health remaining!\n";
            }
            ss << result.winner->gainXP(power_loser, power_winner);
        // if it was a friendly duel
        } else if (mode == 1) {
            ss << result.winner->name << " wins! \n";
            ss << result.winner->name << " has " << result.winner->getSingleStat(1) << " / " << result.winner->getSingleStatMax(1) << " health remaining!";
            ss << result.loser->name << " has " << result.loser->getSingleStat(1) << " / " << result.loser->getSingleStatMax(1) << " health remaining!";
        }
        printSlowly(ss.str());
    }

    CombatResult check_finish(Organism &lhs, Organism &rhs, int mode) {
        CombatResult result{false, nullptr, nullptr};
        if (mode == 0) {
            if (lhs.stats.hp.effectiveValue() <= 0 && rhs.stats.hp.effectiveValue() <= 0) {
                // Both loses: return will have no winner or loser, but isOver will be true.
                result.isOver = true;
            } else if (lhs.stats.hp.effectiveValue() <= 0) {
                // lhs loses
                result.isOver = true;
                result.winner = &rhs;
                result.loser = &lhs;
            } else if (rhs.stats.hp.effectiveValue() <= 0) {
                // rhs loses
                result.isOver = true;
                result.winner = &lhs;
                result.loser = &rhs;
            }
            rhs.checkIfDead();
            checkIfDead();
        } else if (mode == 1) {
            if ((lhs.stats.hp.effectiveValue() <= lhs.stats.hp.max() * 0.3 || lhs.stats.hp.effectiveValue() < 3) && (rhs.stats.hp.effectiveValue() <= rhs.stats.hp.max() * 0.3 || rhs.stats.hp.effectiveValue() < 3)) {
                // both gets pushed
                result.isOver = true;
            } else if (lhs.stats.hp.effectiveValue() <= lhs.stats.hp.max() * 0.3 || lhs.stats.hp.effectiveValue() < 3) {
                // lhs gets pushed
                result.isOver = true;
                result.winner = &rhs;
                result.loser = &lhs;
            } else if (rhs.stats.hp.effectiveValue() <= rhs.stats.hp.max() * 0.3 || rhs.stats.hp.effectiveValue() < 3) {
                // rhs gets pushed
                result.isOver = true;
                result.winner = &lhs;
                result.loser = &rhs;
            }
        }
        return result;
    }


    void turn_combat(Organism &rhs, int mode = 0) {
        // mode 0 = fight to the death, mode 1 = fight to 30% hp or lower.
        if (dead || rhs.dead) return;
        // RNG setup
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        std::uniform_real_distribution<> dis(0.0, 100.0); // 0 to 100%

        float progress_lhs = this->stats.spd.effectiveValue();
        float progress_rhs = rhs.stats.spd.effectiveValue();

        float power_lhs = this->stats.power();
        float power_rhs = rhs.stats.power();

        float damageDealt_lhs = 0;
        float damageDealt_rhs = 0;
        int turns = 0;

        std::cout << this->name << " [" << int(power_lhs) << " power] versus " << rhs.name << " [" << int(power_rhs)
                  << " power] " << std::endl;

        while (this->stats.hp.effectiveValue() > 0 && rhs.stats.hp.effectiveValue() > 0) {
            if (mode != 0) {
                checkForMercy(rhs);
                rhs.checkForMercy(*this);
            }
            progress_lhs += this->stats.spd.effectiveValue();
            progress_rhs += rhs.stats.spd.effectiveValue();

            if (Settings::getInstance().detailed_combat_text) {
                std::cout << "[" << this->name << " Progress: " << progress_lhs << ", HP: "
                          << this->stats.hp.effectiveValue() << "] ";
                std::cout << "[" << rhs.name << " Progress: " << progress_rhs << ", HP: "
                          << rhs.stats.hp.effectiveValue() << "]\n";
            }

            if (progress_lhs >= 1000) {
                turns++;
                // This organism attacks
                damageDealt_lhs += performAttack(rhs, mode);
                progress_lhs = 0;
            }

            if (progress_rhs >= 1000) {
                turns++;
                // The opponent attacks
                damageDealt_rhs += rhs.performAttack(*this, mode);
                progress_rhs = 0;
            }

            CombatResult result = check_finish(*this, rhs, mode);
            if (result.isOver) {
                postMatch(result, rhs, turns, damageDealt_rhs, damageDealt_lhs, power_lhs, power_rhs, mode);
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

    std::string gainXP(float opponentPower, float selfPower) {
        if (dead) return "";
        std::stringstream ss;
        float gainedXP = calculateXPGain(opponentPower, selfPower);
        addXP(gainedXP);
        ss << name << " gained " << gainedXP << " xp! ";
        ss << "[Level " << level << ", " << round(xp) << "/" << xpRequiredForLevelUp() << " until next level ]\n";
        return ss.str();
    }

    bool checkIfDead() {
        if (dead) return true;
        if (stats.hp.effectiveValue() <= 0) {
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
            //std::cout << name << " had " << beforeHealth << " health, but now they have " << afterHealth << "/"
            //          << stats.hp.max() << " health! (+" << healthGain << ")" << std::endl;
        }
    }

    float currentPower() {
        return stats.power();
    }

    float performAttack(Organism &opponent, int mode) {
        // performs a single attack
        float damage = calculate_crit(normal_attack_damage());

        if (mode == 1) {
            float mercifulDamage = calculateMercifulDamage(damage, opponent.stats.hp.effectiveValue());
            opponent.stats.hp -= mercifulDamage;

            // Only display "with mercy" if the merciful damage is less than the original damage
            if (Settings::getInstance().detailed_combat_text) {
                if (mercifulDamage < damage) {
                    std::stringstream ss;
                    ss << this->name << " attacked " << opponent.name << " with mercy, only dealing "
                              << mercifulDamage << " damage! (normal: " << damage << ")\n";
                    printSlowly(ss.str());
                } else {
                    std::stringstream ss;
                    ss << this->name << " attacks " << opponent.name << " for " << mercifulDamage << " damage.\n";
                    printSlowly(ss.str());
                }
            }

            return mercifulDamage;
        } else {
            opponent.stats.hp -= damage;
            if (Settings::getInstance().detailed_combat_text) {
                std::cout << this->name << " attacks " << opponent.name << " for " << damage << " damage.\n";
            }

            return damage;
        }
    }

    void setDetails(const std::string& detai) {
        details = detai;
    }

    void earnSlate(float amount) {
        if (amount < 0) {
            std::cout << "You cannot earn negative amounts of slate!\n";
        } else {
            balance += amount;
        }
    }

    void loseSlate(float amount) {
        if (amount < 0) {
            std::cout << "You cannot lose negative amounts of slate!\n";
        } else {
            balance -= amount;
        }
    }

protected:
    bool magic_able = false;
    bool dead;
    Attribute carcerisStrength;
    Stats stats;
    std::string name;
    Inventory inventory;
    Slots slots;
    int level;
    float xp;
    float balance;
    std::string details;
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

    // simple assignment operator overload (cause there is nothing)
    Magical& operator=(const Magical& other) {
        // 1. Check self assignment
        if (this == &other) {
            return *this;
        }

        // 2. Copy the base class members
        Organism::operator=(other);

        // 3. Copy the derived class members
        return *this;
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

    // copy constructor (new thing)
    Sicut(const Sicut& other) : Magical(other), charm(other.charm), reputation(other.reputation) {
        // Assuming that the copy constructor for the Magical class and Attribute class handles everything appropriately
    }

    // copy assignment constructor (existing thing)
    Sicut& operator=(const Sicut& other) {
        // 1. Check self assignment
        if (this == &other) {
            return *this;  // Handle self-assignment
        }

        // 2. Call the base class copy assignment constructor overload
        Magical::operator=(other);

        // 3. Copy non-inherited, non-pointer members
        this->charm = other.charm;
        this->reputation = other.reputation;

        return *this;
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

// 3. Class-Dependent
float calculateSlate(const Organism& org);

#endif //FRONTIER_GAME_BEINGS_H
