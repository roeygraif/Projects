


#include "wet1util.h"
#include <iostream>
#include "Team.h"
#include "player.h" 
#include "playerStats.h"
#include <memory>
#include <cstdlib>
#include "worldcup23a1.h"

template <class T>
void printNodes(T data,int * counter){
    std::cout << data << std::endl;
}
template <class T>
void checkNodes(T data,int * counter=nullptr,int* arr=nullptr){
    PlayerStats s =data;
    std::cout << s.nodePlayer_m->data_m.goals_m << std::endl;
}



int main(){

    
    
    world_cup_t worldCup;
    
    worldCup.add_team(4, 2);
    
    worldCup.add_player(1, 4, 2, 2, 2, true);
    //worldCup.add_player(2, 4, 2, 2, 2, true);
    //worldCup.add_player(3, 4, 2, 2, 2, true);
    //worldCup.add_player(4, 4, 2, 2, 2, true);
    //worldCup.add_player(5, 5, 2, 2, 2, true);
    //worldCup.add_player(6, 5, 2, 2, 2, true);
    //worldCup.add_player(7, 5, 2, 2, 2, true);
    //worldCup.add_player(8, 5, 2, 2, 2, true);
    //worldCup.add_player(5, 4, 2, 2, 2, true);
    //worldCup.add_player(6, 4, 2, 2, 2, true);
    //worldCup.add_player(7, 5, 2, 2, 2, true);
    //worldCup.add_player(8, 5, 2, 2, 2, true);
    //worldCup.add_player(9, 5, 2, 2, 2, true);
    
    worldCup.add_team(3, 2);
    worldCup.add_team(5, 3);
    worldCup.add_team(6, 3);
    worldCup.add_team(7, 3);
    worldCup.add_team(8, 3);
    worldCup.add_team(9, 3);
    worldCup.update_player_stats(1, 2, 2, 2);
    //worldCup.remove_team(4);
    //worldCup.remove_player(2);
    
    /*
    int random = 0;
    int random2 = 0;
    int random3 = 0;
    int random4 = 0;
    int random5 = 0;
    int * count = new int(0);
    for (int i=0;i<300;i++){
        random = rand() % 100;
        random2 = rand() % 10;
        random3 = rand() % 10;
        random4 = rand() % 10;
        random5 = rand() % 10;
        worldCup.add_player(random, random2, random3, random4, random5, true);
    }
    
    
    for (int i=0;i<10;i++){
        random = rand() % 100;
        random2 = rand() % 10;
        random3 = rand() % 10;
        random4 = rand() % 10;
        random5 = rand() % 10;
        worldCup.remove_player(random);
    }*/
    int n = worldCup.get_all_players_count(-1).ans();
    
    //worldCup.playerStatsTree_m.inOrder(worldCup.playerStatsTree_m.getRoot(),checkNodes<PlayerStats>,count,array);
    int * array = new int[n];
    
    worldCup.get_all_players(-1, array);
    for(int i=0;i<n;i++){
        std::cout << array[i] << " ";
    }
    worldCup.get_closest_player(3,4);
    worldCup.remove_player(7);
    worldCup.unite_teams(4, 5,12);
    worldCup.knockout_winner(1, 10);
    
    //delete count;
    delete[] array;
    //worldCup.~world_cup_t();
    

    std::cout << "-----------------" << std::endl;
}