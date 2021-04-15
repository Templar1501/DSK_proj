#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdbool.h>
#include <stdio.h>

typedef struct blockchain_node {
    bool                        approved;
    unsigned long               riddle;
    unsigned long               answer;
    unsigned long               client_id;
    unsigned long               clients_votted;
    struct blockchain_node     *next;
} blockchain_element;

blockchain_element  *init_block; 
unsigned long       riddle;
FILE                *riddle_source;
bool                solution_found_by_other_thread;

void init_riddles();
void finish_riddles();
void add_blockchain_element(unsigned long answer, unsigned long client_id);
void delete_blockchain_list();
void print_blockchain_list();
void set_new_riddle();
void vote_for_blockchain_element(long thread_amount);

#endif