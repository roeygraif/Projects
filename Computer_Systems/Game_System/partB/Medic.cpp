
#include "Medic.h"

using std::shared_ptr;
using std::pair;
using std::map;
using std::set;

using mtm::GridPointsCompare;


namespace mtm{

    Medic::Medic(Team team, units_t health, units_t ammo, units_t range, units_t power):
        Character::Character(
            team, health, ammo, range, power,
            MEDIC_LABEL, MEDIC_MAX_MOVE_DISTANCE, MEDIC_AMMO_ON_ATTACK, MEDIC_AMMO_ON_RELOAD
        ){}

    Medic::Medic(const Medic& other):
        Character::Character(
                other.team, other.health, other.ammo, other.range, other.power,
                MEDIC_LABEL, MEDIC_MAX_MOVE_DISTANCE, MEDIC_AMMO_ON_ATTACK, MEDIC_AMMO_ON_RELOAD
        ){}

    std::shared_ptr<Character> Medic::clone() const{
        return shared_ptr<Character>(new Medic(*this));
    }

    bool Medic::isAttackInRange(const GridPoint & src_coordinates, const GridPoint & dst_coordinates) const{
        int attack_distance = GridPoint::distance(src_coordinates, dst_coordinates);
        return (attack_distance <= this->range);
    }

    bool Medic::haveEnoughAmmo(bool is_friend) const{
        return is_friend || this->Character::haveEnoughAmmo(is_friend);
    }

    bool Medic::isValidTarget(shared_ptr<Character> target, bool is_self) const{
        return ((!is_self) && (target != nullptr));
    }

    map<GridPoint, Damage, GridPointsCompare> Medic::attack(const GridPoint& target_coordinates,
                                                            shared_ptr<Character> target_character)
    {
        map<GridPoint, Damage, GridPointsCompare> damages;

        if(!this->isValidTarget(target_character, false)){
            return damages;
        }

        units_t friend_damage = -this->power;
        units_t foe_damage = this->power;

        damages.insert(pair<GridPoint, Damage>(target_coordinates, Damage(foe_damage, friend_damage)));

        if(this->team != (*target_character).getTeam()){
            this->ammo -= this->ammo_on_attack;
        }
        
        return damages;
    }
}

