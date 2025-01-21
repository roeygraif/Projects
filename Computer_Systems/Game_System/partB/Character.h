#ifndef CHARACTER_H
#define CHARACTER_H

#include <memory>
#include <map>
#include "Auxiliaries.h"
#include "Utils.h"
#include "Damage.h"


namespace mtm {
    class Character{
        protected:
            Team team;
            units_t health;
            units_t ammo;
            units_t range;
            units_t power;
            
            char type_label;
            units_t max_move_distance;
            units_t ammo_on_attack;
            units_t ammo_on_reload;

//            c'tor
            Character(Team team, units_t health,
                      units_t ammo, units_t range,
                      units_t power, char type_label,
                      units_t max_move_distance, units_t ammo_on_attack,
                      units_t ammo_on_reload);

//            copy c'tor
            Character(const Character& other);

        public:
            virtual std::shared_ptr<Character> clone() const = 0;
            virtual Team getTeam() const;
            virtual char getLabel() const;

//            return true if the character allowed to move that distance, false otherwise.
            virtual bool isValidMove(int distance) const;

//            return true if the character allowed to attack the dst provided, false otherwise.
            virtual bool isAttackInRange(const GridPoint & src_coordinates,
                                         const GridPoint & dst_coordinates) const = 0;

//            params: is_friend: is the target a friend or an enemy?.
//            return: true if the character have enough ammo to attack, false otherwise.
            virtual bool haveEnoughAmmo(bool is_friend) const;

//            params: taget: a ptr to the attacked character.
//                    is_self: am I attacking myself?
//            returns: true if the character can attack based on the target, false otherwise.
            virtual bool isValidTarget(std::shared_ptr<Character> target, bool is_self) const = 0;

//            returns: a map (keys: GridPoint -> values: Damage)
//                     that represents where that attack hit and the damage it did in each gridPoint.
//                     (damage struct contains information of the damage to a friend vs the damage to an enemy
            virtual std::map<GridPoint, Damage, mtm::GridPointsCompare> attack(
                const GridPoint& target_coordinates,
                std::shared_ptr<Character> target_character
            ) = 0;

//            handles when inflicted damage.
            virtual void absorb_damage(units_t damage);

            virtual void reload();

            virtual bool isDead() const;
    };
}

#endif