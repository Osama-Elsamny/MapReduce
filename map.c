#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "string.h"
#include <stdbool.h>
#include "map.h"

/**
 * Funcition name: contains
 * Purpose: check if database contains the input key
 * Developer: Osama Elsamny
 * Input: Map and key
 * Output: true if map contains the passed in key
*/
bool contains(Map *map, char *key){
    Map *temp = NULL;
    temp = iterate(map, key);
    if(temp == NULL)
        return false;
    else
        return true;
}

int get(Map *map, char *key){
    Map *temp = NULL;
    temp = iterate(map, key);
    return temp->value;
}

bool put(Map *map, char *key, int value){
    if(contains(map, key)){
        Map *temp = NULL;
        temp = iterate(map, key);
        temp->value++;
        return false;
    }
    Map *current = map;
    while(current->next != NULL){
        current = current->next;
    }
    current->next = (Map*) malloc(sizeof(Map));
    current->next->value = value;
    current->next->key = (char*)malloc(sizeof(char));
    strcpy(current->next->key, key);
    current->next->next = NULL;
    return true;
}
/**
 * Funcition name: contains
 * Purpose: helper function for contains for iterating over the map
 * Developer: Osama Elsamny
 * Input: Map and key
 * Output: returns a pointer to map node
*/
Map* iterate(Map *map, char *key){
    for(Map *current = map; current != NULL; current = current->next){
        if(strcmp(current->key, key) == 0){
            return current;
        }
    }
    return NULL;
}

void printMap(Map *map, int size){
    Map *print = NULL;
    do{
        print = findHighestValue(map);
        printf("%s  %d\n", print->key, print->value);
        print->value = -1;
        size--;
    }while(size > 0);
}

Map* findHighestValue(Map *map){
    Map *current = map;
    Map *highestMap = map;
    while (current != NULL){
        if(highestMap->value < current->value){
            highestMap = current;
        }
        current = current->next;
    }
    return highestMap;
}
