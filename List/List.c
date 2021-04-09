#include "List.h"
#include <stdio.h>
#include <stdlib.h>

FILE *open_file(){
    const char filename[] = "1000.txt";
    FILE *file = fopen(filename, "r");

    return file;
}

void close_file(FILE * file){
    fclose(file);
}

void add_element(unsigned long riddle, unsigned long answer){
    node_t * temp;
    node_t * ptr;
    temp = (node_t *) malloc(sizeof(node_t));

    temp->riddle = riddle;
    temp->answer = answer;
    temp->next = NULL;

    if(head == NULL){
        head = temp;
    }else{
        ptr = head;
        while(ptr->next!=NULL){
            ptr=ptr->next;
        }
        ptr->next = temp;
    }
}

void delete_list(){
    node_t * tmp;
    tmp = (node_t *) malloc(sizeof(node_t));
    while(head != NULL){
        tmp = head;
        head = head->next;
        free(tmp);
    }
    printf("Deleted\n");
}

void print_list() {
    node_t * current = head;

    while (current != NULL) {
        printf("riddle = %ld, answer = %ld\n", current->riddle, current->answer);
        current = current->next;
    }
}

unsigned long get_answer(int i){
    unsigned long val;
    node_t * ptr;

    ptr=head;

    for(int j=1;j<i;j++){
        ptr=ptr->next;
    }

    val = ptr->answer % 10;
    
    return val;
}

unsigned long read_from_file(FILE * file){//}, unsigned int * counter){
    unsigned long riddle, answer;

    fscanf(file, "%lu, %lu", &riddle,&answer);
    return riddle;
    //add_element(riddle,answer);

    //(*counter)++;
}