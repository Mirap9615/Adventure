//
// Created by M on 9/6/23.
//

#ifndef FRONTIER_GAME_QOL_H
#define FRONTIER_GAME_QOL_H

#endif //FRONTIER_GAME_QOL_H

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
    int type; // 0 means unclassified, 1 to 9 are stat types, 10 is damage, 11 is durability
    float value;
    float max_value;
    float bonus_flat;
    float boost_percentage;
};