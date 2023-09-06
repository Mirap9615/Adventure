//
// Created by M on 9/6/23.
//
#include <iostream>
#include <utility>
#include <vector>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <thread> // for sleep
#include "qol.h"

// 1. Class-Independent

float calculatePower(float hp, float mana, float stamina, float defense, float phys_atk, float mag_atk, float speed, float intelligence) {
    float unified1 = (5 * mag_atk * (mana / 30) + phys_atk) * std::pow(1.5f, (speed / 100) - 1);
    float unified2 = unified1 * (stamina / 100) + (hp + 3 * defense) * std::pow(1.1f, stamina / 100);

    return (unified2 * std::pow(2, (intelligence - 100) / 20) - 1) / 10;
}


float calculateMercifulDamage(float baseDamage, float opponentHealth) {
    float actualDamage = baseDamage;
    while (opponentHealth - actualDamage <= 0 && actualDamage >= 1) {
        actualDamage /= 2;
    }
    return actualDamage;
}


// 2. Classes
// Attribute 2-value constructor
Attribute::Attribute(int type, float inValue) : type(type), value(inValue) {
    max_value = value;
    bonus_flat = 0;
    boost_percentage = 0;
}

// Attribute 4-value constructor
Attribute::Attribute(float inValue, float max_value_in, float bonus_flat_in, float bonus_percentage_in) {
    value = inValue;
    max_value = max_value_in;
    bonus_flat = bonus_flat_in;
    boost_percentage = bonus_percentage_in;
    type = 0;
}

float Attribute::effectiveValue() const {
    return (value + bonus_flat) * ((1 + boost_percentage) / 1);
}

Attribute& Attribute::operator+=(float inValue) {
    value += inValue;
    return *this;
}

Attribute& Attribute::operator=(float inValue) {
    value = inValue;
    max_value = inValue;
    return *this;
}

void Attribute::changeType(int typeValue) {
    type = typeValue;
}

Attribute& Attribute::operator-=(float inValue) {
    value -= inValue;
    return *this;
}

float Attribute::max() const {
    return max_value;
}

Stats::Stats() : hp(HP,100), mana(MANA,100), stamina(STAMINA,100),
          defense(DEFENSE,10), phys_atk(PHYS_ATK, 4), mag_atk(MAG_ATK, 0),
          spd(SPD, 100), intelligence(INTELLIGENCE ,100), luck(LUCK, 0),
          cc(CRIT_CHANCE, 5), cd(CRIT_DAMAGE, 50) {}

float Stats::power() const {
    return calculatePower(hp.effectiveValue(), mana.effectiveValue(),stamina.effectiveValue(),
                          defense.effectiveValue(),phys_atk.effectiveValue(),mag_atk.effectiveValue(),
                          spd.effectiveValue(), intelligence.effectiveValue());
}

void Stats::board_change(const std::vector<float>& in_stats) {
    Attribute* attributes[] = {&hp, &mana, &stamina, &defense, &phys_atk, &mag_atk, &spd, &intelligence, &luck, &cc, &cd};
    size_t numAttributes = sizeof(attributes) / sizeof(attributes[0]);

    for (size_t i = 0; i < std::min(in_stats.size(), numAttributes); ++i) {
        *(attributes[i]) = in_stats[i];
    }
}

bool Stats::changeSingleStat(int type, float value) {
    switch (type) {
        case HP:
            hp += value;
            break;
        case MANA:
            mana += value;
            break;
        case STAMINA:
            stamina += value;
            break;
        case DEFENSE:
            defense += value;
            break;
        case PHYS_ATK:
            phys_atk += value;
            break;
        case MAG_ATK:
            mag_atk += value;
            break;
        case SPD:
            spd += value;
            break;
        case INTELLIGENCE:
            intelligence += value;
            break;
        case LUCK:
            luck += value;
            break;
        case CRIT_CHANCE:
            cc += value;
            break;
        case CRIT_DAMAGE:
            cd += value;
            break;
        default:
            std::cerr << "Invalid attribute type.";
            return false;
    }
    return true;
}

bool Stats::setSingleStat(int type, float value) {
    switch (type) {
        case HP:
            hp = value;
            break;
        case MANA:
            mana = value;
            break;
        case STAMINA:
            stamina = value;
            break;
        case DEFENSE:
            defense = value;
            break;
        case PHYS_ATK:
            phys_atk = value;
            break;
        case MAG_ATK:
            mag_atk = value;
            break;
        case SPD:
            spd = value;
            break;
        case INTELLIGENCE:
            intelligence = value;
            break;
        case LUCK:
            luck = value;
            break;
        case CRIT_CHANCE:
            cc = value;
            break;
        case CRIT_DAMAGE:
            cd = value;
            break;
        default:
            std::cerr << "Invalid attribute type.";
            return false;
    }
    return true;
}

float Stats::getSingleStatEffective(int type) const {
    switch (type) {
        case HP:
            return hp.effectiveValue();
        case MANA:
            return mana.effectiveValue();
        case STAMINA:
            return stamina.effectiveValue();
        case DEFENSE:
            return defense.effectiveValue();
        case PHYS_ATK:
            return phys_atk.effectiveValue();
        case MAG_ATK:
            return mag_atk.effectiveValue();
        case SPD:
            return spd.effectiveValue();
        case INTELLIGENCE:
            return intelligence.effectiveValue();
        case LUCK:
            return luck.effectiveValue();
        case CRIT_CHANCE:
            return cc.effectiveValue();
        case CRIT_DAMAGE:
            return cd.effectiveValue();
        default:
            std::cerr << "Invalid attribute type.";
            return -1;
    }
}

float Stats::getSingleStatMax(int type) const {
    switch (type) {
        case HP:
            return hp.max();
        case MANA:
            return mana.max();
        case STAMINA:
            return stamina.max();
        case DEFENSE:
            return defense.max();
        case PHYS_ATK:
            return phys_atk.max();
        case MAG_ATK:
            return mag_atk.max();
        case SPD:
            return spd.max();
        case INTELLIGENCE:
            return intelligence.max();
        case LUCK:
            return luck.max();
        case CRIT_CHANCE:
            return cc.max();
        case CRIT_DAMAGE:
            return cd.max();
        default:
            std::cerr << "Invalid attribute type.";
            return -1;
    }
}



// 3. Class-Dependent
