#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./map.h"
#include "./utils.h"
#include "./game.h"
#include "./tournament.h"
#define EMPTY_DATA -1

struct tournament_t {
    int id;
    const char* location;
    Map games;
    int next_game_id;
    int winner_id;
    int max_games_per_player;
    Map players;
    Map deleted_players;
};



Tournament tournamentCreate(int id, const char* location, int max_games_per_player){
    Tournament tournament = malloc(sizeof(*tournament));
    if(tournament == NULL){
        return NULL;
    }

    tournament->id = id;
    tournament->location = location;
    tournament->max_games_per_player = max_games_per_player;
    tournament->games = mapCreate(gameCopyPTR, intKeyCopy, gameDestroyPTR, intKeyDestroy, intKeysCompare); 
    if(tournament->games == NULL){
        free(tournament);
        return NULL;
    }

    tournament->next_game_id = 1;
    tournament->winner_id = DEFAULT_WINNER_VALUE;
    tournament->players = mapCreate(intDataCopy, intKeyCopy, intDataDestroy, intKeyDestroy, intKeysCompare); 
    if(tournament->players == NULL){
        mapDestroy(tournament->games);
        free(tournament);
        return NULL;
    }
    tournament->deleted_players = mapCreate(intDataCopy, intKeyCopy, intDataDestroy, intKeyDestroy, intKeysCompare); 
    if(tournament->deleted_players == NULL){
        mapDestroy(tournament->games);
        mapDestroy(tournament->players);
        free(tournament);
        return NULL;
    }

    return tournament;
}

void tournamentDestroy(Tournament tournament){
    if(tournament == NULL){
        return;
    }

    mapDestroy(tournament->games);
    mapDestroy(tournament->players);
    mapDestroy(tournament->deleted_players);
    free(tournament);
}

Tournament tournamentCopy(Tournament tournament){
    if(tournament == NULL){
        return NULL;
    }

    Tournament tournament_cpy = tournamentCreate(
        tournament->id, tournament->location, tournament->max_games_per_player);
    if(tournament_cpy == NULL){
        return NULL;
    }

    mapDestroy(tournament_cpy->games);
    tournament_cpy->games = mapCopy(tournament->games);
    if(tournament_cpy->games == NULL && tournament->games != NULL){
        tournamentDestroy(tournament_cpy);
        return NULL;
    }

    mapDestroy(tournament_cpy->players);
    tournament_cpy->players = mapCopy(tournament->players);
    if(tournament_cpy->players == NULL && tournament->players != NULL){
        tournamentDestroy(tournament_cpy);
        return NULL;
    }

    mapDestroy(tournament_cpy->deleted_players);
    tournament_cpy->deleted_players = mapCopy(tournament->deleted_players);
    if(tournament_cpy->deleted_players == NULL && tournament->deleted_players != NULL){
        tournamentDestroy(tournament_cpy);
        return NULL;
    }

    tournament_cpy->winner_id = tournament->winner_id;
    tournament_cpy->id = tournament->id;
    tournament_cpy->location = tournament->location;
    tournament_cpy->next_game_id = tournament->next_game_id;
    tournament_cpy->winner_id = tournament->winner_id;
    tournament_cpy->max_games_per_player = tournament->max_games_per_player;

    return tournament_cpy;
}

bool tournamentIsEnded(Tournament tournament){   
    if(tournament == NULL){
        return false;
    }
    return tournament->winner_id != DEFAULT_WINNER_VALUE;
}


