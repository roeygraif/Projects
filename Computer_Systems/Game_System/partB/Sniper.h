#ifndef SNIPER_H
#define SNIPER_H

#include <memory>
#include <vector>
#include <set>
#include "Utils.h"
#include "Damage.h"
#include "Character.h"


#define SNIPER_LABEL 'n'
#define SNIPER_MAX_MOVE_DISTANCE 4
#define SNIPER_AMMO_ON_RELOAD 2
#define SNIPER_AMMO_ON_ATTACK 1

namespace mtm {
    class Sniper: public Character{
        private:
            int attack_count;

        public:
            Sniper(Team team, units_t health, units_t ammo, units_t range, units_t power);

            Sniper(const Sniper& other);

            virtual std::shared_ptr<Character> clone() const;

            bool isAttackInRange(const GridPoint & src_coordinates, const GridPoint & dst_coordinates) const override;

            bool isValidTarget(std::shared_ptr<Character> target, bool is_self) const override;

            std::map<GridPoint, Damage, mtm::GridPointsCompare> attack(
                const GridPoint& target_coordinates,
                std::shared_ptr<Character> target_character
            ) override;
    };
}

#endif