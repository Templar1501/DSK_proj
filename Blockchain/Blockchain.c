#include "Blockchain.h"
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
    blockchain_element *temp, *ptr;

    temp = (blockchain_element *) malloc(sizeof(blockchain_element));

    temp->riddle = riddle;
    temp->answer = answer;
    temp->next = NULL;

    if(head == NULL){
        head = temp;
    }else{
        for(ptr = head;ptr!=NULL;)ptr = ptr->next;
        ptr->next = temp;
    }
}

void delete_list(){
    blockchain_element * tmp;
    
    // while(head != NULL){
    //     tmp = head;
    //     head = head->next;
    //     free(tmp);
    // }

    for(;head!=NULL;head=head->next){
        tmp = head;
        free(tmp);
    }
    printf("Deleted\n");
}

void print_list() {
    blockchain_element *current;

    // while (current != NULL) {
    //     printf("riddle = %ld, answer = %ld\n", current->riddle, current->answer);
    //     current = current->next;
    // }

    for (current = head; current != NULL; current = current->next){
        printf("riddle = %ld, answer = %ld\n", current->riddle, current->answer);
    }
}

unsigned long get_answer(int i){
    unsigned long val;
    blockchain_element * ptr;

    ptr=head;

    for(int j=1;j<i;j++){
        ptr=ptr->next;
    }

    val = ptr->answer % 10;
    
    return val;
}

unsigned long read_from_file(FILE * file){
    unsigned long riddle, answer;

    fscanf(file, "%lu, %lu", &riddle,&answer);
    return riddle;
}