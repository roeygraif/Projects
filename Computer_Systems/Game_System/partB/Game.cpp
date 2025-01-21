
#include <algorithm>
#include <iterator>
#include <string>
#include <cstring>
#include "Exceptions.h"
#include "Game.h"

using std::ostream;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::map;

using mtm::Character;
using mtm::Soldier;
using mtm::Medic;
using mtm::Sniper;

using mtm::GridPointsCompare;

using mtm::IllegalArgument;
using mtm::IllegalCell;
using mtm::MoveTooFar;
using mtm::CellOccupied;
using mtm::OutOfRange;
using mtm::OutOfAmmo;
using mtm::IllegalTarget;


namespace mtm {

//    private methods:
    bool Game::isCoordinatesInBoard(const GridPoint& coordinates) const {
        return (
            coordinates.row >= 0 &&
            coordinates.col >= 0 &&
            coordinates.row < this->height &&
            coordinates.col < this->width
        );
    }

    bool Game::isCellEmpty(const GridPoint& coordinates) const {
        if(!this->isCoordinatesInBoard(coordinates)){
            throw IllegalCell();
        }

        return this->board[coordinates.row][coordinates.col] == nullptr;
    }

    shared_ptr<Character> Game::getCharacter(const GridPoint& coordinates) const {
        if(!this->isCoordinatesInBoard(coordinates)){
            throw IllegalCell();
        }

        return this->board[coordinates.row][coordinates.col];
    }

    void Game::remove_character(GridPoint& coordinates){
        this->board[coordinates.row].erase(this->board[coordinates.row].begin() + coordinates.col);

        this->board[coordinates.row].insert(this->board[coordinates.row].begin() + coordinates.col,
                                            shared_ptr<Character>(nullptr));
    }

    shared_ptr<Game> Game::clone() const{
        return shared_ptr<Game>(new Game(*this));
    }

    void Game::swap(Game& game1, Game& game2){
        std::swap(game1.height, game2.height);
        std::swap(game1.width, game2.width);
        std::swap(game1.board, game2.board);
    }

//    public methods:
    Game::Game(int height, int width):
        height(height), width(width)
    {
        if(height <= 0 || width <= 0){
            throw IllegalArgument();
        }

        this->board = vector<vector<shared_ptr<Character>>>(
            height,
            vector<shared_ptr<Character>>(width, shared_ptr<Character>(nullptr))
        );
    }

    Game::~Game(){
        for(int row = 0; row < this->height; row++){
            for(int col = 0; col < this->width; col++){
                if(this->board[row][col] != nullptr){
                    if(this->board[row][col].unique()){
                        this->board[row][col].reset();
                    }
                }
            }
            vector<shared_ptr<Character>>().swap(this->board[row]);
        }
        vector<vector<shared_ptr<Character>>>().swap(this->board);
    }

    Game::Game(const Game& other){
        this->width = other.width;
        this->height = other.height;

        this->board = vector<vector<shared_ptr<Character>>>(
                height,
                vector<shared_ptr<Character>>(width, shared_ptr<Character>(nullptr))
        );

        for(int row = 0; row < other.height; row++){
            for(int col = 0; col < other.width; col++){
                if(other.board[row][col] != nullptr){
                    this->board[row][col] = other.board[row][col]->clone();
                }
            }
        }
    }

    Game& Game::operator=(const Game& other){
        Game::swap(*this, *(other.clone()));
        return *this;
    }

    void Game::addCharacter(const GridPoint& coordinates, shared_ptr<Character> character){
        if(!this->isCoordinatesInBoard(coordinates)){
            throw IllegalCell();
        }

        if(!this->isCellEmpty(coordinates)){
            throw CellOccupied();
        }

        this->board[coordinates.row][coordinates.col] = character;
    }

    shared_ptr<Character> Game::makeCharacter(CharacterType type,
                                              Team team,
                                              units_t health,
                                              units_t ammo,
                                              units_t range,
                                              units_t power)
    {
        if(health <= 0 || ammo < 0 || range < 0 || power < 0){
            throw IllegalArgument();
        }

        if(type == CharacterType::SOLDIER){
            return make_shared<Soldier>(team, health, ammo, range, power);
        }

        else if(type == CharacterType::MEDIC){
            return make_shared<Medic>(team, health, ammo, range, power);
        }

        else{
            return make_shared<Sniper>(team, health, ammo, range, power);
        }
    }

