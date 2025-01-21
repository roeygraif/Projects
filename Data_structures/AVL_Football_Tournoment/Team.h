// 
// 234218 Data Structures 1.
// Semester: 2023A (winter).
// Wet Exercise #1.
// 

#ifndef WET1_TEAM_H
#define WET1_TEAM_H

#include "AVL.h"
#include "player.h"
#include "wet1util.h"
#include "playerStats.h"



class Team{
    public:
    int teamId_m;
    int points_m;
    int numberOfPlayers_m;
    int goalKeepers_m;
    int goals_m;
    int cards_m;
    int gamesPlayed_m=0;
    Node<PlayerStats> * topScorer_m=nullptr;
    Tree<PlayerStats> playerInTeamStats_m;
    Tree<playerInTeam> playersInTeam_m;

    public:
    Team(){}; // default constructor
    Team(int teamId ,const Team& team1, const Team & team2){
        teamId_m = teamId;
        points_m = team1.points_m + team2.points_m;
        numberOfPlayers_m = team1.numberOfPlayers_m + team2.numberOfPlayers_m;
        goalKeepers_m = team1.goalKeepers_m + team2.goalKeepers_m;
        goals_m = team1.goals_m + team2.goals_m;
        cards_m = team1.cards_m + team2.cards_m;
        gamesPlayed_m = 0;
        if (team1.topScorer_m != nullptr && team2.topScorer_m != nullptr){
            if (team1.topScorer_m->data_m.nodePlayer_m > team2.topScorer_m->data_m.nodePlayer_m){
                topScorer_m = team1.topScorer_m;
            }
            else{
                topScorer_m = team2.topScorer_m;
            }
        }
        else if (team1.topScorer_m != nullptr){
            topScorer_m = team1.topScorer_m;
        }
        else if (team2.topScorer_m != nullptr){
            topScorer_m = team2.topScorer_m;
        }
        else{
            topScorer_m = nullptr;
        }
    }
    bool checkIfLegalTeam(){
        if(goalKeepers_m>0 && numberOfPlayers_m>=11){
            return true;
        }
        return false;
    }
    
    
    Team(const Team& team){
        teamId_m = team.teamId_m;
        points_m = team.points_m;
        numberOfPlayers_m = team.numberOfPlayers_m;
        goalKeepers_m = team.goalKeepers_m;
        goals_m = team.goals_m;
        cards_m = team.cards_m;
        topScorer_m = team.topScorer_m;
    }
    Team operator = (Team & team){
        teamId_m = team.teamId_m;
        points_m = team.points_m;
        numberOfPlayers_m = team.numberOfPlayers_m;
        goalKeepers_m = team.goalKeepers_m;
        goals_m = team.goals_m;
        cards_m = team.cards_m;
        gamesPlayed_m = team.gamesPlayed_m;
        topScorer_m = team.topScorer_m;
        playerInTeamStats_m = team.playerInTeamStats_m;
        playersInTeam_m = team.playersInTeam_m;
        //
        team.topScorer_m = nullptr;
        team.playerInTeamStats_m = Tree<PlayerStats>();
        team.playersInTeam_m = Tree<playerInTeam>();
        
        return *this;
    }
    void sumTeam(const Team &team)
    {
        points_m += team.points_m;
        numberOfPlayers_m += team.numberOfPlayers_m;
        goalKeepers_m += team.goalKeepers_m;
        goals_m += team.goals_m;
        cards_m += team.cards_m;
        if (topScorer_m != nullptr && team.topScorer_m != nullptr)
            if (topScorer_m->data_m < team.topScorer_m->data_m)
                topScorer_m = team.topScorer_m;
        if (topScorer_m == nullptr && team.topScorer_m != nullptr)
            topScorer_m = team.topScorer_m;
    }
    bool operator < (const Team& team)  const {return teamId_m < team.teamId_m;}
    bool operator > (const Team& team)  const {return teamId_m > team.teamId_m;}
    bool operator == (const Team& team)  const {return teamId_m == team.teamId_m;}
    StatusType updateTopScorer(Node<PlayerStats>* playerStatsNode){
        if (topScorer_m == nullptr ){
            topScorer_m = playerStatsNode;
            return StatusType::SUCCESS;
        }

        if (playerStatsNode != nullptr && (playerStatsNode->data_m > topScorer_m->data_m)){
            topScorer_m = playerStatsNode;
            return StatusType::SUCCESS;
        }

        if (playerStatsNode==nullptr){
            //we need to find the new top scorer
            Node<PlayerStats>* newTopScorer = playerInTeamStats_m.findMax( playerInTeamStats_m.getRoot());
            topScorer_m = newTopScorer;
        }
        return StatusType::SUCCESS;
    }

    StatusType addPlayerToTeam(Node<Player>* playerNode , Node<PlayerStats>* playerStatsNode , Node<Team>* teamNode){
    StatusType status = playerInTeamStats_m.insert(playerStatsNode->data_m).status(); // O(logn)
    if (status != StatusType::SUCCESS){
        return status;
    }

    status = playersInTeam_m.insert(playerNode).status(); // O(logn)
    if (status != StatusType::SUCCESS){
        playerInTeamStats_m.remove(playerStatsNode->data_m);
        return status;
    }
    updateTopScorer(playerStatsNode);
    if (playerNode->data_m.goalKeeper_m){
        goalKeepers_m++;
    }
    playerNode->data_m.nodeTeam_m = teamNode;
    numberOfPlayers_m++;
    goals_m += playerNode->data_m.goals_m;
    cards_m += playerNode->data_m.cards_m;
    return status;
    }

