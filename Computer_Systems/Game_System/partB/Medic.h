#ifndef MEDIC_H
#define MEDIC_H

#include <memory>
#include <vector>
#include <set>
#include "Utils.h"
#include "Damage.h"
#include "Character.h"


#define MEDIC_LABEL 'm'
#define MEDIC_MAX_MOVE_DISTANCE 5
#define MEDIC_AMMO_ON_RELOAD 5
#define MEDIC_AMMO_ON_ATTACK 1

namespace mtm {
    class Medic: public Character{
        public:
            Medic(Team team, units_t health, units_t ammo, units_t range, units_t power);

            Medic(const Medic& other);

            virtual std::shared_ptr<Character> clone() const;

            bool isAttackInRange(const GridPoint & src_coordinates, const GridPoint & dst_coordinates) const override;

            bool haveEnoughAmmo(bool is_friend) const override;

            bool isValidTarget(std::shared_ptr<Character> target, bool is_self) const override;

            std::map<GridPoint, Damage, mtm::GridPointsCompare> attack(
                const GridPoint& target_coordinates,
                std::shared_ptr<Character> target_character
            ) override;
    };
}

#endif