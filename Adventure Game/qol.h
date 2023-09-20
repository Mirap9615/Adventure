//
// Created by M on 9/6/23.
//



#ifndef FRONTIER_GAME_QOL_H
#define FRONTIER_GAME_QOL_H

#include "universal.h"

// 1. Class-Independent

// Uses the m-type power formula to calculate power
float calculatePower(float hp, float mana, float stamina, float defense, float phys_atk, float mag_atk, float speed, float intelligence);


float calculateMercifulDamage(float baseDamage, float opponentHealth);


// 2. Classes
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
    Attribute(int type, float inValue);

    // Constructor for multi-input scenarios (for vanity stats):
    Attribute(float inValue, float max_value_in, float bonus_flat_in, float bonus_percentage_in);

    // returns the effective value of the Attribute
    float effectiveValue() const;

    // += operator overload
    Attribute& operator+=(float inValue);

    // = operator overload
    Attribute& operator=(float inValue);

    // changes type based on input
    void changeType(int typeValue);

    // -= operator overload
    Attribute& operator-=(float inValue);

    // returns the max value of the Attribute
    float max() const;

private:
    int type; // 0 means unclassified, 1 to 9 are stat types, 10 is damage, 11 is durability, 12 is range
    float value;
    float max_value;
    float bonus_flat;
    float boost_percentage;
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
        stat.cc.changeType(10);
        stat.cd.changeType(11);

    }
public:
    // Default constructor for stats
    Stats();

    // just calls calculatePower
    float power() const;

    Attribute hp; Attribute mana; Attribute stamina; Attribute defense; Attribute phys_atk;
    Attribute mag_atk; Attribute spd; Attribute intelligence; Attribute luck; Attribute cc; Attribute cd;

    // Accepts a vector of any nonzero size and changes the Attributes in order.
    void board_change(const std::vector<float>& in_stats);

    // Adds the given value to the specified stat.
    bool changeSingleStat(int type, float value);

    // Sets the given value to the specified stat, max is adjusted as well. The assumption is that
    // the operator(s) is unlikely to set a stat to an abolute unless it is a permanent change. Everything
    // gameplay related should be relative e.g. using the += mechanic.
    bool setSingleStat(int type, float value);

    // returns the effective value of the desired stat type.
    float getSingleStatEffective(int type) const;

    // returns the maximum value of the desired stat type.
    float getSingleStatMax(int type) const;

};
// 3. Class-Dependent

#endif //FRONTIER_GAME_QOL_H