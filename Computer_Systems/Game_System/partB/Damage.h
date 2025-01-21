#ifndef DAMAGE_H
#define DAMAGE_H

#include "Auxiliaries.h"

namespace mtm {
    struct Damage {
        units_t foe_damage;
        units_t friend_damage;


        explicit Damage(units_t foe_damage, units_t friend_damage = 0):
            foe_damage(foe_damage), friend_damage(friend_damage) {}

        Damage(const Damage &other) = default;

        ~Damage() = default;

        Damage &operator=(const Damage &other) = default;

        bool operator==(const Damage &other) const {
            return this->foe_damage == other.foe_damage && this->friend_damage == other.friend_damage;
        }
    };
}

#endif