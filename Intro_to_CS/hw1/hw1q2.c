#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	char letter = '0';
	char text = '0';
	int counter = 0;
	printf("Enter letter to count:\n");
	scanf("%c", &letter);                         //scan letter to count
	if ((letter >= 'A' && letter <= 'Z') || (letter >= 'a' && letter <= 'z')) {    //if it is a valid letter do loop
		printf("Enter the char sequence:\n");
		while (text != '#')                                                        //finish while when #
		{
			scanf("%c", &text);                                                    //scan letters from input
			if (text >= 'A' && text <= 'Z')                                        //if letter between A and Z
			{
				if (letter == text || letter == text + 32) {                       //if letter is like text (A or a)counter++
					counter++;
				}

			}
			if (text >= 'a' && text <= 'z')                                        //if letter between a and z
			{
				if (letter == text || letter == text - 32) {                        //if letter is like text (a or A)counter++
					counter++;
				}

			}
		}
		printf("The letter appeared %d times", counter);
	}
	return 0;
}