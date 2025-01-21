
#include "Utils.h"
#include "./Soldier.h"

using std::shared_ptr;
using std::pair;
using std::map;
using std::set;

using mtm::GridPointsCompare;


namespace mtm{

    Soldier::Soldier(Team team, units_t health, units_t ammo, units_t range, units_t power):
        Character::Character(
                team, health, ammo, range, power,
                SOLDIER_LABEL, SOLDIER_MAX_MOVE_DISTANCE, SOLDIER_AMMO_ON_ATTACK, SOLDIER_AMMO_ON_RELOAD
        ){}

    Soldier::Soldier(const Soldier& other):
        Character::Character(
                other.team, other.health, other.ammo, other.range, other.power,
                SOLDIER_LABEL, SOLDIER_MAX_MOVE_DISTANCE, SOLDIER_AMMO_ON_ATTACK, SOLDIER_AMMO_ON_RELOAD
        ){}

    std::shared_ptr<Character> Soldier::clone() const{
        return shared_ptr<Character>(new Soldier(*this));
    }

    bool Soldier::isAttackInRange(const GridPoint & src_coordinates, const GridPoint & dst_coordinates) const{
        int attack_distance = GridPoint::distance(src_coordinates, dst_coordinates);
        if(attack_distance > this->range){
            return false;
        }
        return ((src_coordinates.row == dst_coordinates.row) || (src_coordinates.col == dst_coordinates.col));
    }

    bool Soldier::isValidTarget(shared_ptr<Character> target, bool is_self) const{
        return true;
    }


    map<GridPoint, Damage, GridPointsCompare> Soldier::attack(const GridPoint& target_coordinates,
                                                              shared_ptr<Character> target_character)
    {
        map<GridPoint, Damage, GridPointsCompare> damages;

        if(!this->isValidTarget(target_character, false)){
            return damages;
        }

        int ricochet_distance = Utils::roundUpDivision(this->range, 3);
        int ricochet_power = Utils::roundUpDivision(this->power, 2);

        set<GridPoint, GridPointsCompare> ricochet_affected = Utils::getGridPointsInRadius(target_coordinates,
                                                                                            ricochet_distance);

        for (auto it = ricochet_affected.begin(); !(it == ricochet_affected.end()); ++it){
            auto& effected_grid_point = it;

            units_t foe_damage = *effected_grid_point == target_coordinates ? this->power : ricochet_power;
            damages.insert(
                pair<GridPoint, Damage>(*effected_grid_point, Damage(foe_damage, SOLDIER_DAMAGE_TO_FRIEND))
            );
	    }

        this->ammo -= this->ammo_on_attack;
        
        return damages;
    }
    
}

