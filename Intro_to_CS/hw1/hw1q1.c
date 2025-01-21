#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	printf("Welcome, please enter the weights of each ingredient:\n");
	int counter=-1;
	double sum_of_ingredience = 0;
	double ingredience = 0;
	//scanf("%ld", &ingredience);
	while (ingredience >= 0)          //if not negative keep taking input
	{
		sum_of_ingredience += ingredience;        //add to sum
		counter++;                                   //count how many ingediance
		scanf("%lf", &ingredience);                   //scan ingrediance
	}
	double avg_weight = sum_of_ingredience / (double)counter;      //average weight in double
	int pure = counter * 10;                                       //calculating purity
	if (counter > 2 && counter < 11)                               //only 3 to 10 ingredience
	{
		printf("The final product weighs %.3lf pounds and is %d percent pure.", avg_weight, pure);     //print 3 numbers after decimal point
	}
	if (counter<3)                          //if not enough ingredience
	{
		printf("Not enough ingredients.");
	}
	if (counter > 10)                       //if too many ingrediance
	{
		printf("Too many ingredients.");
	}
	return 0;
}