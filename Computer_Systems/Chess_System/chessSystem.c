#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <float.h>
#include "./utils.h"
#include "./map.h"
#include "./game.h"
#include "./tournament.h"
#include "./chessSystem.h"

#define EMPTY_DATA -1
int chessPlayerWins(ChessSystem chess, int playerId);
int chessPlayerDraws(ChessSystem chess, int playerId);
int chessPlayerLosses(ChessSystem chess, int playerId);
int chessPlayerGamesCount(ChessSystem chess, int playerId);

struct chess_system_t {
    Map tournaments;
    Map players;   
};

ChessSystem chessCreate(){
    ChessSystem chess_sys = malloc(sizeof(*chess_sys));
    if (chess_sys == NULL){
        return NULL;
    }

    chess_sys->tournaments = mapCreate( tournamentCopyPTR,
                                        intKeyCopy,
                                        tournamentDestroyPTR,
                                        intKeyDestroy,
                                        intKeysCompare);

    if (chess_sys->tournaments == NULL){
        free(chess_sys);
        return NULL;
    }

    chess_sys->players = mapCreate(intDataCopy, intKeyCopy, intDataDestroy, intKeyDestroy, intKeysCompare);
    if (chess_sys->players == NULL){
        mapDestroy(chess_sys->tournaments);
        free(chess_sys);
        return NULL;
    }

    return chess_sys;
}

void chessDestroy(ChessSystem chess){
    if (chess == NULL){
        return;
    }

    mapDestroy(chess->tournaments);
    mapDestroy(chess->players);
    free(chess);
}

ChessResult chessAddTournament(ChessSystem chess, int tournament_id,
                               int max_games_per_player, const char *tournament_location)
{
    if(chess == NULL){
        return CHESS_NULL_ARGUMENT;
    }
    
    if (!isValidId(tournament_id)){
        return CHESS_INVALID_ID;
    }
    
    if (mapContains(chess->tournaments, &tournament_id)){
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }
    
    if(tournament_location == NULL){
        return CHESS_NULL_ARGUMENT;
    }

    if (!isValidLocation(tournament_location)){
        return CHESS_INVALID_LOCATION;
    }
    
    if (max_games_per_player <= 0){
        return CHESS_INVALID_MAX_GAMES;
    }
    
    Tournament new_tournament = tournamentCreate(tournament_id, tournament_location, max_games_per_player);
    if(new_tournament == NULL){
        return CHESS_OUT_OF_MEMORY;
    }
    if(mapPut(chess->tournaments, &tournament_id, new_tournament) != MAP_SUCCESS){
        tournamentDestroy(new_tournament);
        return CHESS_OUT_OF_MEMORY;
    }

    tournamentDestroy(new_tournament);
    return CHESS_SUCCESS;
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
                         int second_player, Winner winner, int play_time)
{
    if(chess == NULL){
        return CHESS_NULL_ARGUMENT;
    }

    bool tour_valid = isValidId(tournament_id);
    bool first_valid = isValidId(first_player);
    bool second_valid = isValidId(second_player);
    if (!tour_valid || !first_valid || !second_valid || first_player == second_player){
        return CHESS_INVALID_ID;
    }

    if (!mapContains(chess->tournaments, &tournament_id)){
        return CHESS_TOURNAMENT_NOT_EXIST;
    }

    Tournament current_tournament = mapGet(chess->tournaments, &tournament_id);
    if(tournamentIsEnded(current_tournament)){
        return CHESS_TOURNAMENT_ENDED;
    }

    if(tournamentContainsGame(current_tournament, first_player, second_player)){
        return CHESS_GAME_ALREADY_EXISTS;
    }
    
    if(play_time < 0){
        return CHESS_INVALID_PLAY_TIME;
    }

    int max_games_per_player = tournamentMaxGamesPerPlayer(current_tournament);
    int player1_game_count = tournamentPlayerGameCount(current_tournament, first_player);
    int player2_game_count = tournamentPlayerGameCount(current_tournament, second_player);
    if(player1_game_count >= max_games_per_player || player2_game_count >= max_games_per_player){
        return CHESS_EXCEEDED_GAMES;
    }

    int *empty_data_ptr;
    int empty_data = EMPTY_DATA;
    empty_data_ptr = &empty_data;

    if(!mapContains(chess->players, &first_player)){
        if(mapPut(chess->players, &first_player, empty_data_ptr) != MAP_SUCCESS){
            return CHESS_OUT_OF_MEMORY;
        }
    }

    if(!mapContains(chess->players, &second_player)){
        if(mapPut(chess->players, &second_player, empty_data_ptr) != MAP_SUCCESS){
            return CHESS_OUT_OF_MEMORY;
        }
    }
    
    return tournamentAddGame(current_tournament, first_player, second_player, winner, play_time);
}

