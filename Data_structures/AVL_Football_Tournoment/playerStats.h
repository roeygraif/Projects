
#ifndef WET1_PLAYER_STATS_H
#define WET1_PLAYER_STATS_H

#include "player.h"

class PlayerStats{
    public:
    Node<Player>* nodePlayer_m;
    public:
    Node<Player>* getNodePlayer() const {return nodePlayer_m;}
    StatusType setNodePlayer(Node<Player>* nodePlayer){
        this->nodePlayer_m = nodePlayer;
        return StatusType::SUCCESS;
    }

    PlayerStats(Node<Player>* nodePlayer): nodePlayer_m(nodePlayer){}
    PlayerStats(){}; // default constructor

    bool operator < (const PlayerStats& player) const {
        if (nodePlayer_m->data_m.goals_m == player.nodePlayer_m->data_m.goals_m){
            if (nodePlayer_m->data_m.cards_m == player.nodePlayer_m->data_m.cards_m){
                return nodePlayer_m->data_m.playerId_m < player.nodePlayer_m->data_m.playerId_m;
            }
            return nodePlayer_m->data_m.cards_m > player.nodePlayer_m->data_m.cards_m;
        }
        return nodePlayer_m->data_m.goals_m < player.nodePlayer_m->data_m.goals_m;
    }

    bool operator > (const PlayerStats& player) const {
        if (nodePlayer_m->data_m.goals_m == player.nodePlayer_m->data_m.goals_m){
            if (nodePlayer_m->data_m.cards_m == player.nodePlayer_m->data_m.cards_m){
                return nodePlayer_m->data_m.playerId_m > player.nodePlayer_m->data_m.playerId_m;
            }
            return nodePlayer_m->data_m.cards_m < player.nodePlayer_m->data_m.cards_m;
        }
        return nodePlayer_m->data_m.goals_m > player.nodePlayer_m->data_m.goals_m;
    }
    bool operator == (const PlayerStats& player) const {
        return nodePlayer_m->data_m.playerId_m == player.nodePlayer_m->data_m.playerId_m;
    }

};

#endif // WET1_PLAYER_STATS_H