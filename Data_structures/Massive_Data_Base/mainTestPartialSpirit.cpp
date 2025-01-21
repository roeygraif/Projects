#include "worldcup23a2.h"
#include "AVL.h"
#include"RankTree.h"
#include <string>
#include <iostream>

using namespace std;


template<typename T>
void print(string cmd, output_t<T> res);

int main()
{
    int arr1[] = { 1, 2, 0, 4, 3 };
    int arr2[] = { 0, 3, 1, 2, 4 };
    int arr3[] = { 1, 0, 4, 3, 2 };
    int arr4[] = { 1, 0, 4, 2, 3 };
    int arr5[] = { 4, 0, 2, 1, 3 };
    int arr6[] = { 2, 0, 1, 4, 3 };
    permutation_t arr10 = arr1;
    permutation_t arr11 = arr2;
    permutation_t arr20 = arr3;
    permutation_t arr30 = arr4;
    permutation_t arr31 = arr5;
    permutation_t arr40 = arr6;

    // Create teams and world cup
    world_cup_t cup;
    cup.add_team(1);
    cup.add_team(2);
    cup.add_team(3);
    cup.add_team(4);
    
    // Add players
    if (StatusType::SUCCESS == cup.add_player(10, 1, arr1, 0, 0, 1, true))
        std::cout << "SUCCESS add_player\n";
    else
        std::cout << "FAILURE add_player\n";
    if (StatusType::SUCCESS == cup.add_player(11, 1, arr2, 0, 0, 1, true))
        std::cout << "SUCCESS add_player\n";
    else
        std::cout << "FAILURE add_player\n";


    if (StatusType::SUCCESS == cup.add_player(20, 2, arr3, 0, 0, 1, true))
        std::cout << "SUCCESS add_player\n";
    else
        std::cout << "FAILURE add_player\n";
    if (StatusType::SUCCESS == cup.add_player(30, 3, arr4, 0, 0, 1, true))
        std::cout << "SUCCESS add_player\n";
    else
        std::cout << "FAILURE add_player\n";
    if (StatusType::SUCCESS == cup.add_player(31, 3, arr5, 0, 0, 1, true))
        std::cout << "SUCCESS add_player\n";
    else
        std::cout << "FAILURE add_player\n";
    if (StatusType::SUCCESS == cup.add_player(40, 4, arr6, 0, 0, 1, true))
        std::cout << "SUCCESS add_player\n";
    else
        std::cout << "FAILURE add_player\n";

    // Run get_partial_spirit
    print("get_partial_spirit", cup.get_partial_spirit(10));
    print("get_partial_spirit", cup.get_partial_spirit(11));

    cout << "Expected get_partial_spirit(10)" << ": " << arr10 << endl;
    cout << "Expected get_partial_spirit(11)" << ": " << arr10 * arr11 << endl;

    // Team1 buys Team2, Players={player10,player11,player20}
    cup.buy_team(1, 2);

    print("get_partial_spirit", cup.get_partial_spirit(10));
    print("get_partial_spirit", cup.get_partial_spirit(11));
    print("get_partial_spirit", cup.get_partial_spirit(20));

    cout << "Expected get_partial_spirit(10)" << ": " << arr10 << endl;
    cout << "Expected get_partial_spirit(11)" << ": " << arr10 * arr11 << endl;
    cout << "Expected get_partial_spirit(20)" << ": " << (arr10 * arr11) * arr20 << endl;

    // Team3 buys Team1, Players={player30,player31,player10,player11,player20}
    cup.buy_team(3, 1);
    
    print("get_partial_spirit", cup.get_partial_spirit(30));
    print("get_partial_spirit", cup.get_partial_spirit(31));
    print("get_partial_spirit", cup.get_partial_spirit(10));
    print("get_partial_spirit", cup.get_partial_spirit(11));
    print("get_partial_spirit", cup.get_partial_spirit(20));

    cout << "Expected get_partial_spirit(30)" << ": " << arr30 << endl;
    cout << "Expected get_partial_spirit(31)" << ": " << arr30 * arr31 << endl;
    cout << "Expected get_partial_spirit(10)" << ": " << (arr30 * arr31) * arr10 << endl;
    cout << "Expected get_partial_spirit(11)" << ": " << ((arr30 * arr31) * arr10)*arr11 << endl;
    cout << "Expected get_partial_spirit(20)" << ": " << (((arr30 * arr31) * arr10) * arr11 )*arr20<< endl;

    // Team3 buys Team4, Players={player30,player31,player10,player11,player20,player40}
    cup.buy_team(3, 4);

    print("get_partial_spirit(30)", cup.get_partial_spirit(30));
    print("get_partial_spirit(31)", cup.get_partial_spirit(31));
    print("get_partial_spirit(10)", cup.get_partial_spirit(10));
    print("get_partial_spirit(11)", cup.get_partial_spirit(11));
    print("get_partial_spirit(20)", cup.get_partial_spirit(20));
    print("get_partial_spirit(40)", cup.get_partial_spirit(40));

    cout << "Expected get_partial_spirit(30)" << ": " << arr30 << endl;
    cout << "Expected get_partial_spirit(31)" << ": " << arr30 * arr31 << endl;
    cout << "Expected get_partial_spirit(10)" << ": " << (arr30 * arr31) * arr10 << endl;
    cout << "Expected get_partial_spirit(11)" << ": " << ((arr30 * arr31) * arr10) * arr11 << endl;
    cout << "Expected get_partial_spirit(20)" << ": " << (((arr30 * arr31) * arr10) * arr11) * arr20 << endl;
    cout << "Expected get_partial_spirit(40)" << ": " << ((((arr30 * arr31) * arr10) * arr11) * arr20)*arr40 << endl;
}

