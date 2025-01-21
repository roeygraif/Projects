#ifndef WET2_UNION_FIND_H
#define WET2_UNION_FIND_H

#include "wet2util.h"
#include "player.h"
#include "dynamic_array.h"
#include "Team.h"
#include "AVL.h"

class union_find
{
public:
    dynamic_array arr_m;
    union_find() {}
    output_t<PlayerNode *> find(int id)
    {
        output_t<PlayerNode *> node_output = this->arr_m.getPlayerNode(id);


        if (node_output.status() != StatusType::SUCCESS)
            return node_output.status();
        PlayerNode *temp = node_output.ans();
        if (temp->next_m == nullptr)
            return output_t<PlayerNode *>(temp); // return the root
        if (temp->next_m->next_m == nullptr)     // TODO maybe remove this
            return output_t<PlayerNode *>(temp->next_m);
        int sum_games_player = 0;                            // for games player
        permutation_t sum_spirit = permutation_t::neutral(); // GPS
        while (temp->next_m != nullptr)
        {

            sum_games_player += temp->diff_games_m;         // for games player
            sum_spirit = temp->spirit_index_m * sum_spirit; // GPS
            temp = temp->next_m;
        }
        PlayerNode *root = temp;
        // sum_games_player += root->teamptr_m->gamesPlayed_m; // for games player
        //  std::cout << sum_games_player << " sum\n";
        temp = node_output.ans();
        PlayerNode *temp_to_edit = temp;
        int newsum = 0;                                            // for games player
        int diff_temp = 0;                                         // for games player
        permutation_t old_temp_to_edit = permutation_t::neutral(); // GPS
        permutation_t newsum_spirit = permutation_t::neutral();    // GPS
        while (temp != root)
        {
            temp_to_edit = temp;
            temp = temp->next_m;
            temp_to_edit->next_m = root;
            diff_temp = temp_to_edit->diff_games_m;                          // for games player
            temp_to_edit->diff_games_m = sum_games_player - newsum;          // for games player
            newsum += diff_temp;                                             // for games player
            old_temp_to_edit = temp_to_edit->spirit_index_m;                 // GPS
            temp_to_edit->spirit_index_m = sum_spirit * newsum_spirit.inv(); // GPS
            // std::cout << "id: " << temp_to_edit->player_m.playerId_m << " sumspirit: " << sum_spirit << "\n";
            // std::cout << "id: " << temp_to_edit->player_m.playerId_m << " newsumspirit: " << newsum_spirit << "\n";
            // std::cout << "id: " << temp_to_edit->player_m.playerId_m << " index: " << temp_to_edit->spirit_index_m << "\n";
            newsum_spirit = old_temp_to_edit * newsum_spirit; // GPS
        }
        return output_t<PlayerNode *>(root); // return the root
    }
    StatusType unite(PlayerNode *root1, PlayerNode *root2) // pointers to the representors of the team
    {
        Team *team1ptr = root1->teamptr_m;
        Team *team2ptr = root2->teamptr_m;
        // std::cout<<"num of players team "<<team1ptr->teamId_m<<","<<team2ptr->teamId_m<<" : "<<team1ptr->num_of_players_m <<" and "<< team2ptr->num_of_players_m<<"\n";
        //  check who is the biggest team
        if (team1ptr->num_of_players_m >= team2ptr->num_of_players_m)
        {
            root2->diff_games_m += team2ptr->gamesPlayed_m - team1ptr->gamesPlayed_m - root1->diff_games_m;        // for games played                                                              // team buying is bigger
            root2->spirit_index_m = root1->spirit_index_m.inv() * team1ptr->team_spirit_m * root2->spirit_index_m; // GPS

            // std::cout << "in func mul " << team1ptr->team_spirit_m << " * " << team2ptr->team_spirit_m << " = " << team1ptr->team_spirit_m * team2ptr->team_spirit_m<<" teamspirit\n";
            team1ptr->team_spirit_m = team1ptr->team_spirit_m * team2ptr->team_spirit_m;
            root2->next_m = root1;
            root1->teamptr_m->num_of_players_m += root2->teamptr_m->num_of_players_m;
            root1->teamptr_m->goalKeepers_m += root2->teamptr_m->goalKeepers_m;
            root1->teamptr_m->all_ability_m += root2->teamptr_m->all_ability_m;
            team1ptr->points_m += team2ptr->points_m; // TODO
            root2->teamptr_m = nullptr;
        }
        else
        {
            // std::cout << "unite team " << team2ptr->gamesPlayed_m << " and " << team1ptr->gamesPlayed_m << "\n";
            root2->diff_games_m += team2ptr->gamesPlayed_m - team1ptr->gamesPlayed_m;                                    // for games played
            root1->diff_games_m -= team2ptr->gamesPlayed_m - team1ptr->gamesPlayed_m;              // for games played
            root1->spirit_index_m = root1->spirit_index_m * root2->spirit_index_m.inv() * team1ptr->team_spirit_m.inv(); // GPS
            root2->spirit_index_m = team1ptr->team_spirit_m * root2->spirit_index_m;                                     // GPS
            // root1->spirit_index_m = (root2->spirit_index_m).inv();                   // GPS

            team1ptr->team_spirit_m = team1ptr->team_spirit_m * team2ptr->team_spirit_m;
            root1->next_m = root2;
            team1ptr->num_of_players_m += team2ptr->num_of_players_m;
            team1ptr->goalKeepers_m += team2ptr->goalKeepers_m;
            team1ptr->all_ability_m += team2ptr->all_ability_m;
            team1ptr->points_m += team2ptr->points_m; // TODO
            root2->teamptr_m = team1ptr;
            team1ptr->root_player_m = root2;
            team2ptr->root_player_m = nullptr;
            root1->teamptr_m = nullptr;
        }

        return StatusType::SUCCESS;
    }
    output_t<PlayerNode *> createNewPlayerSet(Player player_to_add, Team *nodeteam)
    {
        // std::cout << nodeteam->data_m.root_player_m  << "\n";
        if (nodeteam == nullptr)
            return output_t<PlayerNode *>(StatusType::FAILURE);

        output_t<PlayerNode *> node_output = arr_m.newPlayerNode(player_to_add);
        if (node_output.status() != StatusType::SUCCESS)
            return output_t<PlayerNode *>(node_output.status());

        if (nodeteam->root_player_m == nullptr)
        {
            nodeteam->root_player_m = node_output.ans();
            node_output.ans()->teamptr_m = nodeteam;
            node_output.ans()->player_m.gamesPlayed_m = node_output.ans()->player_m.gamesPlayed_m - nodeteam->gamesPlayed_m;
        }
        else
        {
            node_output.ans()->next_m = nodeteam->root_player_m;
            node_output.ans()->spirit_index_m = node_output.ans()->next_m->spirit_index_m.inv(); // TODO we need this
            node_output.ans()->player_m.gamesPlayed_m = node_output.ans()->player_m.gamesPlayed_m - nodeteam->gamesPlayed_m - node_output.ans()->next_m->diff_games_m;
        }
        nodeteam->num_of_players_m++;

        nodeteam->all_ability_m += node_output.ans()->player_m.ability_m;
        nodeteam->team_spirit_m = nodeteam->team_spirit_m * node_output.ans()->player_m.spirit_m;
        node_output.ans()->spirit_befor_m = nodeteam->team_spirit_m;
        // std::cout<<"player id: "<<node_output.ans()->player_m.playerId_m<<"   "<<node_output.ans()->spirit_befor_m<<" inside creat node\n";
        if (node_output.ans()->player_m.goalKeeper_m == true)
            nodeteam->goalKeepers_m++;
        return node_output;
    }
    output_t<PlayerNode *> getSpecificPlayer(int id)
    {
        return output_t<PlayerNode *>(this->arr_m.getPlayerNode(id));
    }
};

#endif // WET2_UNION_FIND_H