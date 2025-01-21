#include "worldcup23a2.h"
#include "player.h"
#include "Team.h"
#include "AVL.h"
#include "RankTree.h"
#include "union_find.h"
#include <iostream>

world_cup_t::world_cup_t()
{
	// TODO: Your code goes here
}

world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
}

StatusType world_cup_t::add_team(int teamId)
{
	if (teamId <= 0)
		return StatusType::INVALID_INPUT;
	if (all_teams.find(Team(teamId)).status() == StatusType::SUCCESS)
		return StatusType::FAILURE;
	output_t<Node<Team> *> team_out = all_teams.insert(Team(teamId));
	if (team_out.status() != StatusType::SUCCESS)
	{
		return team_out.status();
	}
	output_t<RankNode<TeamPointlessAbility> *> pointless_ability_team_out = all_pointless_ability_teams.insert(TeamPointlessAbility(teamId));
	return pointless_ability_team_out.status();
}

StatusType world_cup_t::remove_team(int teamId)
{
	if (teamId <= 0)
		return StatusType::INVALID_INPUT;
	output_t<Node<Team> *> team_out = all_teams.find(Team(teamId));
	if (team_out.status() != StatusType::SUCCESS)
		return team_out.status();
	output_t<Node<Team> *> inv_team = invalid_teams.insert(team_out.ans()->data_m);
	if (inv_team.status() != StatusType::SUCCESS)
		return inv_team.status();
	if (team_out.ans()->data_m.root_player_m != nullptr)
	{
		team_out.ans()->data_m.root_player_m->teamptr_m = &inv_team.ans()->data_m; // TODO might be seg fault
		team_out.ans()->data_m.root_player_m = nullptr;
	}
	inv_team.ans()->data_m.valid_team_m = false;
	// If team is on all_pointless_ability_teams, remove it (no need to check if remove succeeded)
	all_pointless_ability_teams.remove(TeamPointlessAbility(team_out.ans()->data_m));
	return all_teams.remove(Team(teamId));
}

