#include "Utils.h"
#include "./Character.h"


namespace mtm{
    Character::Character(Team team, units_t health,
                         units_t ammo, units_t range,
                         units_t power, char type_label,
                         units_t max_move_distance, units_t ammo_on_attack,
                         units_t ammo_on_reload):
                         team(team),
                         health(health),
                         ammo(ammo),
                         range(range),
                         power(power),
                         type_label(type_label),
                         max_move_distance(max_move_distance),
                         ammo_on_attack(ammo_on_attack),
                         ammo_on_reload(ammo_on_reload){}

    Character::Character(const Character& other):
        team(other.team),
        health(other.health),
        ammo(other.ammo),
        range(other.range),
        power(other.power),
        type_label(other.type_label),
        max_move_distance(other.max_move_distance),
        ammo_on_attack(other.ammo_on_attack),
        ammo_on_reload(other.ammo_on_reload){}

    Team Character::getTeam() const{
        return this->team;
    }

    char Character::getLabel() const{
        return this->team == POWERLIFTERS ?
            Utils::to_upper(this->type_label) :
            Utils::to_lower(this->type_label);
    }

    bool Character::isValidMove(int distance) const{
        return (distance <= this->max_move_distance);
    }

    bool Character::haveEnoughAmmo(bool is_friend) const{
        return (this->ammo >= this->ammo_on_attack);
    }

    void Character::absorb_damage(units_t damage){
        this->health = damage >= this->health ? 0 : (this->health - damage);
    }

    void Character::reload(){
        this->ammo += this->ammo_on_reload;
    }

    bool Character::isDead() const{
        return (this->health == 0);
    }

}