// Helpers
static const char* StatusTypeStr[] =
{
    "SUCCESS",
    "ALLOCATION_ERROR",
    "INVALID_INPUT",
    "FAILURE"
};



template<typename T>
void print(string cmd, output_t<T> res)
{
    if (res.status() == StatusType::SUCCESS)
    {
        cout << cmd << ": " << StatusTypeStr[(int)res.status()] << ", " << res.ans() << endl;
    }
    else
    {
        cout << cmd << ": " << StatusTypeStr[(int)res.status()] << endl;
    }
}
//     if (StatusType::SUCCESS == cup.add_player(1, 1, arr1, 0, 0, 1, true))
//         std::cout << "success add\n";
//     else
//         std::cout << "fail add\n";
//     if (StatusType::SUCCESS == cup.add_player(2, 1, arr2, 0, 0, 1, true))
//         std::cout << "success add\n";
//     else
//         std::cout << "fail add\n";
//     if (StatusType::SUCCESS == cup.add_player(3, 1, arr3, 0, 0, 1, true))
//         std::cout << "success add\n";
//     else
//         std::cout << "fail add\n";

//     if (StatusType::SUCCESS == cup.add_player(4, 2, arr4, 0, 0, 1, true))
//         std::cout << "success add\n";
//     else
//         std::cout << "fail add\n";
//     if (StatusType::SUCCESS == cup.add_player(5, 2, arr5, 0, 0, 1, true))
//         std::cout << "success add\n";
//     else
//         std::cout << "fail add\n";

//     // for (int i = 7; i < 10; i++)
//     // {
//     //     if (StatusType::SUCCESS == cup.add_player(i, 3, arr, 0, 0, 1, true))
//     //         std::cout << "success add\n";
//     //     else
//     //         std::cout << "fail add\n";
//     // }

//     // std::cout << cup.play_match(1, 2).ans() << " play match\n";
//     // std::cout << cup.play_match(1, 2).ans() << " play match\n";
//     // std::cout << cup.play_match(1, 2).ans() << " play match\n";
//     // std::cout << cup.play_match(1, 3).ans() << " play match\n";
//     // std::cout << cup.play_match(1, 3).ans() << " play match\n";

//     // std::cout << cup.num_played_games_for_player(11).ans() << " games played for id 11\n";
//     // std::cout << cup.num_played_games_for_player(21).ans() << " games played for id 21\n";
//     // std::cout << cup.num_played_games_for_player(30).ans() << " games played for id 30\n";
//     for (int i = 1; i < 7; i++)
//     {
//         if (cup.player_sets.getSpecificPlayer(i).status() == StatusType::SUCCESS)
//             std::cout << cup.player_sets.getSpecificPlayer(i).ans()->player_m.spirit_m << "\n";
//     }

//     if (StatusType::SUCCESS == cup.buy_team(1, 2))
//         std::cout << "success unite\n";

//     permutation_t trueval = permutation_t::neutral();
//     int index = 4;
//     for (int i = 1; i < index + 1; i++)
//     {
//         trueval = trueval * cup.player_sets.getSpecificPlayer(i).ans()->player_m.spirit_m;
//     }

//     output_t<permutation_t> out = cup.get_partial_spirit(index);
//     if (StatusType::SUCCESS == out.status())
//         std::cout << out.ans() << " == " << trueval;
//     else
//         std::cout << "fail\n";

//     return 0;
// }