#ifndef MAP_H
# define MAP_H
#include <stdlib.h>
#include <stdio.h>

typedef struct Map{
    char* key;
    int value;
    struct Map* next;
}Map;

bool contains(Map *map, char *key);
int get(Map *map, char *key);
bool put(Map *map, char *key, int value);
Map* iterate(Map *map, char *key);
void printMap(Map *map, int size);
Map* findHighestValue(Map *map);
#endif