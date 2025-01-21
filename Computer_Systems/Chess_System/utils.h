#ifndef _UTILS_H
#define _UTILS_H

#include <stdlib.h>
#include <stdbool.h>
#include "./map.h"


MapKeyElement intKeyCopy(MapKeyElement n);
MapDataElement intDataCopy(MapDataElement n);

void intKeyDestroy(MapKeyElement n);
void intDataDestroy(MapDataElement n);

int intKeysCompare(MapKeyElement n1, MapKeyElement n2);

/////////////////////////////////////

int* intCopy(int* n);

MapDataElement doubleDataCopy(MapDataElement n);

void intDestroy(int* n);

void doubleDataDestroy(MapDataElement n);

int intCompare(int* n1, int* n2);

// int intCopy(int n);

// void intDestroy(int n);

// int intCompare(int n1, int n2);

bool isValidId(int id);

bool isUpperLetter(char c);

bool isLowerLetter(char c);

bool isValidLocation(const char* location);

#endif //_UTILS_H