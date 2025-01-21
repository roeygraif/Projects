#ifndef SOLDIER_H
#define SOLDIER_H

#include <memory>
#include <vector>
#include <set>
#include "Utils.h"
#include "Damage.h"
#include "Character.h"


#define SOLDIER_LABEL 's'
#define SOLDIER_MAX_MOVE_DISTANCE 3
#define SOLDIER_AMMO_ON_RELOAD 3
#define SOLDIER_AMMO_ON_ATTACK 1
#define SOLDIER_DAMAGE_TO_FRIEND 0

namespace mtm {
    class Soldier: public Character{
        public:
            Soldier(Team team, units_t health, units_t ammo, units_t range, units_t power);

            Soldier(const Soldier& other);

            virtual std::shared_ptr<Character> clone() const;

            bool isAttackInRange(const GridPoint& src_coordinates, const GridPoint& dst_coordinates) const override;

            bool isValidTarget(std::shared_ptr<Character> target, bool is_self) const override;

            std::map<GridPoint, Damage, mtm::GridPointsCompare> attack(
                const GridPoint& target_coordinates,
                std::shared_ptr<Character> target_character
            ) override;

    };
}

#endif