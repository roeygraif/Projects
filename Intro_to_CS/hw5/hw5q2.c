/*-------------------------------------------------------------------------
  Include files:
--------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/*=========================================================================
  Constants and definitions:
==========================================================================*/

// Print Functions' Declarations
void printNumCoinsInputMessage();

void printCoinValsInputMessage();

void printSumInputMessage();

void printLimitInputMessage();

void printAnswerMessage(int n);

void printAllocationError();
void findPar(int* valArr, int n, int sum, int limit, int* count);
/*-------------------------------------------------------------------------
  The main program. (describe what your program does here)
 -------------------------------------------------------------------------*/
int main() {
    printNumCoinsInputMessage();
    int n = 0;
    scanf("%d",&n);
    printCoinValsInputMessage();
    int count[1] = {0};
    int* valArr = malloc((n+1)*sizeof(int));
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &valArr[i]);
    }
    printSumInputMessage();
    int sum = 0;
    scanf("%d",&sum);
    printLimitInputMessage();
    int limit = 0;
    scanf("%d", &limit);
    findPar(valArr,n,sum,limit,count);
    printAnswerMessage(*count);
    return 0;
}

void findPar(int* valArr,int n,int sum, int limit,int* count) {
    if (sum == 0 && limit >= 0) //success
    {
        *count=*count+1;
        return;
    }
    
    if (sum < 0 || limit < 0 || n == 0) // failed
        return;
    if (valArr[0]<=sum) // move in the same path
        findPar(valArr, n , sum - valArr[0], limit - 1, count);  //go dowwn the tree
    findPar(++valArr, n - 1, sum, limit, count);                 //go to the next brunch
}
void printNumCoinsInputMessage() {
    printf("Please enter coins num:\n");
}

void printCoinValsInputMessage() {
    printf("Please enter coins vals:\n");
}

void printSumInputMessage() {
    printf("Please enter the requested sum:\n");
}

void printLimitInputMessage() {
    printf("Please enter the coins limit:\n");
}

void printAnswerMessage(int n) {
    printf("There are %d possible partitions.\n", n);
}

void printAllocationError() {
    printf("ERROR: allocation failed.\n");
}