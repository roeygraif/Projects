// #include "worldcup23a2.h"
// #include "AVL.h"
// #include <iostream>

// int main()
// {
//     // int arr1[] = {3, 0, 1, 2, 4};
//     int arr2[] = {2, 3, 1, 0, 4};
//     int arr3[] = {2, 4, 3, 1, 0};
//     int arr4[] = {4, 0, 3, 1, 2};
//     int arr5[] = {3, 4, 0, 1, 2};
//     int arr6[] = {3, 2, 1, 0, 4};
//     int arr7[] = {0, 4, 2, 3, 1};
//     // int arr8[] = {0, 3, 1, 2, 4};
//     // int arr9[] = {1, 2, 3, 0, 4};
//     // int arr6[] = {0, 1, 2, 3, 4};
//     //    permutation_t arr = permutation_t().neutral();
//     world_cup_t cup;
//     // permutation_t some = permutation_t::neutral();
//     // some = cup.player_sets.getSpecificPlayer(1).ans()->player_m.spirit_m * cup.player_sets.getSpecificPlayer(2).ans()->player_m.spirit_m * cup.player_sets.getSpecificPlayer(3).ans()->player_m.spirit_m * cup.player_sets.getSpecificPlayer(4).ans()->player_m.spirit_m * cup.player_sets.getSpecificPlayer(5).ans()->player_m.spirit_m;
//     // permutation_t plr1 = cup.player_sets.getSpecificPlayer(1).ans()->player_m.spirit_m;
//     for (int i = 1; i < 4; i++)
//     {
//         cup.add_team(i);
//     }
//     // if (StatusType::SUCCESS == cup.add_player(10, 1, arr1, 0, 0, 1, true))
//     //     std::cout << "success add\n";
//     // else
//     //     std::cout << "fail add\n";
//     std::cout << "team 2 spirit " << cup.all_teams.find(2).ans()->data_m.team_spirit_m << "\n";
//     if (StatusType::SUCCESS == cup.add_player(1, 2, arr2, 0, 0, 1, true))
//         std::cout << "success add\n";
//     else
//         std::cout << "fail add\n";
//     // std::cout << "team 2 spirit " << cup.all_teams.find(2).ans()->data_m.team_spirit_m << "\n";
//     if (StatusType::SUCCESS == cup.add_player(2, 2, arr3, 0, 0, 1, true))
//         std::cout << "success add\n";
//     else
//         std::cout << "fail add\n";
//     // std::cout << "team 2 spirit " << cup.all_teams.find(2).ans()->data_m.team_spirit_m << "\n";
//     if (StatusType::SUCCESS == cup.add_player(3, 3, arr4, 0, 0, 1, true))
//         std::cout << "success add\n";
//     else
//         std::cout << "fail add\n";
//     if (StatusType::SUCCESS == cup.add_player(4, 3, arr5, 0, 0, 1, true))
//         std::cout << "success add\n";
//     else
//         std::cout << "fail add\n";
//     std::cout << "mul " << cup.all_teams.find(2).ans()->data_m.team_spirit_m << " * " << cup.all_teams.find(3).ans()->data_m.team_spirit_m << " = " << cup.all_teams.find(2).ans()->data_m.team_spirit_m * cup.all_teams.find(3).ans()->data_m.team_spirit_m << "\n";
//     if (StatusType::SUCCESS == cup.buy_team(2, 3))
//         std::cout << "success unite\n";
//     std::cout << "team 2 spirit " << cup.all_teams.find(2).ans()->data_m.team_spirit_m << "\n";
//     std::cout<<cup.player_sets.getSpecificPlayer(1).ans()->player_m.spirit_m * cup.player_sets.getSpecificPlayer(2).ans()->player_m.spirit_m <<" * "<< cup.player_sets.getSpecificPlayer(3).ans()->player_m.spirit_m * cup.player_sets.getSpecificPlayer(4).ans()->player_m.spirit_m;
//     std::cout << " = " << cup.player_sets.getSpecificPlayer(1).ans()->player_m.spirit_m * cup.player_sets.getSpecificPlayer(2).ans()->player_m.spirit_m * cup.player_sets.getSpecificPlayer(3).ans()->player_m.spirit_m * cup.player_sets.getSpecificPlayer(4).ans()->player_m.spirit_m << "\n";
//     if (StatusType::SUCCESS == cup.add_player(5, 2, arr6, 0, 0, 1, true))
//         std::cout << "success add\n";
//     else
//         std::cout << "fail add\n";
//     // std::cout << "team 2 spirit " << cup.all_teams.find(2).ans()->data_m.team_spirit_m << "\n";
//     // std::cout << "mul " << cup.all_teams.find(2).ans()->data_m.team_spirit_m * cup.player_sets.getSpecificPlayer(5).ans()->player_m.spirit_m << "\n";

//     if (StatusType::SUCCESS == cup.add_player(6, 2, arr7, 0, 0, 1, true))
//         std::cout << "success add\n";
//     else
//         std::cout << "fail add\n";
//     // std::cout << "team 2 spirit " << cup.all_teams.find(2).ans()->data_m.team_spirit_m << "\n";

//     int index = 0;
//     permutation_t trueval = permutation_t::neutral();

//     for (int j = 1; j < 7; j++)
//     {
//         trueval = permutation_t::neutral();
//         index = j;
//         for (int i = 1; i < index + 1; i++)
//         {
//             trueval = trueval * cup.player_sets.getSpecificPlayer(i).ans()->player_m.spirit_m;
//         }

//         output_t<permutation_t> out = cup.get_partial_spirit(index);
//         if (StatusType::SUCCESS == out.status())
//         {
//             std::cout << j << ") " << out.ans() << " == " << trueval << "\n";
//         }
//         else
//             std::cout << "fail\n";
//     }
//     return 0;
// }