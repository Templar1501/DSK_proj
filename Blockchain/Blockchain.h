#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


blockchain_element * head; //TODO: set to null 
unsigned long ans;

typedef struct node {
    unsigned long   riddle;
    unsigned long   answer;
    unsigned long   client_id;
    unsigned long   clients_votted;
    bool            approved;
    struct node     *next;
} blockchain_element;

FILE *open_file();
void close_file(FILE * file);
void add_element(unsigned long riddle, unsigned long answer);
void delete_list();
void print_list();
unsigned long get_answer(int i);
unsigned long read_from_file(FILE * file);

#endif