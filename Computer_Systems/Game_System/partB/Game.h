#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "Auxiliaries.h"
#include "Damage.h"
#include "Character.h"
#include "Soldier.h"
#include "Medic.h"
#include "Sniper.h"


namespace mtm {

    class Game {
        private:
            int height;
            int width;
            std::vector<std::vector<std::shared_ptr<Character>>> board;

            static void swap(Game& game1, Game& game2);
            std::shared_ptr<Game> clone() const;
            bool isCoordinatesInBoard(const GridPoint& coordinates) const;
            bool isCellEmpty(const GridPoint& coordinates) const ;
            std::shared_ptr<Character> getCharacter(const GridPoint& coordinates) const;
            void remove_character(GridPoint& coordinates);

        public:
            Game(int height, int width);
            ~Game();
            Game(const Game& other);
            Game& operator=(const Game& other);
            void addCharacter(const GridPoint& coordinates, std::shared_ptr<Character> character);
            static std::shared_ptr<Character> makeCharacter(CharacterType type,
                                                            Team team,
                                                            units_t health,
                                                            units_t ammo,
                                                            units_t range,
                                                            units_t power);
            void move(const GridPoint & src_coordinates, const GridPoint & dst_coordinates);
            void attack(const GridPoint & src_coordinates, const GridPoint & dst_coordinates);
            void reload(const GridPoint & coordinates);
            
            friend std::ostream& operator<<(std::ostream& os, const Game& game);

            bool isOver(Team* winningTeam=NULL) const;
    };
}

#endif