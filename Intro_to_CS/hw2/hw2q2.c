#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#define SEQ_SIZE 10
void createMat(int seq[SEQ_SIZE], int seqInt);   //function that takes int and makes array (matrix 1xn)
int findIndex(int seq[SEQ_SIZE]);          //find where the zeros begin
bool findP(int seq[SEQ_SIZE], int index, int length);//find the paladrium
void createMat(int seq[SEQ_SIZE], int seqInt) {
	for (int i = 0; i < SEQ_SIZE; i++)
	{
		seq[i] = seqInt % 10;		//take each number of the in and put in in array backwards
		seqInt = seqInt / 10;		//devide in by 10 after each time
	}
}
int findIndex(int seq[SEQ_SIZE]) {
	int index = 0;
	int i = 0;
	for (i = 0; i < SEQ_SIZE; i++)
	{
		if (seq[i] != 0)						//if is zero than put in index the index of the number in array
			index = i;
	}
	return index+1;							//return the index of the zero we want to ignore
}
bool findP(int seq[SEQ_SIZE], int index, int length) {
	int count = 0;
	for (int i = 0; i < (length); i++)				//run loop length time
	{
		if (seq[index + i] == seq[(index + length-1) - i]) {    //if the num in equal to the num+length count++
			count++;											//we need all of the num to be equal
		}
	}
	if (count == length)										//if all where equal than we have paldrium
		return true;											//return true id equal
	return false;
}
int main() {
	int length = 0, newPal = 0, seqInt = 0;
	int seq[SEQ_SIZE] = {0};
	bool found = false;
	printf("Please enter a length:\n");
	scanf("%d", &length);
	printf("Please enter a sequence:\n");
	scanf("%d", &seqInt);
	createMat(seq,seqInt);
	for (int i = 0; i < findIndex(seq)+1-length; i++){      //run on the array and stop before we find zeros and before the length will take you out of array
		if ((i+length<SEQ_SIZE)&&findP(seq,i,length))	{	//if we are in array and we have found paladrium
			newPal = 0;                                     //restart pladium value because we want the first paladrium from the left
			for (int j = 0; j < length; j++){               //this loop only translate the paladrium from array to int
				newPal += seq[i+j];
				newPal = newPal * 10;
			}
			found = true;									//if we found atleast one paldrium found is true
		}
	}
	if (!found)												//if we didnt find paladrium print this
		printf("There is no such palindrome.");
	else
		printf("The requested palindrome is: %d",newPal/10);		//if we found print that
}