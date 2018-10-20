#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <math.h>
#include "string.h"

#define THREAD_NUM 5
#define MAX_NUM_WORD 40
int size;
typedef struct Map{
    char* key;
    int value;
    struct Map* next;
}Map;
typedef struct{
    char** array;
    int threadNum;
    int startIndx;
    int endIndx;
    Map *mylist;
}Args;

/** 
 * Funcition name: input_reader
 * Purpose: Reads and tokenize a file and save it in an array
 * Developer: Osama Elsamny
 * Input: The name of the file and A pointer to the array
 * Output: N/A
*/
void input_reader(char* file, char** word){
    int i = 0;
    FILE *inputFile = NULL;
    inputFile = fopen(file,"r");
    if(inputFile == NULL){
        printf("Input file failed to open\n");
        exit(1);
    }
    while(!feof(inputFile)){
        word[i] = (char*)malloc(sizeof(char));
        fscanf(inputFile, "%s", word[i]);
        i++;
    }
    size = i;
    fclose(inputFile);
}

/** 
 * Funcition name:
 * Purpose: 
 * Developer:
 * Input: 
 * Output: 
*/
void print(char** word, int arrsize){
    for(int i = 0; i < arrsize; i++){
        printf("%s\n", word[i]);
    }
}

/** 
 * Funcition name: mapping
 * Purpose: Returns and key with with sepcifiecd key with
 *          hard coded value of 1
 * Developer: Osama Elsamny
 * Input: A pointer to the key which in this case a word
 * Output: Map pointer to the element
*/
Map* mapping(char* key){
    Map *temp = (Map*) malloc(sizeof(Map));
    temp->value = 1;
    temp->key = (char*)malloc(sizeof(char));
    strcpy(temp->key, key);
    temp->next = NULL;
    return temp;
}

/** 
 * Funcition name: reduce
 * Purpose: Takes a list with same key and returns the added 
 *          sum of all the similar keys
 * Developer: Osama Elsamny
 * Input: Take Pointer to the first element in the list
 * Output: return a pointer to the begining of the new 
 *         reduced list
*/
Map* reduce(Map* list){
    Map *temp = (Map*) malloc(sizeof(Map));
    int value = 0;
    for(Map *itr = list; itr != NULL; itr = itr->next){
        value++;
    }
    temp->value = value;
    temp->key = (char*)malloc(sizeof(char));
    strcpy(temp->key, list->key);
    temp->next = NULL;
    return temp;
}

/** 
 * Funcition name: output
 * Purpose: print out the give Map
 * Developer: Osama Elsamny
 * Input: Map 
 * Output: N/A
*/
void output(Map* wordToPrint){
    printf("%s  %d\n", wordToPrint->key, wordToPrint->value);
}

/** 
 * Funcition name: myCompare
 * Purpose: Used by the sorting function to compare Two strings
 * Developer: Osama Elsamny
 * Input: two strings
 * Output: returns an int holding the value of the comparison
*/
int myCompare (const void * a, const void * b){ 
    return strcmp (*(const char **) a, *(const char **) b); 
} 

/** 
 * Funcition name: Sort
 * Purpose: * Use to sort the array inorder for each thread
 *          * to take a part of the sorted array
 * Developer: Osama Elsamny
 * Input: takes a pointer to the arrray and size of the array
 * Output: N/A
*/
void sort(char** arr, int n){ 
    qsort (arr, n, sizeof (const char *), myCompare); 
}

/** 
 * Funcition name: ThreadMap
 * Purpose: To map and compuse a list of the word
 * Developer: Osama Elsamny
 * Input: structure called args, used to send different args to the function
 * Output: N/A
*/ 
void* ThreadMap(void* arg){
    Args *input = (Args *) arg;
    Map *itr = NULL;
    input->mylist = mapping(input->array[input->startIndx]);
    itr = input->mylist;
    for(int i = input->startIndx + 1; i < input->endIndx; i++){
        itr->next = mapping(input->array[i]);
        itr = itr->next;
    }
    return NULL;
}

/** 
 * Funcition name:
 * Purpose: 
 * Developer:
 * Input: 
 * Output: 
*/ 
void initializeThreadInput(Args **input, char** key,int n){
    for(int i = 0; i < THREAD_NUM; i++){
        input[i] = (Args*) malloc(sizeof(Args));
        input[i]->startIndx = floor( i * n/THREAD_NUM);
        input[i]->endIndx = floor((i+1) * n/THREAD_NUM);
        input[i]->threadNum = i;
        input[i]->array = key;
        input[i]->mylist = NULL;
    }
}

/** 
 * Funcition name: Main
 * Purpose: * Acts like the host in this context
 *          * Deals with all the interaction between
 *            the mapping, reducint, and ouputing functions
 * Developer: Osama Elsamny
 * Input: takes the file name from the user
 * Output: ouput the wordCount
*/
int main(int argc, char* argv[]){
    char* word[MAX_NUM_WORD];
    Args *theadsInput[THREAD_NUM];
    pthread_t threadMapping[THREAD_NUM];
    int localsize = 0;
    size = 0;

    input_reader(argv[1], word);
    localsize = size;
    sort(word, localsize);
    initializeThreadInput(theadsInput, word, localsize);
    for(int i = 0; i < THREAD_NUM; i++){
        pthread_create(&threadMapping[i], NULL, ThreadMap, (void *) theadsInput[i]);
    }
    for(int i = 0; i < THREAD_NUM; i++){
        pthread_join(threadMapping[i], NULL);
    }
    for(int i = 0; i < THREAD_NUM; i++){
        for(Map *itr = theadsInput[i]->mylist; itr != NULL; itr = itr->next){
            output(itr);
        }
    }
    //output(reduce(mylist));
    return 0;
}
/** 
 * Funcition name:
 * Purpose: 
 * Developer:
 * Input: 
 * Output: 
*/    