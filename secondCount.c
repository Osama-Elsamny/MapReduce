#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include "string.h"

#define MAX_NUM_WORD 24
char* word[MAX_NUM_WORD];
int size;
typedef struct Map{
    char* key;
    int value;
    struct Map* next;
}Map;

void input_reader(char* file){
    int i = 0;
    FILE *inputFile = NULL;
    inputFile = fopen(file,"r");
    if(inputFile == NULL){
        printf("Input file failed to open");
        exit(1);
    }
    while(!feof(inputFile)){
        word[i] = (char*)malloc(sizeof(char));
        fscanf(inputFile, "%s", word[i]);
        i++;
    }
    size = i;
}
void print(){
    for(int i = 0; i < size; i++){
        printf("%s\n", word[i]);
    }
}
Map* mapping(char* key){
    Map *temp = (Map*) malloc(sizeof(Map));
    temp->value = 1;
    temp->key = (char*)malloc(sizeof(char));
    strcpy(temp->key, key);
    temp->next = NULL;
    return temp;
}
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
void output(Map* word){
    printf("%s  %d\n", word->key, word ->value);
}
int main(int argc, char* argv[]){
    Map *mylist = (Map*) malloc(sizeof(Map));
    Map *itr = NULL;
    size = 0;
    input_reader(argv[1]);
    //print();
    mylist = mapping(word[0]);
    itr = mylist;
    for(int i = 1; i < size; i++){
        itr->next = mapping(word[i]);
        itr = itr->next;
    }
    output(reduce(mylist));
    return 0;
}