    void Game::move(const GridPoint & src_coordinates, const GridPoint & dst_coordinates){
        if(!(this->isCoordinatesInBoard(src_coordinates) && this->isCoordinatesInBoard(dst_coordinates))){
            throw IllegalCell();
        }

        if(this->isCellEmpty(src_coordinates)){
            throw CellEmpty();
        }

        int move_distance = GridPoint::distance(src_coordinates, dst_coordinates);
        shared_ptr<Character> character = this->getCharacter(src_coordinates);
        
        if(!(character->isValidMove(move_distance))){
            throw MoveTooFar();
        }

        if(!this->isCellEmpty(dst_coordinates)){
            throw CellOccupied();
        }

         std::swap(this->board[src_coordinates.row][src_coordinates.col],
                   this->board[dst_coordinates.row][dst_coordinates.col]);
    }

    void Game::attack(const GridPoint & src_coordinates, const GridPoint & dst_coordinates){
         if(!(this->isCoordinatesInBoard(src_coordinates) && this->isCoordinatesInBoard(dst_coordinates))){
            throw IllegalCell();
        }

        if(this->isCellEmpty(src_coordinates)){
            throw CellEmpty();
        }

        shared_ptr<Character> attacking_character = this->getCharacter(src_coordinates);

        if(!attacking_character->isAttackInRange(src_coordinates, dst_coordinates)){
            throw OutOfRange();
        }

        shared_ptr<Character> target = this->getCharacter(dst_coordinates);

        bool is_friend = (target != nullptr && target->getTeam() == attacking_character->getTeam());
        if(!attacking_character->haveEnoughAmmo(is_friend)) {
            throw OutOfAmmo();
        }

        if(!(attacking_character->isValidTarget(target, (src_coordinates == dst_coordinates)))){
            throw IllegalTarget();
        }

        map<GridPoint, Damage, GridPointsCompare> damages = (*attacking_character).attack(dst_coordinates,
                                                                                          target);

        for(auto it = damages.begin(); !(it == damages.end()); ++it){
            GridPoint damage_coordinates = it->first;
            
            if(!(this->isCoordinatesInBoard(damage_coordinates))){
                continue;
            }
            
            shared_ptr<Character> damage_target = this->getCharacter(damage_coordinates);
            
            if(damage_target == nullptr){
                continue;
            }

            Damage damage = it->second;
            
            bool is_same_team = attacking_character->getTeam() == damage_target->getTeam();
            damage_target->absorb_damage(is_same_team ? damage.friend_damage : damage.foe_damage);

            if(damage_target->isDead()){
                this->remove_character(damage_coordinates);
            }
        }
    }

    void Game::reload(const GridPoint & coordinates){
         if(!(this->isCoordinatesInBoard(coordinates))){
            throw IllegalCell();
        }

        if(this->isCellEmpty(coordinates)){
            throw CellEmpty();
        }

        shared_ptr<Character> character = this->getCharacter(coordinates);
        (*character).reload();
    }

    ostream& operator<<(ostream& os, const Game& game){
        char* flat_board = new char[game.height * game.width +1];
        int pos = 0;
        for(int row = 0; row < game.height; row++){
            for(int col = 0; col < game.width; col++){
                shared_ptr<Character> character = game.getCharacter(GridPoint(row, col));
                if(character != nullptr){
                    flat_board[pos++] = character->getLabel();
                }
                else {
                    flat_board[pos++] = ' ';
                }
            }
        }
        flat_board[pos] = '\0';

        ostream& strm = printGameBoard(os, flat_board, &(flat_board[pos]), game.width);
        delete[] flat_board;
        return strm;
    }


    bool Game::isOver(Team* winningTeam) const {
        bool is_board_empty = true;
        Team first_encountered_team = Team::POWERLIFTERS;

        for(int row = 0; row < this->height; row++){
            for(int col = 0; col < this->width; col++){
                shared_ptr<Character> curr_character = this->getCharacter(GridPoint(row, col));
                if(curr_character != nullptr){
                    if(is_board_empty){
                        is_board_empty = false;
                        first_encountered_team = (*curr_character).getTeam();
                    }
                    else{
                        if(first_encountered_team != (*curr_character).getTeam()){
                            return false;
                        }
                    }
                    
                }                
            }
        }

        if(is_board_empty){
            return false;
        }

        if(winningTeam != NULL && winningTeam != nullptr){
            (*winningTeam) = first_encountered_team;
        }
        return true;
    }

}
