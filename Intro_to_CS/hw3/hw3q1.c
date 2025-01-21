#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#define N 11
#define COL 121
#define ROW 2
#define TWO 2
#define SPACE '_'
#define X 'X'
#define O 'O'
void print_welcome();
void print_enter_board_size();
void print_board(char board[N][N], int n);
void print_player_turn(int player_index);
void print_error();
void print_winner(int player_index);
void print_tie();
void createmat(char board[N][N], int size);
bool thereisplace(char board[N][N], int size,int row, int col);
bool lost(char board[N][N], int size, int row, int col);
bool isvalidmove(char board[N][N], int size, int row, int col);
void createUndoMat(int undomat[ROW][COL]);
int undo(char board[N][N], int undoArray[ROW][COL], int goback, int count,int size);
void startgame(char board[N][N], int undoArray[ROW][COL], char symble[TWO], int size, int row, int col, int count, bool printagain);
bool updateBoard(char board[N][N], char symble[TWO], int size, int row, int col, int count);
int readRow(bool printagain, int count);
int main() {
    int undoArray[ROW][COL];
    createUndoMat(undoArray);
    bool printagain = true;
    int size = 1, row = 1, col = 1, count = 0;    //define all my varribles
    char symble[] = { O,X };                  //we need to know whos turn it is and put the correct symble
    char board[N][N];
    print_welcome();
    print_enter_board_size();
    scanf("%d", &size);                            //read board size
    createmat(board,size);
    print_board(board, size);
    startgame(board,undoArray,symble,size,row,col,count,printagain);   //game begins
}
void startgame(char board[N][N], int undoArray[ROW][COL], char symble[TWO], int size, int row, int col, int count, bool printagain) {
    while (thereisplace(board, size,row,col))                 //stop when there is no space left
    {
        row=readRow(printagain,count);                                          //read row and print if needed
        if (row < 0 && row % TWO + 1 == 0 && 0 <= count + row && count < COL) {   //if undo is legal move
            count = undo(board, undoArray, row * (-1), count - 1,size);         //update the counter and do undo
            printagain = true;
            continue;
        }
        scanf("%d", &col);                                                      //read column
        if (!isvalidmove(board, size, row - 1, col - 1)) {                      //check if move is valid
            printagain = false;
            continue;
        }
        if (updateBoard(board,symble,size,row, col, count))                     //function that update board
            break;
        undoArray[0][count] = row - 1;                                          //update undo array, wont go out of array because
        undoArray[1][count] = col - 1;                                          // count is updated in undo function
        printagain = true;          
        count++;
    }
}

int readRow(bool printagain,int count) {
    int row = 1;
    if (printagain)
        print_player_turn(count % TWO + 1);
    scanf("%d", &row);
    return row;
}

bool updateBoard(char board[N][N],char symble[TWO],int size,int row,int col,int count) {
    board[row - 1][col - 1] = symble[(count + 1) % TWO];   //put x or o in board
    print_board(board, size);
    if (lost(board, size, row - 1, col - 1)) {               //check if player lost
        print_winner(TWO - count % TWO);
        return true;
    }
    return false;
}


int undo(char board[N][N],int undoArray[ROW][COL], int goback, int count,int size) {
    int row = undoArray[0][count];               //go to history and put the board back
    int col = undoArray[1][count];               //where it was a few steps ago
    for (int i = 0; i < goback; i++)
    {
        board[row][col] = SPACE;
        undoArray[0][count] = -1;                //and update the history arry
        undoArray[1][count] = -1;
        count--;
        row = undoArray[0][count];                
        col = undoArray[1][count];
    }
    print_board(board, size);
    return count+1;                             //return the updated count
}
void createUndoMat(int undomat[ROW][COL]) {
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            undomat[i][j] = -1;;
        }
    }
}
bool isvalidmove(char board[N][N], int size,int row,int col) {
    if (row < size && row >= 0 && col < size && col >= 0) {
        if (board[row][col] != SPACE) {                              //check if the player is not puting
            print_error();                                         //x or o on a place there is already symble
            return false;
        }
        if (row < size && row >= 0 && col < size && col >= 0)     //and check that he doesnt put them outside the matrix
            return true;
    }
    print_error();
    return false;
}
bool lost(char board[N][N], int size, int row, int col) {
    if (board[row][col] == SPACE)
        return false;
    bool flag1 = true, flag2 = true, flag3 = row == col, flag4 = row == size - col - 1;
    for (int i = 0; i < size; i++)
    {
        if (board[row][i] != board[row][col])                          //check the row
            flag1 = false;
        if (board[i][col] != board[row][col])                           //check the column
            flag2 = false;
        if (row == col&&board[i][i] != board[row][col])               //check diagnol
            flag3 = false;
        if (row == size - col - 1&& board[i][size - 1 - i] != board[row][col])//secound diagnol
                flag4 = false;  
    }
        return flag1 || flag2 || flag3 || flag4;                       //if one is true then gameover
}
bool thereisplace(char board[N][N], int size,int row,int col){
    for (int i = 0; i < size; i++)                               //checks that there is more space on the board
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] == SPACE)
                return true;
        }
    }
    if (!lost(board, size, row - 1, col - 1))                       //check if tie
        print_tie();
    return false;
}
void createmat(char board[N][N],int size) {
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            board[i][j] = SPACE;
        }
    }
}
//print welcome message
//1 lines
void print_welcome()
{
    printf("*** Welcome to AVOIDANCE TIC-TAC-TOE game ***\n\n");
}

//print message to enter board size
//1 lines
void print_enter_board_size()
{
    printf("Please enter board size (1 to %d):\n", N);
}

//print the board
//7 lines
void print_board(char board[N][N], int n)
{
    printf("\nCurrent board:\n");
    for (int i = 0; i < n; i++)
    {
        printf("|");
        for (int j = 0; j < n; j++)
        {
            printf("%c|", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//print a request for player with index "player_index" to move
//1 lines
void print_player_turn(int player_index)
{
    printf("\nPlayer ** %d **, enter next move:\n", player_index);
}

//print error message
//1 lines
void print_error()
{
    printf("Illegal move!!!, please try again:\n");
}

//print the winner
//1 lines
void print_winner(int player_index)
{
    printf("Player %d Wins! Hooray!\n", player_index);
}

//print message to announce there is a tie (no one wins)
//1 lines
void print_tie()
{
    printf("It's a tie!\n");
}