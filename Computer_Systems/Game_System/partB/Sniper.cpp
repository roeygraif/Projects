
#include "Utils.h"
#include "Sniper.h"

using std::shared_ptr;
using std::pair;
using std::map;
using std::set;

using mtm::Utils;
using mtm::GridPointsCompare;

namespace mtm{

    Sniper::Sniper(Team team, units_t health, units_t ammo, units_t range, units_t power):
        Character::Character(
            team, health, ammo, range, power,
            SNIPER_LABEL, SNIPER_MAX_MOVE_DISTANCE, SNIPER_AMMO_ON_ATTACK, SNIPER_AMMO_ON_RELOAD
        )
        {
            this->attack_count = 0;
        }

        Sniper::Sniper(const Sniper& other):
            Character::Character(
                    other.team, other.health, other.ammo, other.range, other.power,
                    SNIPER_LABEL, SNIPER_MAX_MOVE_DISTANCE, SNIPER_AMMO_ON_ATTACK, SNIPER_AMMO_ON_RELOAD
            )
        {
            this->attack_count = other.attack_count;
        }

        std::shared_ptr<Character> Sniper::clone() const{
            return shared_ptr<Character>(new Sniper(*this));
        }

    bool Sniper::isAttackInRange(const GridPoint & src_coordinates, const GridPoint & dst_coordinates) const{
        int attack_distance = GridPoint::distance(src_coordinates, dst_coordinates);
        if(attack_distance > this->range){
            return false;
        }

        return (attack_distance >= Utils::roundUpDivision(this->range, 2));
    }

    bool Sniper::isValidTarget(shared_ptr<Character> target, bool is_self) const{
        if(target == nullptr){
            return false;
        }
        return ((*target).getTeam() != this->team);
    }

    map<GridPoint, Damage, GridPointsCompare> Sniper::attack(const GridPoint& target_coordinates,
                                                             shared_ptr<Character> target_character)
    {
        map<GridPoint, Damage, GridPointsCompare> damages;

        if(!this->isValidTarget(target_character, false)){
            return damages;
        }

        this->attack_count++;
        units_t foe_damage = (this->attack_count % 3 == 0) ? (this->power * 2) : this->power;

        damages.insert(pair<GridPoint, Damage>(target_coordinates, Damage(foe_damage)));

        this->ammo -= this->ammo_on_attack;
        
        return damages;
    }
    
}

