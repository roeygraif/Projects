

#include "wet2util.h"
#include "AVL.h"

#ifndef WET2_PLAYER_H
#define WET2_PLAYER_H
class Team;

class Player
{

public:
    int playerId_m;
    int team_id_m;
    permutation_t spirit_m;
    int gamesPlayed_m;
    int ability_m;
    int cards_m;
    bool goalKeeper_m;

public:
    ~Player(){}; // default destructor
    Player(int playerId = 0, int teamid = 0, permutation_t spirit = permutation_t::neutral(), int gamesPlayed = 0, int ability = 0, int cards = 0, bool goalKeeper = false)
    {
        playerId_m = playerId;
        team_id_m = teamid;
        spirit_m = spirit;
        gamesPlayed_m = gamesPlayed;
        ability_m = ability;
        cards_m = cards;
        goalKeeper_m = goalKeeper;
    }
    Player(const Player& plr)
    {
        playerId_m = plr.playerId_m;
        team_id_m = plr.team_id_m;
        spirit_m = plr.spirit_m;
        gamesPlayed_m = plr.gamesPlayed_m;
        ability_m = plr.ability_m;
        cards_m = plr.cards_m;
        goalKeeper_m = plr.goalKeeper_m;
    }
    // TODO make copy constructor
    int getPlayerId() const { return playerId_m; }
    bool operator<(const Player &player) const { return playerId_m < player.playerId_m; }
    bool operator>(const Player &player) const { return playerId_m > player.playerId_m; }
    bool operator==(const Player &player) const { return playerId_m == player.playerId_m; }
};

class PlayerNode
{
public:
    Player player_m;
    PlayerNode *next_m;
    Team *teamptr_m;
    int diff_games_m;
    permutation_t spirit_index_m; // h like it tirgul 9 ex 4
    permutation_t spirit_befor_m; // r like in tirgul 9 ex 4
    //permutation_t old_team_spirit;

public:
    PlayerNode(Player plr)
    {
        player_m = plr;
        next_m = nullptr;
        teamptr_m = nullptr; // only the representor has team pointer
        diff_games_m = 0;
        spirit_index_m = permutation_t::neutral();
        spirit_befor_m = permutation_t::neutral();
        //old_team_spirit = permutation_t::neutral();
    }
    ~PlayerNode()
    {
        this->next_m = nullptr;
        this->teamptr_m = nullptr;
    }
    int getPlayerId() const { return this->player_m.playerId_m; }
};

#endif // WET2_PLAYER_H