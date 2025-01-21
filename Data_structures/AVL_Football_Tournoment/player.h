// 
// 234218 Data Structures 1.
// Semester: 2023A (winter).
// Wet Exercise #1.
// 

#include "AVL.h"
#ifndef WET1_PLAYER_H
#define WET1_PLAYER_H
class Team;



class Player{
  
    public:
    int playerId_m;
    int teamId_m;
    int gamesPlayed_m;
    int goals_m;
    int cards_m;
    bool goalKeeper_m;
    Node<Team>* nodeTeam_m=nullptr;
    Node<Player>* nodeSuccesorPlayer_m=nullptr; //by stats
    Node<Player>* nodePredeccessorPlayer_m=nullptr; //by stats
    

    public:
    ~Player(){}; // default destructor
    Player(int playerId=0,int teamId=0,int gamesPlayed=0,int goals=0,int cards=0,bool goalKeeper=0){
        playerId_m = playerId;
        teamId_m = teamId;
        gamesPlayed_m = gamesPlayed;
        goals_m = goals;
        cards_m = cards;
        goalKeeper_m = goalKeeper;
    }

    int abs(int x){
    if(x<0){
        return -x;
    }
    return x;
    }   

    int getPlayerId() const {return playerId_m;}
    bool operator < (const Player& player)  const {return playerId_m < player.playerId_m;}
    bool operator > (const Player& player)  const {return playerId_m > player.playerId_m;}
    bool operator == (const Player& player)  const {return playerId_m == player.playerId_m;}
    output_t<int> findClosest(){
        if(nodeSuccesorPlayer_m == nullptr && nodePredeccessorPlayer_m == nullptr){
            return output_t<int>(StatusType::FAILURE);
        }
        if(nodeSuccesorPlayer_m == nullptr){
            return output_t<int>(nodePredeccessorPlayer_m->data_m.getPlayerId());
        }
        if(nodePredeccessorPlayer_m == nullptr){
            return output_t<int>(nodeSuccesorPlayer_m->data_m.getPlayerId());
        }
        if (abs(nodeSuccesorPlayer_m->data_m.goals_m - goals_m) == abs(nodePredeccessorPlayer_m->data_m.goals_m-goals_m)){
            if (abs(nodeSuccesorPlayer_m->data_m.cards_m -cards_m) == abs(nodePredeccessorPlayer_m->data_m.cards_m -cards_m)){
                if(abs(nodeSuccesorPlayer_m->data_m.playerId_m - playerId_m) == abs(playerId_m - nodePredeccessorPlayer_m->data_m.playerId_m)){
                    return output_t<int>(nodeSuccesorPlayer_m->data_m.getPlayerId());
                }else if (abs(nodeSuccesorPlayer_m->data_m.playerId_m - playerId_m) < abs(playerId_m - nodePredeccessorPlayer_m->data_m.playerId_m)){
                    return output_t<int>(nodeSuccesorPlayer_m->data_m.getPlayerId());
                }else {
                    return output_t<int>(nodePredeccessorPlayer_m->data_m.getPlayerId());
                }

            } else if(abs(nodeSuccesorPlayer_m->data_m.cards_m -cards_m) < abs(nodePredeccessorPlayer_m->data_m.cards_m -cards_m)){
                return output_t<int>(nodeSuccesorPlayer_m->data_m.getPlayerId());
            } else {
                return output_t<int>(nodePredeccessorPlayer_m->data_m.getPlayerId());
            }

        }else if(abs(nodeSuccesorPlayer_m->data_m.goals_m - goals_m) < abs(nodePredeccessorPlayer_m->data_m.goals_m-goals_m)){
            return output_t<int>(nodeSuccesorPlayer_m->data_m.getPlayerId());
        }else {
            return output_t<int>(nodePredeccessorPlayer_m->data_m.getPlayerId());
        }
    }


};


class playerInTeam{
    public:
    Node<Player>* playerNode_m;

    public:
    int getPlayerId() const {return playerNode_m->data_m.playerId_m;}
    bool operator < (const playerInTeam& player)  const {return playerNode_m->data_m.playerId_m < player.playerNode_m->data_m.playerId_m;}
    bool operator > (const playerInTeam& player)  const {return playerNode_m->data_m.playerId_m > player.playerNode_m->data_m.playerId_m;}
    bool operator == (const playerInTeam& player)  const {return playerNode_m->data_m.playerId_m == player.playerNode_m->data_m.playerId_m;}
    int getTeamId() const {return playerNode_m->data_m.teamId_m;}
    int getGamesPlayed() const {return playerNode_m->data_m.gamesPlayed_m;}
    int getGoals() const {return playerNode_m->data_m.goals_m;}
    int getCards() const {return playerNode_m->data_m.cards_m;}
    bool getGoalKeeper() const {return playerNode_m->data_m.goalKeeper_m;}
    Node<Team>* getTeamNode() const {return playerNode_m->data_m.nodeTeam_m;}
    playerInTeam(Node<Player>* playerNode = nullptr): playerNode_m(playerNode){};
};

#endif // WET1_PLAYER_H