#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAXTEXT 32
#define NOUNLEN 6
#define VERBLEN 5
#define ADJLEN 6
int compare(char* small, char* big);
bool is_suffix_in_dict(char* str, char* dict[], int n);
bool read_sentence(char* noun_suffixes[], int noun_suffixes_len,
    char* verb_suffixes[], int verb_suffixes_len,
    char* adj_suffixes[], int adj_suffixes_len);
void print_stats(int num_of_nouns, int num_of_verbs, int num_of_adjs);


/* Additional functions of your choice */


void print_stats(int num_of_nouns, int num_of_verbs, int num_of_adjs)
{
    printf("The sentence had %d special nouns, %d special verbs and %d special adjectives.\n",
        num_of_nouns, num_of_verbs, num_of_adjs);
    return;
}
int compare(char* small, char* big) {
   // int smalllength = strlen(small);
    int count = 0;
    int i = 1;
    int bigcount = 1;
    while (bigcount != (int)strlen(big) + 1) {       //stop when the big word is the same size as the counter
        if ((big[strlen(big) - bigcount] >= 'A' && big[strlen(big) - bigcount] <= 'Z') || (big[strlen(big) - bigcount] >= 'a' && big[strlen(big) - bigcount] < 'z')) {
            if (big[strlen(big) - bigcount] == small[i - 1] || (int)big[strlen(big) - bigcount] + ('a' - 'A') == (int)small[i - 1]) {
              //  printf("if(big[%d]==small[%d])\n", strlen(big) - bigcount, i - 1);
              //  printf("%c==%c\n", big[strlen(big) - bigcount], small[i - 1]);
                count++;
            }
            else {
                if (big[strlen(big) - bigcount] >= 'a')         //return if not the same letter
                    return (int)(big[strlen(big) - bigcount] - small[i - 1]);
                else
                    return (int)(big[strlen(big) - bigcount] + ('a' - 'A') - small[i - 1]);        //takes care of when a is A
            }
            i++;
        }
        bigcount++;
        if ((int)strlen(small) == count)              //if the dictionary fits the word
            return 0;
    }
    return -1;
}
bool is_suffix_in_dict(char* str, char* dict[], int n) {
    int left = 0, right = n - 1, mid = (right + left) / 2;
    while (n != 0) {
      //  printf("%s\n", dict[mid]);
      //  printf("%d\n", compare(dict[mid], str));
        if (compare(dict[mid], str) == 0)            //if we found one that is the same last words return true
            return true;
        if (compare(dict[mid], str) > 0) {               //if its bigger go right
            left = mid;
            mid = (left + right) / 2;
        }
        else {                                            //else go left
            right = mid;
            mid = (left + right) / 2;
        }
        if (n == 1)
            break;
        n = n / 2;
    }
    if (compare(dict[mid], str) == 0 || compare(dict[right], str) == 0 || compare(dict[left], str) == 0)//we must check the left right and mid before we finish
        return true;
    return false;
}
bool read_sentence(char* noun_suffixes[], int noun_suffixes_len, char* verb_suffixes[], int verb_suffixes_len, char* adj_suffixes[], int adj_suffixes_len) {
    char* text = malloc(MAXTEXT * sizeof(char));
    int verb = 0, noun = 0, adj = 0;
    bool lastLoop = false;
    if (text == NULL)
        exit(0);
    while (scanf("%s", text) !=EOF)
    {
        if (text[strlen(text) - 1] == '.')                    //if you find . then this is last loop
            lastLoop = true;
        if (is_suffix_in_dict(text,noun_suffixes, noun_suffixes_len))
            noun++;
        if (is_suffix_in_dict(text,verb_suffixes, verb_suffixes_len))
            verb++;
        if (is_suffix_in_dict(text,adj_suffixes, adj_suffixes_len))
            adj++;
       // printf("%s", text);
        if (lastLoop) {                  //if lastloop then we must print and reset the varibles
            printf("The sentence had %d special nouns, %d special verbs and %d special adjectives.\n",noun,verb,adj);
            lastLoop = false;
            noun = 0;
            verb = 0;
            adj = 0;
        }
    }
    if (lastLoop)
        return false;
    return true;
}

int main() {
    char* noun_suffixes[] = { "msi", "re", "sci", "ssen", "tnem", "tsi" };
    char* verb_suffixes[] = { "de", "eta", "ezi", "gni", "yfi" };
    char* adj_suffixes[] = { "elba", "evi", "hsi", "la", "luf", "suo" };
    printf("Enter text to analyze:\n");
    //char* text = malloc(MAXTEXT * sizeof(char));
    //scanf("%s",text);
    //printf("%d",strlen(verb_suffixes));
    read_sentence(noun_suffixes, NOUNLEN,verb_suffixes, VERBLEN, adj_suffixes, ADJLEN);
    return 0;
}