ChessResult chessRemoveTournament(ChessSystem chess, int tournament_id)
{
    if(chess == NULL){
        return CHESS_NULL_ARGUMENT;
    }

    if(!isValidId(tournament_id)){
        return CHESS_INVALID_ID;
    }

    if(!mapContains(chess->tournaments, &tournament_id)){
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    
    mapRemove(chess->tournaments, &tournament_id);
    return CHESS_SUCCESS;

}

ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (!isValidId(player_id))
    {
        return CHESS_INVALID_ID;
    }
    if (!mapContains(chess->players, &player_id))
    {
        return CHESS_PLAYER_NOT_EXIST;
    }
    if(mapRemove(chess->players, &player_id) == MAP_ITEM_DOES_NOT_EXIST){
        return CHESS_PLAYER_NOT_EXIST;
    }
    MAP_FOREACH(int*, tour_id, chess->tournaments)
    {
        Tournament cur_tournament = mapGet(chess->tournaments, tour_id);
        if (!tournamentIsEnded(cur_tournament))
        {
            tournamentRemovePlayer(cur_tournament, player_id);
        }
        free(tour_id);
    }
    return CHESS_SUCCESS;
}

ChessResult chessEndTournament(ChessSystem chess, int tournament_id)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (!isValidId(tournament_id))
    {
        return CHESS_INVALID_ID;
    }
    if (!mapContains(chess->tournaments, &tournament_id))
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    return tournamentEnd(mapGet(chess->tournaments, &tournament_id));
}

int chessCountEndedTournaments(ChessSystem chess){
    int count = 0;
    MAP_FOREACH(int*, tour_id, chess->tournaments){
        Tournament tour = mapGet(chess->tournaments, tour_id);
        if(tournamentIsEnded(tour)){
            count++;
        }
        free(tour_id);
    }
    return count;
}

double chessCalculateAveragePlayTime(ChessSystem chess, int player_id, ChessResult *chess_result)
{
    if (chess == NULL)
    {
        *chess_result = CHESS_NULL_ARGUMENT;
        return -1;                                             
    }
    if (!isValidId(player_id))
    {
        *chess_result = CHESS_INVALID_ID;
        return -1;
    }
    if (!mapContains(chess->players, &player_id))
    {
        *chess_result = CHESS_PLAYER_NOT_EXIST;
        return -1;
    }
    int total_time = 0;
    int countGames = 0;
    MAP_FOREACH(int*, tour_id, chess->tournaments)
    {
        Tournament cur_tournament = mapGet(chess->tournaments, tour_id);
        if (tournamentPlayerExist(cur_tournament, player_id))
        {
            total_time += tournamentPlayerTime(cur_tournament, player_id);
            countGames += tournamentPlayerGameCount(cur_tournament, player_id); //amount of games a player played
        }
        free(tour_id);
    }
    *chess_result = CHESS_SUCCESS;
    if(countGames == 0){
        return 0;
    }
    return (double)total_time / (double)countGames;
}

