#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <math.h>
#include "string.h"

#define THREAD_NUM 5
#define MAX_NUM_WORD 1024
#define MAX_STRING_SIZE 1024
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
typedef struct{
    int threadNum;
    int startIndx;
    int endIndx;
    Map *myOutput;
    Map **mylist;
}ArgsReduce;

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
        word[i] = (char*)malloc(MAX_STRING_SIZE * sizeof(char));
        fscanf(inputFile, "%s", word[i]);
        i++;
    }
    size = i;
    fclose(inputFile);
}

/**
 * Funcition name: mapping
 * Purpose: Map.value is mapped to 1 for the keys
            which has the same as input key
 * Developer: Osama Elsamny
 * Input: A pointer to the key which in this case a word
 * Output: Map pointer to the element
*/
Map* mapping(char* key){
    Map *temp = (Map*) malloc(sizeof(Map));
    temp->value = 1;
    temp->key = (char*)malloc(MAX_STRING_SIZE * sizeof(char));
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
    temp->key = (char*)malloc(MAX_STRING_SIZE * sizeof(char));
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
    for(int i = (input->startIndx) + 1; i < input->endIndx; i++){
        itr->next = mapping(input->array[i]);
        itr = itr->next;
    }
    return NULL;
}

/**
 * Funcition name: initializeThreadMappingInput
 * Purpose: To initialize multiple inputs and outputs for
 *          the thread function
 * Developers: Osama Elsamny and Jasmandeep Batra
 * Input: to be completed -------------------------
 * Output: N/A
*/
void initializeThreadReduceInput(ArgsReduce **input2, Map **list, int n){
    for(int i = 0; i < THREAD_NUM; i++){
        input2[i] = (ArgsReduce*) malloc(sizeof(ArgsReduce));
        input2[i]->threadNum = i;
        input2[i]->startIndx = floor( i * n/THREAD_NUM);
        input2[i]->endIndx = floor((i+1) * n/THREAD_NUM);
        input2[i]->mylist = list;
        input2[i]->myOutput = NULL;
    }
}

/**
 * Funcition name: initializeThreadMappingInput
 * Purpose: To initialize multiple inputs and outputs for
 *          the thread function
 * Developers: Osama Elsamny and Jasmandeep Batra
 * Input: the whole array, the size of the array, pointer to
 *        the arguments array
 * Output: N/A
*/
void initializeThreadMappingInput(Args **input, char** key,int n){
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
 * Funcition name: combineList
 * Purpose: To combine the list after is done mapping
 * Developer: Osama Elsamny
 * Input: The argument array that had the output saved
 *        after the threads are done with the computations
 * Output: Map pointer to the list
*/
Map* combineList(Args **input){
    Map *temp = NULL;
    temp = input[0]->mylist;
    for(int i = 1; i < THREAD_NUM; i++){
        Map *itr = NULL;
        itr = input[i-1]->mylist;
        while(itr->next != NULL){
            itr = itr->next;
        }
        itr->next = input[i]->mylist;
    }
    return temp;
}
/**
 * Funcition name: getsize
 * Purpose: To find the number of different words in the list
 * Developer: Osama Elsamny
 * Input: the list to find
 * Output: A integer with number of different words in the list
*/
int getsize(Map *list){
    int n = 1;
    for(Map *itr = list; itr->next != NULL; itr = itr->next){
        if(strcmp(itr->key, itr->next->key) != 0){
            n++;
        }
    }
    return n;
}

/**
 * Funcition name: findSimilarList
 * Purpose: To seperate each key list in a different
 *          into a different list
 * Developer: Osama Elsamny
 * Input: The whole maped list
 * Output: A pointer to each pointer of different list
*/
void findSimilarList(Map **temp, Map *list, int n){
    for(int i = 0; i < n; i++){
        temp[i] = (Map*) malloc(sizeof(Map));
    }
    Map *itr = list;
    Map *itr2 = list;
    int i = 1;
    temp[0] = list;
    while(itr2 != NULL){
        if(itr->next == NULL){
            temp[i] = NULL;
            break;
        }
        while(strcmp(itr->key, itr->next->key) == 0){
            itr = itr->next;
            itr2 = itr2->next;
            if(itr->next == NULL)
                break;
        }
        if(itr->next == NULL){
            temp[i] = NULL;
            break;
        }
        itr2 = itr2->next;
        itr->next = NULL;
        itr = itr2;
        temp[i] = itr;
        i++;
    }
}

/**
 * Funcition name: ThreadReduce
 * Purpose: To manage multiple threads calling the reduce
 *          function
 * Developer: Osama Elsamny
 * Input: A sturcture with differnt inputs please check
 *        the def above
 * Output: N/A
*/
void* ThreadReduce(void* arg){
    ArgsReduce *input = (ArgsReduce *) arg;
    Map *temp = NULL;
    for(int i = input->startIndx; i < input->endIndx; i++){
        if(input->myOutput == NULL){
            input->myOutput = reduce(input->mylist[i]);
            temp = input->myOutput;
            input->threadNum = input->threadNum + 1;
            continue;
        }
        temp->next = reduce(input->mylist[i]);
        temp = temp->next;
        input->threadNum = input->threadNum + 1;
    }
    return NULL;
}

void print(ArgsReduce **input){
    for(int i = 0; i < THREAD_NUM; i++){
        Map *temp = input[i]->myOutput;
        while(temp != NULL){
            output(temp);
            temp = temp->next;
        }
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
    char *word[MAX_NUM_WORD];
    Args *theadsMappingInput[THREAD_NUM];
    ArgsReduce *theadsReduceInput[THREAD_NUM];
    pthread_t threadMapping[THREAD_NUM];
    pthread_t threadReducing[THREAD_NUM];
    int localsize = 0;
    Map *combinedList = NULL;
    int wordNum = 0;
    size = 0;

    input_reader(argv[1], word);
    localsize = size;
    sort(word, localsize);
    initializeThreadMappingInput(theadsMappingInput, word, localsize);
    for(int i = 0; i < THREAD_NUM; i++){
        pthread_create(&threadMapping[i], NULL, ThreadMap, (void *) theadsMappingInput[i]);
    }
    for(int i = 0; i < THREAD_NUM; i++){
        pthread_join(threadMapping[i], NULL);
    }
    combinedList = combineList(theadsMappingInput);
    wordNum = getsize(combinedList);
    Map *similarList[wordNum];
    findSimilarList(similarList, combinedList, wordNum);
    initializeThreadReduceInput(theadsReduceInput, similarList,wordNum);
    for(int i = 0; i < THREAD_NUM; i++){
        pthread_create(&threadReducing[i], NULL, ThreadReduce, (void *) theadsReduceInput[i]);
    }
    for(int i = 0; i < THREAD_NUM; i++){
        pthread_join(threadReducing[i], NULL);
    }
    print(theadsReduceInput);

    return 0;
}
/**
 * Funcition name:
 * Purpose:
 * Developer:
 * Input:
 * Output:
*/
