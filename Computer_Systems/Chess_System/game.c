#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

//#include "./utils.h"
#include "./game.h"
//#include "./chessSystem.h"
#define SET_DRAW -1
#define DELETED_PLAYER_ID -1

struct game_t {
    int id;
    int player1_id;
    int player2_id;
    Winner winner;
    int duration;
};



Game gameCreate(int player1_id, int player2_id, Winner winner, int duration, int gameId){
    Game game = malloc(sizeof(*game));
    if(game == NULL){
        return NULL;
    }

    game->id = gameId;
    game->player1_id = player1_id;
    game->player2_id = player2_id;
    game->winner = winner;
    game->duration = duration;
    return game;
}

void gameDestroy(Game game){
    if(game == NULL){
        return;
    }
    
    free(game);
}

Game gameCopy(Game game){
    Game new_game = gameCreate(game->player1_id, game->player2_id, game->winner, game->duration, game->id);
    if(new_game == NULL){
        return NULL;
    }

    return new_game;
}

void setWinner(Game game, int new_winner){
    if(game == NULL)
    {
        return;
    }
    if(new_winner == game->player1_id)
    {
        game->winner = FIRST_PLAYER;
        return;
    }
    if(new_winner == game->player2_id)
    {
        game->winner = SECOND_PLAYER;
        return;
    }
    if(new_winner == SET_DRAW)  //-1 id is draw
    {
        game->winner = DRAW;
        return;
    }
}

bool gameIsWin(Game game, int id){
    if(game == NULL)
    {
        return false;
    }
    if(id == game->player1_id && game->winner == FIRST_PLAYER)
    {
        return true;
    }
    if(id == game->player2_id && game->winner == SECOND_PLAYER)
    {
        return true;
    }
    return false;
}
bool gameIsDraw(Game game, int id){
    if(game == NULL)
    {
        return false;
    }
    if((id == game->player1_id || id == game->player2_id) && game->winner == DRAW)
    {
        return true;
    }
    return false;
}
bool gameIsLoss(Game game, int id){
    if(game == NULL)
    {
        return false;
    }
    if(id == game->player1_id && game->winner == SECOND_PLAYER)
    {
        return true;
    }
    if(id == game->player2_id && game->winner == FIRST_PLAYER)
    {
        return true;
    }
    return false;
}
int getTime(Game game){
    return game->duration;
}
void setTime(Game game,int newtime){
    game->duration = newtime;
}

void gameRemovePlayer(Game game, int player_id){
    if(game->player1_id == player_id){
        game->winner = SECOND_PLAYER;
        game->player1_id = DELETED_PLAYER_ID;
    }
    if(game->player2_id == player_id){
        game->winner = FIRST_PLAYER;
        game->player2_id = DELETED_PLAYER_ID;
    }
    if(game->player1_id == DELETED_PLAYER_ID && game->player2_id == DELETED_PLAYER_ID){
        game->winner = DRAW;
    }
}


int gameGetPlayerOneId(Game game){
    return game->player1_id;
}
int gameGetPlayerTwoId(Game game){
    return game->player2_id;
}


bool gameIsEqual(Game game, int first_player, int second_player){
    if(game->player1_id == first_player && game->player2_id == second_player){
        return true;
    }
    if(game->player1_id == second_player && game->player2_id == first_player){
        return true;
    }
    return false;
}