#include "worldcup23a1.h"
#include "AVL.h"
#include "wet1util.h"
#include "Team.h"
#include <iostream>


world_cup_t::world_cup_t()
{
	numOfPlayers_m = 0;
	topScorer_m = nullptr;

	return;
}

world_cup_t::~world_cup_t()
{
	return;
}

StatusType world_cup_t::add_team(int teamId, int points)
{
	if (teamId <= 0 || points < 0)
	{
		return StatusType::INVALID_INPUT;
	}
	return teamsTree_m.insert(Team(teamId, points)).status();
}

StatusType world_cup_t::remove_team(int teamId)
{
	if (teamId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	output_t<Node<Team> *> output = teamsTree_m.find(Team(teamId, 0));
	if (output.status() != StatusType::SUCCESS)
	{
		return output.status();
	}
	Node<Team> *teamNode = output.ans();
	if (teamNode->data_m.numberOfPlayers_m > 0)
	{
		return StatusType::FAILURE;
	}
	return teamsTree_m.remove(Team(teamId, 0));
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
								   int goals, int cards, bool goalKeeper)
{
	if (playerId <= 0 || teamId <= 0 || gamesPlayed < 0 || goals < 0 || cards < 0)
	{
		return StatusType::INVALID_INPUT;
	}
	if (gamesPlayed == 0 && (goals > 0 || cards > 0))
	{
		return StatusType::INVALID_INPUT;
	}
	output_t<Node<Team> *> teamOutput = teamsTree_m.find(Team(teamId, 0));
	StatusType teamStatus = teamOutput.status();
	if (teamStatus != StatusType::SUCCESS)
	{
		return teamStatus;
	}
	
	output_t<Node<Player> *> playerOutput = playersTree_m.insert(Player(playerId, teamId, gamesPlayed, goals, cards, goalKeeper));
	StatusType playerStatus = playerOutput.status();
	if (playerStatus != StatusType::SUCCESS)
	{
		return playerStatus;
	}
	output_t<Node<PlayerStats> *> playerStatsStatus = playerStatsTree_m.insert(playerOutput.ans());
	if (playerStatsStatus.status() != StatusType::SUCCESS)
	{
		playersTree_m.remove(Player(playerId, teamId, gamesPlayed, goals, cards, goalKeeper));
		return playerStatsStatus.status();
	}
	StatusType addToTeamStatus = teamOutput.ans()->data_m.addPlayerToTeam(playerOutput.ans(), playerStatsStatus.ans(), teamOutput.ans());
	if (addToTeamStatus != StatusType::SUCCESS)
	{
		playersTree_m.remove(Player(playerId, teamId, gamesPlayed, goals, cards, goalKeeper));
		playerStatsTree_m.remove(playerOutput.ans());
		return addToTeamStatus;
	}
	// until here we have added the player to the team , and both trees of world cup
	numOfPlayers_m++; // increase the number of players in the world cup

	updateClosestPlayers(playerStatsStatus.ans()); // update the closest players of the new player in O(logn)

	if (teamOutput.ans()->data_m.checkIfLegalTeam()) // if the team became legal, add it. O(logk). and if its already there its not changing
	{
		legalTeamsTree_m.insert(teamOutput.ans());
	}

	if (topScorer_m == nullptr || topScorer_m->data_m < playerStatsStatus.ans()->data_m)
	{
		topScorer_m = playerStatsStatus.ans(); // update the top scorer
	}
	if (teamOutput.ans()->data_m.checkIfLegalTeam())
	{
		legalTeamsTree_m.insert(teamOutput.ans());
	}
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	if (playerId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	output_t<Node<Player> *> playerOutput = playersTree_m.find(Player(playerId)); // check if the player exists O(logn)
	if (playerOutput.status() != StatusType::SUCCESS)
	{
		return playerOutput.status();
	}

	//flags to check if we need to update the the predecessor and successor of players after the removed player
	bool updateSuccesor = false;
	bool updatePredecessor = false;
	Node<PlayerStats> * succesorPlayerStats;
	Node<PlayerStats> * predecessorPlayerStats;

	// if there was a successor, we need to update it later
	if (playerOutput.ans()->data_m.nodeSuccesorPlayer_m != nullptr)
	{
		output_t<Node<PlayerStats>*> succesorPlayerStatsOutput = playerStatsTree_m.find(PlayerStats(playerOutput.ans()->data_m.nodeSuccesorPlayer_m)); // O(logn)
		succesorPlayerStats = succesorPlayerStatsOutput.ans();
		updateSuccesor = true;
	}
	if (playerOutput.ans()->data_m.nodePredeccessorPlayer_m != nullptr)
	{
		output_t<Node<PlayerStats>*> predeccessorPlayerStatsOutput = playerStatsTree_m.find(PlayerStats(playerOutput.ans()->data_m.nodePredeccessorPlayer_m));
		predecessorPlayerStats = predeccessorPlayerStatsOutput.ans();
		updatePredecessor = true;
	}

	Node<Team> * teamNode = playerOutput.ans()->data_m.nodeTeam_m; // easy access to the team of the player

	StatusType status = playerOutput.ans()->data_m.nodeTeam_m->data_m.removePlayerFromTeam(playerOutput.ans()); //O(logn)
	if (status != StatusType::SUCCESS)
	{
		return status;
	}

	if (!(teamNode->data_m.checkIfLegalTeam())) // if the team became illegal, remove it
	{
		legalTeamsTree_m.remove(teamNode); // one might think its logk, but its actually logn because there could be up to n/11 teams in the tree
	}

	if (topScorer_m->data_m == PlayerStats(playerOutput.ans()))
	{
		topScorer_m = nullptr;
	}

	status = playerStatsTree_m.remove(playerOutput.ans()); // remove the player from the player stats tree
	if (status != StatusType::SUCCESS)
	{
		return status;
	}

	status = playersTree_m.remove(Player(playerId));
	if (status != StatusType::SUCCESS)
	{
		return status;
	}

	if (updateSuccesor)
	{
		updateClosestPlayers(succesorPlayerStats);
	}
	if (updatePredecessor)
	{
		updateClosestPlayers(predecessorPlayerStats);
	}
	
	
	
	if (topScorer_m == nullptr)
	{
		output_t<Node<PlayerStats> *> topScorerOutput = playerStatsTree_m.findMax(this->playerStatsTree_m.getRoot());
		if (topScorerOutput.status() == StatusType::SUCCESS)
		{
			topScorer_m = topScorerOutput.ans();
		}
		else
		{
			return topScorerOutput.status();
		}
	}
	
	numOfPlayers_m--;
	return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
											int scoredGoals, int cardsReceived)
{
	// first we will check if the input is valid
	if (playerId <= 0 || gamesPlayed < 0 || scoredGoals < 0 || cardsReceived < 0)
	{
		return StatusType::INVALID_INPUT;
	}
	// now we will check if the player exists
	output_t<Node<Player> *> playerOutput = playersTree_m.find(Player(playerId));
	if (playerOutput.status() != StatusType::SUCCESS)
	{
		return playerOutput.status();
	}
	bool updateSuccesor = false;
	bool updatePredecessor = false;
	Node<PlayerStats> * succesorPlayerStats;
	Node<PlayerStats> * predecessorPlayerStats;

	// if there was a successor, we need to update it later
	if (playerOutput.ans()->data_m.nodeSuccesorPlayer_m != nullptr)
	{
		output_t<Node<PlayerStats>*> succesorPlayerStatsOutput = playerStatsTree_m.find(PlayerStats(playerOutput.ans()->data_m.nodeSuccesorPlayer_m)); // O(logn)
		succesorPlayerStats = succesorPlayerStatsOutput.ans();
		updateSuccesor = true;
	}
	if (playerOutput.ans()->data_m.nodePredeccessorPlayer_m != nullptr)
	{
		output_t<Node<PlayerStats>*> predeccessorPlayerStatsOutput = playerStatsTree_m.find(PlayerStats(playerOutput.ans()->data_m.nodePredeccessorPlayer_m));
		predecessorPlayerStats = predeccessorPlayerStatsOutput.ans();
		updatePredecessor = true;
	}

	// now we will check if the player exists in the playerStatsTree
	output_t<Node<PlayerStats> *> playerStatsOutput = playerStatsTree_m.find(playerOutput.ans());
	if (playerStatsOutput.status() != StatusType::SUCCESS)
	{
		return playerStatsOutput.status();
	}
	if (playerStatsOutput.ans() == topScorer_m)
	{
		topScorer_m = nullptr;
	}
	// now we will remove the player from the playerStatsTree of world cup

	StatusType status = playerStatsTree_m.remove(playerOutput.ans());
	if (status != StatusType::SUCCESS)
	{
		return status;
	}

	Node<Team> *teamNode = playerOutput.ans()->data_m.nodeTeam_m; // easy access to the team node
	// we will look for the player in the team
	output_t<Node<playerInTeam> *> playerInTeamOutput = teamNode->data_m.playersInTeam_m.find(playerOutput.ans());
	if (playerInTeamOutput.status() != StatusType::SUCCESS)
	{
		return playerOutput.status();
	}
	// we will remove the player from stats tree inside the team
	status = teamNode->data_m.playerInTeamStats_m.remove(playerOutput.ans()); // O(logn)
	if (status != StatusType::SUCCESS)
	{
		return status;
	}
	// now we will update the team stats
	teamNode->data_m.goals_m += scoredGoals;
	teamNode->data_m.cards_m += cardsReceived;
	// now we will update the player stats
	playerOutput.ans()->data_m.gamesPlayed_m += gamesPlayed;
	playerOutput.ans()->data_m.goals_m += scoredGoals;
	playerOutput.ans()->data_m.cards_m += cardsReceived;
	// now we will insert the player back to the playerStatsTree of the team
	status = teamNode->data_m.playerInTeamStats_m.insert(playerOutput.ans()).status(); // O(logn)
	if (status != StatusType::SUCCESS)
	{
		return status;
	}
	// now we will insert the player back to the playerStatsTree of the world cup
	output_t<Node<PlayerStats>*> playerStatsOutput_new = playerStatsTree_m.insert(playerOutput.ans());
	if (playerStatsOutput_new.status() != StatusType::SUCCESS)
	{
		return playerStatsOutput_new.status();
	}
	if (updateSuccesor)
	{
		updateClosestPlayers(succesorPlayerStats);
	}
	if (updatePredecessor)
	{
		updateClosestPlayers(predecessorPlayerStats);
	}
	updateClosestPlayers(playerStatsOutput_new.ans());
	if (teamNode->data_m.checkIfLegalTeam())
	{
		legalTeamsTree_m.insert(teamNode);
	}

	teamNode->data_m.updateTopScorer(nullptr);
	topScorer_m = playerStatsTree_m.findMax(playerStatsTree_m.getRoot());
	return StatusType::SUCCESS;
}



StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
	{
		return StatusType::INVALID_INPUT;
	}
	output_t<Node<Team> *> team1Output = teamsTree_m.find(Team(teamId1, 0));
	if (team1Output.status() != StatusType::SUCCESS)
	{
		return team1Output.status();
	}
	output_t<Node<Team> *> team2Output = teamsTree_m.find(Team(teamId2, 0));
	if (team2Output.status() != StatusType::SUCCESS)
	{
		return team2Output.status();
	}
	// now we will check if the teams are valid (legal). that means that they have at least 11 players and at least 1 goal keeper
	if (team1Output.ans()->data_m.numberOfPlayers_m < 11 || team2Output.ans()->data_m.numberOfPlayers_m < 11 || team1Output.ans()->data_m.goalKeepers_m < 1 || team2Output.ans()->data_m.goalKeepers_m < 1)
	{
		return StatusType::FAILURE;
	}
	// now we will play the match
	int team1Score = team1Output.ans()->data_m.goals_m - team2Output.ans()->data_m.cards_m + team1Output.ans()->data_m.points_m;
	int team2Score = team2Output.ans()->data_m.goals_m - team1Output.ans()->data_m.cards_m + team2Output.ans()->data_m.points_m;
	if (team1Score > team2Score)
	{
		team1Output.ans()->data_m.points_m += 3;
	}
	else if (team1Score < team2Score)
	{
		team2Output.ans()->data_m.points_m += 3;
	}
	else
	{
		team1Output.ans()->data_m.points_m++;
		team2Output.ans()->data_m.points_m++;
	}
	team2Output.ans()->data_m.gamesPlayed_m++;
	team1Output.ans()->data_m.gamesPlayed_m++;
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
	if (playerId <= 0)
	{
		return output_t<int>(StatusType::INVALID_INPUT);
	}
	output_t<Node<Player> *> playerOutput = playersTree_m.find(Player(playerId));
	if (playerOutput.status() != StatusType::SUCCESS)
	{
		return output_t<int>(playerOutput.status());
	}
	int gamesPlayed = playerOutput.ans()->data_m.gamesPlayed_m;
	gamesPlayed += playerOutput.ans()->data_m.nodeTeam_m->data_m.gamesPlayed_m;
	return output_t<int>(gamesPlayed);
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	if (teamId <= 0)
	{
		return output_t<int>(StatusType::INVALID_INPUT);
	}
	output_t<Node<Team> *> teamOutput = teamsTree_m.find(Team(teamId, 0));
	if (teamOutput.status() != StatusType::SUCCESS)
	{
		return output_t<int>(teamOutput.status());
	}
	return output_t<int>(teamOutput.ans()->data_m.points_m);
}
// in the method unite_teams we will unite 2 teams to one team.
// in the proccess we will unite 2 trees of players to one tree of players.
// we need to follow a complexity of O(log(k) +n1 +n2) where k is the number of teams
// n1 is the number of players in team1 and n2 is the number of players in team2
template <class T>
void saveNodesFromTeam(T data , int * counter , void* arr){
	Node<T>** p_arr = (Node<T>**)arr;
	p_arr[*counter] = new Node<T>(data);
	p_arr[*counter]->left_m = nullptr;
	p_arr[*counter]->right_m = nullptr;
	p_arr[*counter]->parent_m = nullptr;
	
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	// TODO check the recursion
	//  valid input
	if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2 || newTeamId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	// find 2 teams to merge
	output_t<Node<Team> *> team1Output = teamsTree_m.find(Team(teamId1, 0));
	if (team1Output.status() != StatusType::SUCCESS)
	{
		return team1Output.status();
	}
	output_t<Node<Team> *> team2Output = teamsTree_m.find(Team(teamId2, 0));
	if (team2Output.status() != StatusType::SUCCESS)
	{
		return team2Output.status();
	}
	// check the new team doesnt exist

	output_t<Node<Team> *> newTeamOutput = teamsTree_m.find(Team(newTeamId, 0));
	if (newTeamId != teamId1 && newTeamId != teamId2)
	{
		if (newTeamOutput.status() == StatusType::SUCCESS)
		{
			return StatusType::FAILURE;
		}
	}

	// save the teams data we will use it later
	Node<playerInTeam>** team1PlayersArray;
	Node<playerInTeam> ** team2PlayersArray;
	Node<PlayerStats> ** team1PlayersStatsArray;
	Node<PlayerStats> ** team2PlayersStatsArray;
	try{

		team1PlayersArray = new Node<playerInTeam>*[team1Output.ans()->data_m.numberOfPlayers_m];
		team2PlayersArray = new Node<playerInTeam> *[team2Output.ans()->data_m.numberOfPlayers_m];
		team1PlayersStatsArray = new Node<PlayerStats> *[team1Output.ans()->data_m.numberOfPlayers_m];
		team2PlayersStatsArray = new Node<PlayerStats> *[team2Output.ans()->data_m.numberOfPlayers_m];
	} catch (std::bad_alloc& e) {
		return StatusType::ALLOCATION_ERROR;
	}
	

	team1Output.ans()->data_m.playersInTeam_m.inOrderArray(saveNodesFromTeam, team1PlayersArray);
	team2Output.ans()->data_m.playersInTeam_m.inOrderArray(saveNodesFromTeam, team2PlayersArray);
	team1Output.ans()->data_m.playerInTeamStats_m.inOrderArray(saveNodesFromTeam, team1PlayersStatsArray);
	team2Output.ans()->data_m.playerInTeamStats_m.inOrderArray(saveNodesFromTeam, team2PlayersStatsArray);

	//
	int numOfPlayersInTeam1 = team1Output.ans()->data_m.numberOfPlayers_m;
	int numOfPlayersInTeam2 = team2Output.ans()->data_m.numberOfPlayers_m;

	// we will add the num of games played of a team to each member of the team
	int gamesPlayed = team1Output.ans()->data_m.gamesPlayed_m;
	for (int i = 0; i < numOfPlayersInTeam1; i++)
	{
		team1PlayersArray[i]->data_m.playerNode_m->data_m.gamesPlayed_m += gamesPlayed;
	}
	gamesPlayed = team2Output.ans()->data_m.gamesPlayed_m;
	for (int i = 0; i < numOfPlayersInTeam2; i++)
	{
		team2PlayersArray[i]->data_m.playerNode_m->data_m.gamesPlayed_m += gamesPlayed;
	}

	// now we will make a sorted array from the 2 sorted arrays
	Node<playerInTeam> ** sortedPlayersArray;
	try{
		sortedPlayersArray = new Node<playerInTeam> *[numOfPlayersInTeam1 + numOfPlayersInTeam2];
	} catch (std::bad_alloc& e) {
		return StatusType::ALLOCATION_ERROR;
	}
	Team tempTeam(newTeamId, team1Output.ans()->data_m , team2Output.ans()->data_m);

	// now we will delete team1 and team2 from the teams tree
	legalTeamsTree_m.remove(team1Output.ans());
	legalTeamsTree_m.remove(team2Output.ans());
	teamsTree_m.remove(Team(teamId1, 0));
	teamsTree_m.remove(Team(teamId2, 0));
	// now we will add the new team to the teams tree
	output_t<Node<Team>*> newTeamInsertedOutput = teamsTree_m.insert(tempTeam);
	if (newTeamInsertedOutput.status() != StatusType::SUCCESS)
	{
		return newTeamInsertedOutput.status();
	}
	int i = 0, j = 0, k = 0;
	while (k <(numOfPlayersInTeam1 + numOfPlayersInTeam2)){
		if (i < numOfPlayersInTeam1 && j < numOfPlayersInTeam2){
			if (team1PlayersArray[i]->data_m.playerNode_m->data_m < team2PlayersArray[j]->data_m.playerNode_m->data_m){
				sortedPlayersArray[k] = team1PlayersArray[i];
				sortedPlayersArray[k]->data_m.playerNode_m->data_m.nodeTeam_m = newTeamInsertedOutput.ans();
				i++;
			}
			else{
				sortedPlayersArray[k] = team2PlayersArray[j];
				sortedPlayersArray[k]->data_m.playerNode_m->data_m.nodeTeam_m = newTeamInsertedOutput.ans();
				j++;
			}
		}
		else if (i < numOfPlayersInTeam1){
			sortedPlayersArray[k] = team1PlayersArray[i];
			
			i++;
		}
		else{
			sortedPlayersArray[k] = team2PlayersArray[j];
			
			j++;
		}
		k++;
	}
	// now we will take care of the stats array
	Node<PlayerStats> ** sortedPlayersStatsArray;
	try{
		sortedPlayersStatsArray = new Node<PlayerStats> *[numOfPlayersInTeam1 + numOfPlayersInTeam2];
	} catch (std::bad_alloc& e) {
		return StatusType::ALLOCATION_ERROR;
	}
	
	i = 0, j = 0, k = 0;
	while (k < (numOfPlayersInTeam1+numOfPlayersInTeam2)){
		if (i < numOfPlayersInTeam1 && j < numOfPlayersInTeam2){
			if (team1PlayersStatsArray[i]->data_m < team2PlayersStatsArray[j]->data_m){
				sortedPlayersStatsArray[k] = team1PlayersStatsArray[i];
				i++;
			}
			else{
				sortedPlayersStatsArray[k] = team2PlayersStatsArray[j];
				j++;
			}
		}
		else if (i < numOfPlayersInTeam1){
			sortedPlayersStatsArray[k] = team1PlayersStatsArray[i];
			i++;
		}
		else{
			sortedPlayersStatsArray[k] = team2PlayersStatsArray[j];
			j++;
		}
		k++;
	}
	try{
		delete[] team1PlayersArray;
		delete[] team2PlayersArray;
		delete[] team1PlayersStatsArray;
		delete[] team2PlayersStatsArray;
	} catch (std::bad_alloc& e){
		return StatusType::ALLOCATION_ERROR;
	}
	// now we have 2 sorted arrays. from each one we want to create a new tree
	// the complexity of creating a tree from a sorted array is O(n)
	Tree<playerInTeam> newPlayersInTeamTree;
	Tree<PlayerStats> newPlayersStatsTree;
	newPlayersInTeamTree.createTreeFromSortedArray(sortedPlayersArray, 0,numOfPlayersInTeam1 + numOfPlayersInTeam2 - 1);
	newPlayersStatsTree.createTreeFromSortedArray(sortedPlayersStatsArray, 0, numOfPlayersInTeam1 + numOfPlayersInTeam2 - 1);
	// now we will create a temp team to save the data of team1+team2
	
	newTeamInsertedOutput.ans()->data_m= tempTeam;
	newTeamInsertedOutput.ans()->data_m.playersInTeam_m.setRoot(newPlayersInTeamTree.getRoot());
	newTeamInsertedOutput.ans()->data_m.playerInTeamStats_m.setRoot(newPlayersStatsTree.getRoot());
	newPlayersInTeamTree.setRoot(nullptr);
	newPlayersStatsTree.setRoot(nullptr);
	//newTeamOutput.ans()->data_m.changeTeamNodeForPlayers( newTeamOutput.ans());
	
	// now we will add the new team to the legal teams tree
	if (newTeamInsertedOutput.ans()->data_m.checkIfLegalTeam()){
		legalTeamsTree_m.insert(newTeamInsertedOutput.ans());
	}
	try{
		delete[] sortedPlayersArray;
		delete[] sortedPlayersStatsArray;
	} catch (std::bad_alloc& e){
		return StatusType::ALLOCATION_ERROR;
	}

	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	if(teamId==0)
	{
		return StatusType::INVALID_INPUT;
	}
	Node<PlayerStats> * topScorer = nullptr;
	if(teamId<0)
	{
		topScorer=topScorer_m;
	}
	else
	{
		output_t<Node<Team> *> teamOutput = teamsTree_m.find(Team(teamId, 0));
		if (teamOutput.status() != StatusType::SUCCESS)
		{
			return teamOutput.status();
		}
		topScorer =teamOutput.ans()->data_m.topScorer_m;
	}
	if(topScorer==nullptr)
	{
		return StatusType::FAILURE;
	}
	return topScorer->data_m.nodePlayer_m->data_m.playerId_m;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
	if (teamId == 0)
	{
		return output_t<int>(StatusType::INVALID_INPUT);
	}
	if (teamId < 0)
	{
		return output_t<int>(this->numOfPlayers_m);
	}
	output_t<Node<Team> *> teamOutput = this->teamsTree_m.find(Team(teamId, 0));
	if (teamOutput.status() != StatusType::SUCCESS)
	{
		return teamOutput.status();
	}
	return output_t<int>(teamOutput.ans()->data_m.numberOfPlayers_m);
}

template <class T>
void saveNodes(T data=T(0),int * counter=nullptr, void* arr=nullptr){
	//PlayerStats s = data;
	int * int_arr = (int*)arr;
    int_arr[*counter] = data.nodePlayer_m->data_m.playerId_m;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	// TODO: Your code goes here
	if(teamId==0 || output==nullptr)
	{
		return StatusType::INVALID_INPUT;
	}
	output_t<int> numOfPlayersOutput = get_all_players_count(teamId);
	if (numOfPlayersOutput.status() != StatusType::SUCCESS)
	{
		return numOfPlayersOutput.status();
	}
	if (numOfPlayersOutput.ans() == 0)
	{
		return StatusType::FAILURE;
	}
	
	if (numOfPlayersOutput.status() != StatusType::SUCCESS)
	{
		return numOfPlayersOutput.status();
	}
	int * counter;
	try{
		counter =new int(0);
	} catch (std::bad_alloc& e) {
		return StatusType::ALLOCATION_ERROR;
	}
	if(teamId<0)
	{
		//playerStatsTree_m.inOrder(playerStatsTree_m.getRoot(), saveNodes<int>,counter, output);
		//tree.inOrder(tree.getRoot(),saveNodes<int>,count,array);
		playerStatsTree_m.inOrder(playerStatsTree_m.getRoot(), saveNodes<PlayerStats>,counter, output);
	}
	else
	{
		output_t<Node<Team> *> teamOutput = teamsTree_m.find(Team(teamId, 0));
		if (teamOutput.status() != StatusType::SUCCESS)
		{
			delete counter;
			return teamOutput.status();
		}
		teamOutput.ans()->data_m.playerInTeamStats_m.inOrder(teamOutput.ans()->data_m.playerInTeamStats_m.getRoot(), saveNodes<PlayerStats>,counter, output);
	}
	delete counter;
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	//the complexity of this function is O(log(n) +log(k)) where n is the number of players in the team and k is the number of teams in the world cup
	if (playerId <= 0 || teamId <= 0)
	{
		return StatusType::INVALID_INPUT;
	}
	output_t<Node<Team> *> teamOutput = teamsTree_m.find(Team(teamId, 0)); // find the team in the tree
	if (teamOutput.status() != StatusType::SUCCESS)
	{
		return teamOutput.status();
	}
	Node<Player> * tempPlayerNode;
	try{
		tempPlayerNode = new Node<Player>(Player(playerId, 0)); // remmeber to free this node
	} catch (std::bad_alloc& e) {
		return StatusType::ALLOCATION_ERROR;
	}
	output_t<Node<playerInTeam>* > playerOutput = teamOutput.ans()->data_m.playersInTeam_m.find(playerInTeam(tempPlayerNode)); // find the player in the tree
	try{
		delete tempPlayerNode;
	} catch (std::bad_alloc& e) {
		return StatusType::ALLOCATION_ERROR;
	}
	if (playerOutput.status() != StatusType::SUCCESS)
	{
		return playerOutput.status();
	}
	Node<Player> * playerNode = playerOutput.ans()->data_m.playerNode_m;
	output_t<int> closestPlayer = playerNode->data_m.findClosest(); // find the closest player in the tree
	if (closestPlayer.status() != StatusType::SUCCESS)
	{
		return closestPlayer.status();
	}
	return closestPlayer.ans();
}

template <class T>
void countNodesInRange(T data=T(0),int * counter=nullptr, void* arr=nullptr){
	//PlayerStats s = data;
	int * arr_TeamsId = (int *)arr; // arr[0]=minTeamId arr[1]=maxTeamId arr[2]=counter of legal teams

	if(data.teamNode_m->data_m.teamId_m>=arr_TeamsId[0] && data.teamNode_m->data_m.teamId_m<=arr_TeamsId[1])
	{
		arr_TeamsId[2]++;
	}
	return;
}

template <class T>
void getLegalTeamArray(T data,int * counter=nullptr, void* arr=nullptr){
	//PlayerStats s = data;
	// counter[0]= minTeamId counter[1]=maxTeamId counter[2]=location of the array
	TeamPair * arr_teams = (TeamPair *)arr; 
	if (data.teamNode_m->data_m.teamId_m >= counter[0] && data.teamNode_m->data_m.teamId_m <= counter[1]){
		arr_teams[counter[2]] = TeamPair(data.teamNode_m);
		counter[2]++;
	}
	counter[0]--;
	return;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	if (minTeamId < 0 || maxTeamId < 0 || minTeamId > maxTeamId)
	{
		return StatusType::INVALID_INPUT;
	}
	int * counterOfLegalTeamsInRange;
	try{
		counterOfLegalTeamsInRange =new int(0);
	} catch (std::bad_alloc& e) {
		return StatusType::ALLOCATION_ERROR;
	}
	int * arr_TeamsId;
	try{
		arr_TeamsId =new int[3];
	} catch (std::bad_alloc& e) {
		delete counterOfLegalTeamsInRange;
		return StatusType::ALLOCATION_ERROR;
	}
	arr_TeamsId[0]=minTeamId;
	arr_TeamsId[1]=maxTeamId;
	arr_TeamsId[2]=0;
	legalTeamsTree_m.inOrder(legalTeamsTree_m.getRoot(), countNodesInRange<legalTeam>,counterOfLegalTeamsInRange, arr_TeamsId);
	if(arr_TeamsId[2]==0)
	{
		delete counterOfLegalTeamsInRange;
		delete[] arr_TeamsId;
		return StatusType::FAILURE;
	}
	*counterOfLegalTeamsInRange = arr_TeamsId[2];
	TeamPair * legalTeamsArray;
	try{
		legalTeamsArray =new TeamPair[*counterOfLegalTeamsInRange ];
	} catch (std::bad_alloc& e) {
		delete counterOfLegalTeamsInRange;
		delete[] arr_TeamsId;
		return StatusType::ALLOCATION_ERROR;
	}
	arr_TeamsId[2]=0;
	//bool oddNumberOfTeams = false;
	legalTeamsTree_m.inOrder(legalTeamsTree_m.getRoot(), getLegalTeamArray<legalTeam>,arr_TeamsId, legalTeamsArray);
	TeamPair winning_Team= recursiveKnockOut(*counterOfLegalTeamsInRange,legalTeamsArray,*counterOfLegalTeamsInRange);
	delete counterOfLegalTeamsInRange;
	delete[] arr_TeamsId;
	delete[] legalTeamsArray;
	
	return output_t<int>(winning_Team.teamId_m);
}

