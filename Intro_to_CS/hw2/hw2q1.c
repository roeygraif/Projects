#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#define MAT_SIZE 4
int is_strong(int mat[MAT_SIZE][MAT_SIZE], int row, int column);
int space_rank(int mat[MAT_SIZE][MAT_SIZE], int row, int column);
int is_strong(int mat[MAT_SIZE][MAT_SIZE], int row, int column) {
	bool rightExist = (MAT_SIZE - 1 > column);										//if right exist
	bool rightIsBig = rightExist && (mat[row][column] <= mat[row][column + 1]);     //first will check is that right exist if not it will stop if and return false
	bool leftExist = (0 != column);													//same like before but for left
	bool leftIsBig = leftExist && (mat[row][column] <= mat[row][column - 1]);		
	bool upExist = (0 != row);
	bool upIsBig = upExist && (mat[row][column] <= mat[row - 1][column]);
	bool downExist = (MAT_SIZE - 1 > row);
	bool downIsBig = downExist && (mat[row][column] <= mat[row + 1][column]);
	return rightIsBig + leftIsBig + upIsBig + downIsBig == 0;						//if they all return false that means the middle number is the biggest!
}
int space_rank(int mat[MAT_SIZE][MAT_SIZE], int row, int column) {
	bool lastcol = (MAT_SIZE - 1 == column);								//check if we are in the last colomn
	int numOnRight = mat[row][column + (int)(!lastcol)];					//if not we can check who is num on right
	int sum = mat[row][column] - (((int)!lastcol) * numOnRight);            //find the sum between the two numbers
	bool sumIsZero = (sum == 0);											//we need to make sure we are not deviding by zero
	int SumPosOrNeg = (sum < 0) - 2 * (sum < 0) + (sum > 0);				//we need to know if the sum is neg or pos
	return ((sum * sum) / (sum + (int)(sumIsZero))) * (!lastcol) * (SumPosOrNeg);//we will multiply the sum then devide(not by zero)and multiplay by 1 or -1 depending if num is pos or neg
}
int main() {
	int mat[MAT_SIZE][MAT_SIZE];
	int num = 0, countStrong = 0, countSpace = 0;
	printf("Please enter a matrix:\n");
	for (int row = 0; row < MAT_SIZE; row++){
		for (int col = 0; col < MAT_SIZE; col++){
			scanf("%d", &num);
			mat[row][col] = num;					//create matrix
		}
	}
	for (int row = 0; row < MAT_SIZE; row++){
		for (int col = 0; col < MAT_SIZE; col++){
			countStrong += (int)(is_strong(mat, row, col));    //count strong for all matrix
			countSpace += space_rank(mat, row, col);		   //count space for all matrix
		}
	}
	printf("Strong elements: %d\n", countStrong);
	printf("Space rank: %d\n", countSpace);
	return 1;
}