#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include "string.h"
#include "map.h"

/**
 * Funcition name: main()
 * Purpose: *
 * Developer: Osama Elsamny
 * Input: N/A
 * Output: N/A
*/
int main(int argc, char* argv[]){
    FILE *inputFile = NULL;
    Map *myMap = (Map*) malloc(sizeof(Map));
    char *word = (char*)malloc(sizeof(char));
    inputFile = fopen(argv[1],"r");
    int size = 0;
    if(inputFile == NULL){
        printf("Input file failed to open");
        exit(1);
    }
    fscanf(inputFile, "%s", word);
    myMap->key = (char*)malloc(sizeof(char));
    strcpy(myMap->key, word);
    myMap->value = 1;
    myMap->next = NULL;
    size++;
    while(!feof(inputFile)){
        fscanf(inputFile, "%s", word);
        if(put(myMap, word, 1))
            size++;
    }
    fclose(inputFile);
    printMap(myMap, size);
    return 0;
}

//Templet for the functions
/** 
 * Funcition name:
 * Purpose: 
 * Developer:
 * Input: 
 * Output: 
*/