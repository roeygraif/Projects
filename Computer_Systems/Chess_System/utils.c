#include <stdlib.h>
#include <stdbool.h>
#include "./utils.h"


#define NULL_TERMINATOR '\0'
#define SPACE ' '


MapKeyElement intKeyCopy(MapKeyElement n) {
    if (!n) {
        return NULL;
    }
    int *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(int *) n;
    return copy;
}
MapDataElement intDataCopy(MapDataElement n) {
    if (!n) {
        return NULL;
    }
    int *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(int *) n;
    return copy;
}

MapDataElement doubleDataCopy(MapDataElement n) {
    if (!n) {
        return NULL;
    }
    double *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(double *) n;
    return copy;
}

void intKeyDestroy(MapKeyElement n) {
    free(n);
}
void intDataDestroy(MapDataElement n) {
    free(n);
}

void doubleDataDestroy(MapDataElement n) {
    free(n);
}

int intKeysCompare(MapKeyElement n1, MapKeyElement n2) {
    return (*(int *) n1 - *(int *) n2);
}

//////////////////////////////////////////////////////////////

int* intCopy(int* n){
    int *n2 = malloc(sizeof(int));
    *n2 = (*n);
     return n2;
}

void intDestroy(int *n){
    free(n);
}

int intCompare(int *n1, int *n2){
    return *n1 - *n2;
}

// int intCopy(int n){
//     return n;
// }

// void intDestroy(int n){
//     return;
// }

// int intCompare(int n1, int n2){
//     return n1 - n2;
// }

bool isValidId(int id){
    return id > 0;
}

bool isUpperLetter(char c){
    return c >= 'A' && c <= 'Z';
}

bool isLowerLetter(char c){
    return c >= 'a' && c <= 'z';
}

bool isValidLocation(const char* location){
    if(location[0] == NULL_TERMINATOR || !isUpperLetter(location[0])){
        return false;
    }
    
    for(int i = 1; location[i] != NULL_TERMINATOR; i++){
        if((!isLowerLetter(location[i])) && location[i] != SPACE){
            return false;
        }
    }

    return true;
}

