// 
// 234218 Data Structures 1.
// Semester: 2023A (winter).
// Wet Exercise #1.
// 
// Recommended TAB size to view this file: 8.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef WORLDCUP23A1_H_
#define WORLDCUP23A1_H_

#include "wet1util.h"
#include "Team.h"

class world_cup_t {
private:
	
	Tree<Player> playersTree_m;
	Tree<PlayerStats> playerStatsTree_m;
	Tree<Team> teamsTree_m;
	Tree<legalTeam> legalTeamsTree_m;
	
	int numOfPlayers_m;
	Node<PlayerStats> * topScorer_m;
	// Here you may add anything you want
	//
	StatusType updateClosestPlayers(Node<PlayerStats>* playerStatsNode){
		if(playerStatsNode == nullptr){
			return StatusType::FAILURE;
		}
		Node<PlayerStats>* playerStatsNodePredeccessor = playerStatsTree_m.findPredecessor(playerStatsNode);
		Node<PlayerStats>* playerStatsNodeSuccesor = playerStatsTree_m.findSuccessor(playerStatsNode);
		if( playerStatsNodePredeccessor != nullptr){
	
			playerStatsNode->data_m.nodePlayer_m->data_m.nodePredeccessorPlayer_m = playerStatsNodePredeccessor->data_m.getNodePlayer();
			playerStatsNodePredeccessor->data_m.nodePlayer_m->data_m.nodeSuccesorPlayer_m = playerStatsNode->data_m.getNodePlayer();
		}
		
		if (playerStatsNodeSuccesor != nullptr){
			playerStatsNode->data_m.nodePlayer_m->data_m.nodeSuccesorPlayer_m = playerStatsNodeSuccesor->data_m.getNodePlayer();
			playerStatsNodeSuccesor->data_m.nodePlayer_m->data_m.nodePredeccessorPlayer_m = playerStatsNode->data_m.getNodePlayer();
		}
		
		return StatusType::SUCCESS;
		}
	
	TeamPair recursiveKnockOut(int numOfTeamsInCall, TeamPair* array, int numOfTeamsToPlay){
		if (numOfTeamsInCall == 1){
			return array[0];
		}
		if (numOfTeamsInCall == 2){
			return array[0]+array[1];
		}
		if (numOfTeamsInCall == 3){
			return (array[0]+array[1])+array[2];
		}
		if (numOfTeamsInCall % 4 ==0){
			return recursiveKnockOut(numOfTeamsInCall/2, array, numOfTeamsToPlay) + recursiveKnockOut(numOfTeamsInCall/2, array+numOfTeamsInCall/2, numOfTeamsToPlay);
		}
		if (numOfTeamsInCall % 4 == 1){
			return recursiveKnockOut(numOfTeamsInCall-1, array, numOfTeamsToPlay) + array[numOfTeamsInCall-1];
		}
		if (numOfTeamsInCall % 4 == 2){
			return recursiveKnockOut(numOfTeamsInCall-2, array, numOfTeamsToPlay) + recursiveKnockOut(2, array+numOfTeamsInCall-2, numOfTeamsToPlay);
		}
		if(numOfTeamsInCall % 4 == 3){
			return recursiveKnockOut(numOfTeamsInCall-1, array, numOfTeamsToPlay) + recursiveKnockOut(1, array+numOfTeamsInCall-1, numOfTeamsToPlay);
		}
		return TeamPair(0,0);

	}
	
public:
	// <DO-NOT-MODIFY> {
	
	world_cup_t();
	virtual ~world_cup_t();
	
	StatusType add_team(int teamId, int points);
	
	StatusType remove_team(int teamId);
	
	StatusType add_player(int playerId, int teamId, int gamesPlayed,
	                      int goals, int cards, bool goalKeeper);
	
	StatusType remove_player(int playerId);
	
	StatusType update_player_stats(int playerId, int gamesPlayed,
	                                int scoredGoals, int cardsReceived);
	
	StatusType play_match(int teamId1, int teamId2);
	
	output_t<int> get_num_played_games(int playerId);
	
	output_t<int> get_team_points(int teamId);
	
	StatusType unite_teams(int teamId1, int teamId2, int newTeamId);
	
	output_t<int> get_top_scorer(int teamId);
	
	output_t<int> get_all_players_count(int teamId);
	
	StatusType get_all_players(int teamId, int *const output);
	
	output_t<int> get_closest_player(int playerId, int teamId);
	
	output_t<int> knockout_winner(int minTeamId, int maxTeamId);
	
	// } </DO-NOT-MODIFY>
};

#endif // WORLDCUP23A1_H_