ChessResult chessSavePlayersLevels(ChessSystem chess, FILE *file)
{
    if (chess == NULL || file == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    Map players_cpy = mapCopy(chess->players);
    if(players_cpy == NULL){
        return CHESS_OUT_OF_MEMORY;
    }
    int size = mapGetSize(chess->players);
    
    
    for (int i = 0; i < size; i++){
        double best_level = -DBL_MAX;
        int best_player_id = 0;
        MAP_FOREACH(int*, cur_player, players_cpy){
            double level =0;
            if(chessPlayerGamesCount(chess, (*cur_player)) != 0){
                int win_points = 6 * chessPlayerWins(chess, (*cur_player));
                int loss_points = 10 * chessPlayerLosses(chess, (*cur_player));
                int draw_points = 2 * chessPlayerDraws(chess, (*cur_player));
                int player_game_count = chessPlayerGamesCount(chess, (*cur_player));
                level = ((double)(win_points - loss_points + draw_points)) / ((double)player_game_count);
            }
            if(level > best_level){
                best_level = level;
                best_player_id = *cur_player;
            }
            free(cur_player);
        }
        if(fprintf(file, "%d %.2f\n", best_player_id, best_level) < 0){
            mapDestroy(players_cpy);
            return CHESS_SAVE_FAILURE; 
        }
        mapRemove(players_cpy, &best_player_id);
    }
    mapDestroy(players_cpy);
    return CHESS_SUCCESS;
}

ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file){
    if(chess == NULL || path_file == NULL){
        return CHESS_NULL_ARGUMENT;
    }

    if(mapGetSize(chess->tournaments) <= 0 || chessCountEndedTournaments(chess) <= 0){
        return CHESS_NO_TOURNAMENTS_ENDED;
    }

    FILE* file = fopen(path_file, "w");

    MAP_FOREACH(int*, tour_id, chess->tournaments){
        Tournament tour = mapGet(chess->tournaments, tour_id);
        if(tournamentIsEnded(tour)){
            if(fprintf(file, "%d\n", tournamentGetWinner(tour)) < 0      ||
               fprintf(file, "%d\n", tournamentGetLongestTime(tour)) < 0 ||
               fprintf(file, "%.2lf\n", tournamentAvgTime(tour)) < 0     ||
               fprintf(file, "%s\n", tournamentGetLocation(tour)) < 0    ||
               fprintf(file, "%d\n", tournamentGetNumGames(tour)) < 0    ||
               fprintf(file, "%d\n", (tournamentGetNumPlayers(tour) + tournamentDeletedPlayers(tour))) < 0
            ){
                fclose(file);
                free(tour_id);
                return CHESS_SAVE_FAILURE;  
            }
        }
        free(tour_id);
    }
    fclose(file);
    return CHESS_SUCCESS;
}






int chessPlayerWins(ChessSystem chess, int playerId){
    int countWins = 0;
    MAP_FOREACH(int*, tour_id, chess->tournaments){
        Tournament tour = mapGet(chess->tournaments, tour_id);
        Map games = tournamentGetGames(tour);       
        MAP_FOREACH(int*, game_id, games){    
            Game game = mapGet(games, game_id);
            if(gameIsWin(game, playerId)){
                countWins++;
            }
            free(game_id);
        }
        free(tour_id);
    }
    return countWins;
}

int chessPlayerLosses(ChessSystem chess, int playerId){
    int countLoss = 0;
    MAP_FOREACH(int*, tour_id, chess->tournaments){
        Tournament tour = mapGet(chess->tournaments, tour_id);
        Map games = tournamentGetGames(tour);       
        MAP_FOREACH(int*, game_id, games){    
            Game game = mapGet(games, game_id);
            if(gameIsLoss(game, playerId)){
                countLoss++;
            }
            free(game_id);
        }
        free(tour_id);
    }
    return countLoss;
}

int chessPlayerDraws(ChessSystem chess, int playerId){
    int countDraws = 0;
    MAP_FOREACH(int*, tour_id, chess->tournaments){
        Tournament tour = mapGet(chess->tournaments, tour_id);
        Map games = tournamentGetGames(tour);       
        MAP_FOREACH(int*, game_id, games){    
            Game game = mapGet(games, game_id);
            if(gameIsDraw(game, playerId)){
                countDraws++;
            }
            free(game_id);
        }
        free(tour_id);
    }
    return countDraws;
}

int chessPlayerGamesCount(ChessSystem chess, int playerId){
    int count_games = 0;
    MAP_FOREACH(int*, tour_id, chess->tournaments)
    {
        Tournament cur_tournament = mapGet(chess->tournaments, tour_id);
        count_games += tournamentPlayerGameCount(cur_tournament, playerId);
        free(tour_id);
    }
    return count_games;
}