ChessResult tournamentAddGame(Tournament tournament, int first_player, int second_player,
                                Winner winner, int play_time)
{
    if(tournament == NULL){
        return CHESS_NULL_ARGUMENT;
    }
    
    int gameId = tournamentGetId(tournament);
    Game new_game = gameCreate(first_player, second_player, winner, play_time, gameId);
    if(new_game == NULL){
        return CHESS_OUT_OF_MEMORY;
    }

    if(mapPut(tournament->games, &gameId, new_game) != MAP_SUCCESS){
        gameDestroy(new_game);
        return CHESS_OUT_OF_MEMORY;
    }

    int updated_game_count1 = tournamentPlayerGameCount(tournament, first_player) +1;
    int updated_game_count2 = tournamentPlayerGameCount(tournament, second_player) +1;
    
    if(mapPut(tournament->players, &first_player, &updated_game_count1) != MAP_SUCCESS){
        gameDestroy(new_game);
        return CHESS_OUT_OF_MEMORY;
    }

    if(mapPut(tournament->players, &second_player, &updated_game_count2) != MAP_SUCCESS){
        gameDestroy(new_game);
        return CHESS_OUT_OF_MEMORY;
    }
    mapRemove(tournament->deleted_players, &first_player);
    mapRemove(tournament->deleted_players, &second_player);

    gameDestroy(new_game);
    return CHESS_SUCCESS;
}



void tournamentRemovePlayer(Tournament tournament, int player_id){
    if(tournament == NULL){
        return;
    }
    
    if(mapContains(tournament->players, &player_id)){
        int *empty_data_ptr;
        int empty_data = EMPTY_DATA;
        empty_data_ptr = &empty_data;
        if(mapPut(tournament->deleted_players, &player_id, empty_data_ptr) != MAP_SUCCESS){
            return;
        }
        mapRemove(tournament->players, &player_id);
    }
    MAP_FOREACH(int*, game_id, tournament->games)                   
    {
        Game cur_game = mapGet(tournament->games, game_id);         
        gameRemovePlayer(cur_game, player_id);
        free(game_id);
    }

}

bool tournamentPlayerExist(Tournament tournament, int playerId){
    if(tournament == NULL){
        return false;
    }
    return mapContains(tournament->players, &playerId);
}


Map tournamentGetGames(Tournament tournament){
    if(tournament == NULL){
        return NULL;
    }
    return tournament->games;
}


int tournamentPlayerTime(Tournament tournament, int player_id){
    if(tournament == NULL){
        return POSITIVE_INTEGER_ERROR_RESULT;
    }

    int count_time = 0;
    MAP_FOREACH(int*, game_id, tournament->games){
        Game game = mapGet(tournament->games, game_id);
        if(gameGetPlayerOneId(game) == player_id || gameGetPlayerTwoId(game) == player_id){
           count_time += getTime(game);
        }
        free(game_id);
    }

    return count_time;
}

int tournamentPlayerGameCount(Tournament tournament, int player_id){
    if(tournament == NULL){
        return POSITIVE_INTEGER_ERROR_RESULT;
    }

    if(!mapContains(tournament->players, &player_id)){
        return 0;
    }

    MapDataElement games_count_ptr = mapGet(tournament->players, &player_id);
    if(games_count_ptr == NULL){
        return 0;
    }

    return (int)(*(int*)games_count_ptr);
}

int tournamentMaxGamesPerPlayer(Tournament tournament){
    if(tournament == NULL){
        return POSITIVE_INTEGER_ERROR_RESULT;
    }
    return tournament->max_games_per_player;
}

ChessResult tournamentEnd(Tournament tournament){
    if(tournament == NULL){
        return CHESS_NULL_ARGUMENT;
    }

    if(mapGetSize(tournament->games) <= 0){
        return CHESS_NO_GAMES;
    }

    if(tournamentIsEnded(tournament)){
        return CHESS_TOURNAMENT_ENDED;
    }

    tournamentSetWinner(tournament);
    return CHESS_SUCCESS;
}

bool tournamentContainsGame(Tournament tournament, int player1_id, int player2_id){
    MAP_FOREACH(int*, index, tournament->games){
        Game cur_game = mapGet(tournament->games, index);
        if(gameIsEqual(cur_game, player1_id, player2_id)){
            free(index);
            return true;
        }
        free(index);
    }
    return false;
}

