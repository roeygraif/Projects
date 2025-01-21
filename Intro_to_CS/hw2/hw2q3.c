#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#define MAT_SIZE 11
void printGrades(int mat[MAT_SIZE],char sybol);   //prints histogram
int findMostGrade(int mat[MAT_SIZE]);				//which grade most people got
void printGrades(int mat[MAT_SIZE],char symbol) {	
	int maxGrade = findMostGrade(mat);
	int maxGradecounter = maxGrade;      //because I dont want to restart maxgraade
	printf("Histogram:\n");
	for (int i = 0; i < maxGradecounter; i++)  //we neeed number of most grades rows
	{
		for (int j = 1; j < MAT_SIZE; j++)		//this loop prints the colums for each row
		{
			if (mat[j] == maxGrade) {			//max grade goes down by one after every loop therefor if they oth the same we print symble
				if (i == maxGradecounter)       //we need to make sure we are not at the end
					printf("%c", symbol);
				else
					printf("%c ", symbol);
				mat[j]--;
			}
			else {
				if (i == maxGradecounter)        //if not print space
					printf(" ");
				else
					printf("  ");
			}
		}
		maxGrade--;
		printf("\n");
	}
	printf("1 2 3 4 5 6 7 8 9 10");
}
int findMostGrade(int mat[MAT_SIZE]) {
	int most = 0;
	for (int i = 0; i < MAT_SIZE; i++)
	{
		if (most < mat[i])               //finds the grade most people got
			most = mat[i];
	}
	return most;
}
int main() {
	char sybol = '0';
	int grade = 0;
	int matOfGrades[MAT_SIZE] = { 0 };
	printf("Please enter a character:\n");
	scanf("%c", &sybol);
	printf("Please enter grades:\n");
	//scanf("%d", &grade);
	while (scanf("%d", &grade)!=EOF)
	{
		matOfGrades[grade]++;            //add one to the grade in the array
	}
	printGrades(matOfGrades,sybol);
}