StatusType world_cup_t::add_player(int playerId, int teamId,
								   const permutation_t &spirit, int gamesPlayed,
								   int ability, int cards, bool goalKeeper)
{
	// valid input
	if (teamId <= 0 || playerId <= 0 || gamesPlayed < 0 || cards < 0 || spirit.isvalid() == false)
		return StatusType::INVALID_INPUT;
	if (player_sets.find(playerId).status() == StatusType::SUCCESS)
		return StatusType::FAILURE;
	// find team
	output_t<Node<Team> *> team_out = all_teams.find(Team(teamId));
	if (team_out.status() != StatusType::SUCCESS)
		return team_out.status();
	all_pointless_ability_teams.remove(TeamPointlessAbility(team_out.ans()->data_m));

	// add player to team
	output_t<PlayerNode *> player_out = player_sets.createNewPlayerSet(Player(playerId, teamId, spirit, gamesPlayed, ability, cards, goalKeeper), &team_out.ans()->data_m);
	all_pointless_ability_teams.insert(TeamPointlessAbility(team_out.ans()->data_m));

	return player_out.status();
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{
	if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
		return output_t<int>(StatusType::INVALID_INPUT);
	output_t<Node<Team> *> team_out1 = all_teams.find(Team(teamId1));
	output_t<Node<Team> *> team_out2 = all_teams.find(Team(teamId2));
	if (team_out1.status() != StatusType::SUCCESS || team_out2.status() != StatusType::SUCCESS)
		return output_t<int>(StatusType::FAILURE);
	if (team_out1.ans()->data_m.goalKeepers_m < 1 || team_out2.ans()->data_m.goalKeepers_m < 1)
		return output_t<int>(StatusType::FAILURE);

	team_out1.ans()->data_m.gamesPlayed_m++;
	team_out2.ans()->data_m.gamesPlayed_m++;
	if (team_out1.ans()->data_m.points_m + team_out1.ans()->data_m.all_ability_m < team_out2.ans()->data_m.points_m + team_out2.ans()->data_m.all_ability_m)
	{
		// t2 won by ability
		team_out2.ans()->data_m.points_m += 3;
		return output_t<int>(3);
	}
	if (team_out1.ans()->data_m.points_m + team_out1.ans()->data_m.all_ability_m > team_out2.ans()->data_m.points_m + team_out2.ans()->data_m.all_ability_m)
	{
		// t1 won by ability
		team_out1.ans()->data_m.points_m += 3;
		return output_t<int>(1);
	}
	// there was a tie in ability
	if (team_out1.ans()->data_m.team_spirit_m.strength() < team_out2.ans()->data_m.team_spirit_m.strength())
	{
		team_out2.ans()->data_m.points_m += 3;
		return output_t<int>(4);
	}
	if (team_out1.ans()->data_m.team_spirit_m.strength() > team_out2.ans()->data_m.team_spirit_m.strength())
	{
		team_out1.ans()->data_m.points_m += 3;
		return output_t<int>(2);
	}
	// tie
	team_out1.ans()->data_m.points_m++;
	team_out2.ans()->data_m.points_m++;
	return output_t<int>(0);
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
	if (playerId <= 0)
		return StatusType::INVALID_INPUT;
	output_t<PlayerNode *> output = player_sets.find(playerId);
	if (output.status() != StatusType::SUCCESS)
		return output.status();
	output_t<PlayerNode *> output_player = player_sets.getSpecificPlayer(playerId);

	if (output.ans() == output_player.ans()) // if its the root
		return output_t<int>(output_player.ans()->player_m.gamesPlayed_m + output_player.ans()->diff_games_m + output_player.ans()->teamptr_m->gamesPlayed_m);
	// std::cout<< output_player.ans()->diff_games_m<<" <<\n";
	// std::cout<< output_player.ans()->player_m.gamesPlayed_m<<" <<\n";
	// std::cout<< output_player.ans()->next_m->diff_games_m<<" <<\n";
	// std::cout<< output.ans()->teamptr_m->gamesPlayed_m<<" <<\n";

	return output_t<int>(output_player.ans()->player_m.gamesPlayed_m + output_player.ans()->diff_games_m + output_player.ans()->next_m->diff_games_m + output.ans()->teamptr_m->gamesPlayed_m);
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
	if (playerId <= 0 || cards < 0)
		return StatusType::INVALID_INPUT;
	output_t<PlayerNode *> head = player_sets.find(playerId);
	if (head.status() != StatusType::SUCCESS)
		return head.status();
	if (head.ans()->teamptr_m->valid_team_m == false)
		return StatusType::FAILURE;
	output_t<PlayerNode *> output = player_sets.getSpecificPlayer(playerId);
	output.ans()->player_m.cards_m += cards;
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
	if (playerId <= 0)
		return StatusType::INVALID_INPUT;
	output_t<PlayerNode *> output = player_sets.getSpecificPlayer(playerId);
	if (output.status() != StatusType::SUCCESS)
		return output.status();
	return output_t<int>(output.ans()->player_m.cards_m);
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	if (teamId <= 0)
		return StatusType::INVALID_INPUT;
	output_t<Node<Team> *> team_output = all_teams.find(Team(teamId));
	if (team_output.status() != StatusType::SUCCESS)
		return team_output.status();
	return output_t<int>(team_output.ans()->data_m.points_m);
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
	output_t<RankNode<TeamPointlessAbility> *> pointless_ability_out = all_pointless_ability_teams.select(i + 1); // RankTree indexing starts from 1,2,3,...
	if (pointless_ability_out.status() != StatusType::SUCCESS)
	{
		return output_t<int>(pointless_ability_out.status());
	}
	return output_t<int>(pointless_ability_out.ans()->data_m.getTeamId());
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
	if (playerId <= 0)
		return output_t<permutation_t>(StatusType::INVALID_INPUT);
	output_t<PlayerNode *> output = player_sets.find(playerId);
	if (output.status() != StatusType::SUCCESS)
		return output_t<permutation_t>(output.status());
	if (output.ans()->teamptr_m->valid_team_m == false)
		return output_t<permutation_t>(StatusType::FAILURE);
	output_t<PlayerNode *> output_player = player_sets.getSpecificPlayer(playerId);
	// std::cout<<"hi\n";
	// std::cout << "id: " << output_player.ans()->player_m.playerId_m << " before: " << output_player.ans()->spirit_befor_m << "\n";
	if (output_player.ans()->next_m == nullptr) // if its the root then he is first
		return output_t<permutation_t>(output_player.ans()->spirit_index_m * output_player.ans()->spirit_befor_m);
	return output_t<permutation_t>(output_player.ans()->next_m->spirit_index_m * output_player.ans()->spirit_index_m * output_player.ans()->spirit_befor_m);
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2) // team1 buys team2
{
	if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
		return StatusType::INVALID_INPUT;
	output_t<Node<Team> *> output1 = all_teams.find(Team(teamId1));
	output_t<Node<Team> *> output2 = all_teams.find(Team(teamId2));
	if (output1.status() != StatusType::SUCCESS || output2.status() != StatusType::SUCCESS)
		return StatusType::FAILURE;

	// Remove teams from "pointless ability" tree before unite
	all_pointless_ability_teams.remove(TeamPointlessAbility(output1.ans()->data_m));
	all_pointless_ability_teams.remove(TeamPointlessAbility(output2.ans()->data_m));

	PlayerNode *root1 = output1.ans()->data_m.root_player_m;
	PlayerNode *root2 = output2.ans()->data_m.root_player_m;
	if (root1 == nullptr && root2 == nullptr)
	{
		all_pointless_ability_teams.insert(TeamPointlessAbility(output1.ans()->data_m));
		return remove_team(teamId2);
	}
	if (root2 == nullptr)
	{
		all_pointless_ability_teams.insert(TeamPointlessAbility(output1.ans()->data_m));
		return remove_team(teamId2);
	}
	if (root1 == nullptr)
	{
		output1.ans()->data_m.copyTeamData(output2.ans()->data_m);
		output2.ans()->data_m.root_player_m = nullptr;
		remove_team(teamId2);
		output1.ans()->data_m.root_player_m->teamptr_m = &output1.ans()->data_m;
		all_pointless_ability_teams.insert(TeamPointlessAbility(output1.ans()->data_m));
		return StatusType::SUCCESS;
	}
	player_sets.unite(root1, root2); // root1,root2 are for sure not null and also their team ptr
	remove_team(teamId2);
	// Insert united buying team int "pointless ability" tree
	all_pointless_ability_teams.insert(TeamPointlessAbility(output1.ans()->data_m));

	return StatusType::SUCCESS;
}