    StatusType addPlayerToTeam(Node<Player>* playerNode , PlayerStats playerStats, Node<Team>* teamNode){
        
    StatusType status = playerInTeamStats_m.insert(playerStats).status(); // O(logn)
    if (status != StatusType::SUCCESS){
        return status;
    }

    status = playersInTeam_m.insert(playerNode).status(); // O(logn)
    if (status != StatusType::SUCCESS){
        playerInTeamStats_m.remove(playerStats);
        return status;
    }
    if(topScorer_m == nullptr || playerStats.nodePlayer_m->data_m.goals_m > topScorer_m->data_m.nodePlayer_m->data_m.goals_m){
        topScorer_m = playerInTeamStats_m.getRoot();
    }
    output_t<Node<PlayerStats>*> output = playerInTeamStats_m.find(playerStats);
    if (output.status() != StatusType::SUCCESS){
        return output.status();
    }
    updateTopScorer(output.ans());
    if (playerNode->data_m.goalKeeper_m){
        goalKeepers_m++;
    }
    playerNode->data_m.nodeTeam_m = teamNode;
    numberOfPlayers_m++;
    goals_m += playerNode->data_m.goals_m;
    cards_m += playerNode->data_m.cards_m;
    return status;
    }
    StatusType removePlayerFromTeam(Node<Player> *  playerToRemove){ //O(logn)
        //Player playerToRemove(playerId);
        //Node<PlayerStats> temp_PlayerStats(playerId); //becarful. need to free memory!
        output_t<Node<playerInTeam>*> playerOutput = playersInTeam_m.find(playerToRemove); //O(logn)
        if (playerOutput.status() != StatusType::SUCCESS){
            return playerOutput.status();
        }
        StatusType status = playerInTeamStats_m.remove(playerToRemove); // O(logn)
        if (status != StatusType::SUCCESS){
            return status;
        }
        status = playersInTeam_m.remove(playerToRemove); // O(logn)
        if (status != StatusType::SUCCESS){
            return status;
        }
        
        if (playerToRemove->data_m.goalKeeper_m){
            goalKeepers_m--;
        }
        numberOfPlayers_m--;
        updateTopScorer(nullptr);
        goals_m -= playerToRemove->data_m.goals_m;
        cards_m -= playerToRemove->data_m.cards_m;
        playerToRemove->data_m.nodeTeam_m = nullptr;
        return status;
    }

    Team(int teamId, int points){
    teamId_m = teamId;
    points_m = points;
    numberOfPlayers_m = 0;
    goalKeepers_m = 0;
    goals_m = 0;
    cards_m = 0;
    }

    ~Team(){
        playerInTeamStats_m.~Tree();
        playersInTeam_m.~Tree();
        playerInTeamStats_m.setRoot(nullptr);
        playersInTeam_m.setRoot(nullptr);
    }
};



class legalTeam{
    public:
    Node<Team>* teamNode_m;
    public:
    Node<Team>* getTeamNode() const {return teamNode_m;}
    StatusType setTeamNode(Node<Team>* teamNode) {
        this->teamNode_m = teamNode;
        return StatusType::SUCCESS;
    }
    legalTeam(Node<Team>* teamNode = nullptr){
        teamNode_m = teamNode;
    }
    
    bool operator < (const legalTeam& team)  const {return teamNode_m->data_m.teamId_m < team.teamNode_m->getData().teamId_m;}
    bool operator > (const legalTeam& team)  const {return teamNode_m->data_m.teamId_m > team.teamNode_m->getData().teamId_m;}
    bool operator == (const legalTeam& team)  const {return teamNode_m->data_m.teamId_m == team.teamNode_m->getData().teamId_m;}
    
};

class TeamPair{
    public:
    int teamId_m;
    int score_m;
    public:
    TeamPair(){}; //default constructor
    TeamPair(int teamId, int score){
        teamId_m = teamId;
        score_m = score;
    }
    TeamPair(Node<Team>* teamNode){
        teamId_m = teamNode->data_m.teamId_m;
        score_m = teamNode->data_m.points_m + teamNode->data_m.goals_m - teamNode->data_m.cards_m;
    }
    bool operator < (const TeamPair& team)  const {
        if (score_m == team.score_m){
            return teamId_m < team.teamId_m;
        }
        return score_m < team.score_m;
    }
    bool operator > (const TeamPair& team)  const {
        if (score_m == team.score_m){
            return teamId_m > team.teamId_m;
        }
        return score_m > team.score_m;
    }
    bool operator == (const TeamPair& team)  const {
        if (score_m == team.score_m){
            return teamId_m == team.teamId_m;
        }
        return score_m == team.score_m;
    }
    TeamPair operator + (const TeamPair& team)  const {
        if(score_m>team.score_m){
            return TeamPair(teamId_m, score_m + team.score_m+3);
        }
        if (score_m<team.score_m){
            return TeamPair(team.teamId_m, 3+score_m + team.score_m);
        }
        // so now the scores are equal
        if (teamId_m == team.teamId_m){
            return TeamPair(teamId_m, score_m + team.score_m+3);
        }
        if (teamId_m < team.teamId_m){
            return TeamPair(team.teamId_m, 3+score_m + team.score_m);
        }
        return TeamPair(teamId_m, 3+score_m + team.score_m);
    }
    bool operator != (const TeamPair& team)  const {
        return !(*this == team);
    }

};


#endif // WET1_TEAM_H