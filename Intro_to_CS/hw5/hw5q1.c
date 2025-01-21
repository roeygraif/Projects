#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define LEX 1
#define LET_DIVER 2
#define LETTERSNUM 26
#define DIFF 32
#define TWENTYONE 21
int compareStr(char* str1, char* str2, int rule);
int lex(char* str1, char* str2);
int checkLetter(char c1, char c2);
bool isabc(char c);
bool isBig(char c);
void swap(char* str_arr[], int index, int max);
void sortString(char* str_arr[], int n, int rule);
//int getIndex(char* str_arr[], int n, int rule);
void printNumStringsInputMessage();
void printStringsInputMessage(int n);
void printRuleOfComparisonInputMessage();
void printSortedStrings(char* str_arr[], int n);
void printAllocationError();
int letDiver(char* str1, char* str2);

void sortString(char* str_arr[], int n, int rule) {
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n-i-1; j++)
		{
			if (compareStr(str_arr[j], str_arr[j + 1], rule) > 0)  //bubble sorting
				swap(str_arr, j, j + 1);
		}
	}
}
void swap(char* str_arr[], int index, int max) {//swap function for bubble sort
	char helper[TWENTYONE+1];
	strcpy(helper, str_arr[max]);           
	strcpy(str_arr[max], str_arr[index]);
	strcpy(str_arr[index], helper);
} 

int compareStr(char* str1,char* str2, int rule) {
	if (rule == LEX)
		return lex(str1,str2);
	if (rule == LET_DIVER)
		return letDiver(str1, str2);
	return 0;
}
int letDiver(char* str1, char* str2) {
	int histogram[LETTERSNUM] = {0};
	int histogram2[LETTERSNUM] = { 0 };
	for (int i = 0; i < (int)strlen(str1); i++)//run on firtst histogram
	{
		if (isabc(str1[i])) {
			if (isBig(str1[i]))
				histogram[(int)str1[i] + DIFF- (int)'a'] = 1;  //this is for caps lock
			else 
				histogram[(int)str1[i]- (int)'a'] = 1;         //this is for small letters
		}
	}
	for (int i = 0; i < (int)strlen(str2); i++)    //same thing for second histogram
	{
		if (isabc(str2[i])) {
			if (isBig(str2[i]))
				histogram2[(int)str2[i] + DIFF - (int)'a'] = 1;
			else 
				histogram2[(int)str2[i] - (int)'a'] = 1;
		}
	}
	int count1 = 0;
	int count2 = 0;
	for (int i = 0; i < LETTERSNUM; i++)//count the histogram
	{
		if (histogram[i] == 1)
			count1++;
		if (histogram2[i] == 1)
			count2++;
	}
	return count1 - count2;
}
int lex(char* str1, char* str2) {
	int run = 0;
	if (strlen(str1) < strlen(str2))//find the min length of both strings
		run = strlen(str1);
	else
		run = strlen(str2);
	for (int i = 0; i < run; i++)
	{
		if (checkLetter(str1[i], str2[i]) > 0) //if str1 small
			return -1;
		if (checkLetter(str1[i], str2[i]) < 0)//if str1 big
			return 1;
	}
	if (strlen(str1) == strlen(str2))//if they are the same
		return 0;
	if (strlen(str1) > strlen(str2))//if they are the same but one is longer than the other
		return 1;
	return -1;

}
int checkLetter(char c1, char c2) {
	if (isabc(c1)&&isabc(c2)) {
		if ((isBig(c1) && isBig(c2))|| (!isBig(c1) && !isBig(c2)))//if they are both big or small letters
			return c2 - c1;
		if (isBig(c1) && !isBig(c2))                              //if one is big letters and one is small letters
			return c2 - c1 - ('a' - 'A');
		if (!isBig(c1) && isBig(c2))                              //if one is small letters and one is big letters
			return c2 + ('a' - 'A') - c1 ;
	}
	return c2 - c1;//anything else return the diffrence
}
bool isabc(char c) {//check if the are american alphabet
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return true;
	return false;
}
bool isBig(char c) {//check if they are big letters
	if (c >= 'A' && c <= 'Z')
		return true;
	return false;
}


void printNumStringsInputMessage() {
	printf("Please enter the number of strings:\n");
}
void printStringsInputMessage(int n) {
	printf("Please enter the %d strings:\n", n);
}

void printRuleOfComparisonInputMessage() {
	printf("Please enter the rule of comparison between strings.\n");
	printf("%d: Lexicographic order.\n", LEX);
	printf("%d: By the diversity of letters.\n", LET_DIVER);
}

void printSortedStrings(char* str_arr[], int n) {
	printf("The sorted strings are:\n");
	for (int i = 0; i < n; i++) {
		printf("%s\n", str_arr[i]);
	}
}

void printAllocationError() {
	printf("ERROR: allocation failed.\n");
}



int main() {
	int num = 0;
	printNumStringsInputMessage();
	scanf("%d",&num);
	printStringsInputMessage(num);
	char** arrstr = malloc((num)*sizeof(char*));
	if (arrstr == NULL) {
		printAllocationError();
		exit(0);
	}
	for (int i = 0; i < num; i++) {
		arrstr[i] = malloc((TWENTYONE +1)* sizeof(char) );
		if (arrstr[i] == NULL) {
			printAllocationError();
			exit(0);
		}
	}
	for (int i = 0; i < num; i++)
	{		
		scanf("%s",arrstr[i]);
	}
	printRuleOfComparisonInputMessage();
	int rule = 0;
	scanf("%d",&rule);
	sortString(arrstr, num, rule);
	printSortedStrings(arrstr, num);
	for (int i = 0; i < num; i++)
	{
		free(arrstr[i]);
	}
	free(arrstr);
	return 0;
}