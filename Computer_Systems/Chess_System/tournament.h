#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#include <stdio.h>
#include <stdbool.h>

#define POSITIVE_INTEGER_ERROR_RESULT -1
#define DEFAULT_WINNER_VALUE -1


/** Type for representing a chess system that organizes chess tournaments */
typedef struct tournament_t *Tournament;

/**
 * tournamentCreate: create a new empty tournament.
 *
 * @return A new tournament in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
Tournament tournamentCreate(int id, const char* location, int max_games_per_player);

/**
 * tournamentDestroy: free a tournament, and all its contents, from memory.
 *
 * @param tournament - the tournament to free from memory. A NULL value is
 *     allowed, and in that case the function does nothing.
 */
void tournamentDestroy(Tournament tournament);

/**
 * tournamentCopy: copy a tournament.
 *
 * @param tournament - the tournament to copy.
 * 
 * @return A copy of the given tournament.
 */
Tournament tournamentCopy(Tournament tournament);

bool tournamentIsEnded(Tournament tournament);

ChessResult tournamentAddGame(Tournament tournament, int first_player, int second_player,
                                Winner winner, int play_time);

void tournamentRemovePlayer(Tournament tournament, int player_id);

ChessResult tournamentEnd(Tournament tournament);

bool tournamentContainsGame(Tournament tournament, int player1_id, int player2_id);

void tournamentSetWinner(Tournament tournament);

bool tournamentPlayerExist(Tournament tournament, int playerId);

int tournamentPlayerTime(Tournament tournament, int player_id);

int tournamentPlayerGameCount(Tournament tournament, int player_id);

int tournamentMaxGamesPerPlayer(Tournament tournament);

Map tournamentGetGames(Tournament tournament); 

int tournamentGetWinner(Tournament tournament);

int tournamentGetId(Tournament tournament);

int tournamentGetLongestTime(Tournament tournament);  

double tournamentAvgTime(Tournament tournament);     

const char* tournamentGetLocation(Tournament tournament);    

int tournamentGetNumGames(Tournament tournament);  

int tournamentGetNumPlayers(Tournament tournament);

int tournamentDeletedPlayers(Tournament tournament);




void tournamentDestroyPTR(void* tournament);
void* tournamentCopyPTR(void* tournament);

void gameDestroyPTR(void* game);
void* gameCopyPTR(void* game);


#endif //_TOURNAMENT_H