void tournamentSetWinner(Tournament tournament){
    if(tournament == NULL){
        return;
    }

    int wins = 0;
    int draws = 0;
    int loss = 0;
    int points = 0;
    int winnerId = 0;
    int winner_wins = 0;
    int winner_loss = 0;
    int winner_points = 0;


    MAP_FOREACH(int*, playerId, tournament->players)
    {
        MAP_FOREACH(int*, game_id, tournament->games)
        {
            Game cur_game = mapGet(tournament->games, game_id);
            if(gameIsWin(cur_game, *playerId)){
                wins++;
            }
            if(gameIsDraw(cur_game, *playerId)){
                draws++;
            }
            if(gameIsLoss(cur_game, *playerId)){
                loss++;
            }
            free(game_id);
        }
        points = 2*wins + draws;               
        if(points > winner_points){  
            winnerId = *playerId;
            winner_wins = wins;
            winner_loss = loss;
            winner_points = points;
        }
        if(points == winner_points){
            if(loss < winner_loss){
                winnerId = *playerId;
                winner_wins = wins;
                winner_loss = loss;
                winner_points = points;
            }
        }
        if(points == winner_points){
            if(loss == winner_loss){
                if(wins > winner_wins){
                    winnerId = *playerId;
                    winner_wins = wins;
                    winner_loss = loss;
                    winner_points = points;
                }
            }
        }
        if(points == winner_points){
            if(loss == winner_loss){
                if(wins == winner_wins){
                    if(*playerId < winnerId){
                    winnerId = *playerId;
                    winner_wins = wins;
                    winner_loss = loss;
                    winner_points = points;
                    }
                }
            }
        }
        points = 0;
        wins = 0;
        draws = 0;
        loss = 0;
        free(playerId);
    }

    tournament->winner_id = winnerId;
}






int tournamentGetId(Tournament tournament) {
    if(tournament == NULL){
        return POSITIVE_INTEGER_ERROR_RESULT;
    }

    return tournament->next_game_id++;
}



int tournamentGetWinner(Tournament tournament){
    if(tournament == NULL){
        return POSITIVE_INTEGER_ERROR_RESULT;
    }
    return tournament->winner_id;
}

int tournamentGetLongestTime(Tournament tournament){
    if(tournament == NULL){
        return POSITIVE_INTEGER_ERROR_RESULT;
    }

    int longest_time = 0;
    MAP_FOREACH(int*, game_id, tournament->games)
    {
        Game cur_game = mapGet(tournament->games, game_id);
        if(getTime(cur_game) > longest_time)
        {
            longest_time = getTime(cur_game);
        }
        free(game_id);
    }
    return longest_time;
}

double tournamentAvgTime(Tournament tournament){
    if(tournament == NULL){
        return POSITIVE_INTEGER_ERROR_RESULT;
    }

    int count_games = mapGetSize(tournament->games);
    if(count_games == 0){
        return 0;
    }

    int count_time = 0;
    MAP_FOREACH(int*, game_id, tournament->games){
        Game cur_game = mapGet(tournament->games, game_id);
        count_time += getTime(cur_game);
        free(game_id);
    }

    return (double)count_time/(double)count_games;
}

const char* tournamentGetLocation(Tournament tournament){
    return tournament->location;
}

int tournamentGetNumGames(Tournament tournament){
    if(tournament == NULL){
        return POSITIVE_INTEGER_ERROR_RESULT;
    }

    return mapGetSize(tournament->games);
}

int tournamentGetNumPlayers(Tournament tournament){
    if(tournament == NULL){
        return 0;
    }

    return mapGetSize(tournament->players);
}
int tournamentDeletedPlayers(Tournament tournament){
    if(tournament == NULL){
        return 0;
    }
    return mapGetSize(tournament->deleted_players);
}




void tournamentDestroyPTR(void* tournament){
    tournamentDestroy((Tournament)tournament);
}

void* tournamentCopyPTR(void* tournament){
    return (void*)tournamentCopy((Tournament)tournament);
}

void gameDestroyPTR(void* game){
    gameDestroy((Game)game);
}

void* gameCopyPTR(void* game){
    return (void*)gameCopy((Game)game);
}


