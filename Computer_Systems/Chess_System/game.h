#ifndef _GAME_H
#define _GAME_H

#include <stdio.h>
#include "./game.h"
#include "./chessSystem.h"

// typedef enum {
    
// } Game_Result;

/** Type for representing a chess system that organizes chess tournaments */
typedef struct game_t *Game;

/**
 * gameCreate: create game.
 *
 * @return A new game in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
Game gameCreate(int player1_id, int player2_id, Winner winner, int duration, int gameId);

/**
 * gameDestroy: free a game, and all its contents, from memory.
 *
 * @param game - the game to free from memory. A NULL value is
 *     allowed, and in that case the function does nothing.
 */
void gameDestroy(Game game);

/**
 * gameCopy: return a copy of a given game.
 *
 * @param game - the game to free from memory. A NULL value is
 *     allowed, and in that case the function does nothing.
 * 
 * @return A copy of the given game in case of success, and NULL otherwise.
 */
Game gameCopy(Game game);

/**
 * setWinner: set the winner of the game.
 * 
 * @param: new_winner - the new winner.
 */
void setWinner(Game game, int new_winner);

bool gameIsWin(Game game, int id);
bool gameIsDraw(Game game, int id);
bool gameIsLoss(Game game, int id);

int getTime(Game game);
void setTime(Game game,int newtime);

void gameRemovePlayer(Game game, int player_id);

int gameGetPlayerOneId(Game game);
int gameGetPlayerTwoId(Game game);

bool gameIsEqual(Game game, int first_player, int second_player);

#endif //_GAME_H