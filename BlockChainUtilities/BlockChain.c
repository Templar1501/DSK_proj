#include <stdio.h>
#include <stdlib.h>

#include "Riddles.h"
#include "BlockChain.h"

void vote_for_blockchain_element(long thread_amount) {
    blockchain_element *ptr;

    for(ptr = init_block; ptr->next != NULL;) ptr = ptr->next;
    
    if (check_if_proper_answer(ptr->answer)) ptr->clients_votted++;
    
    if (ptr->clients_votted == thread_amount - 1) {
        ptr->approved = true;
        solution_found_by_other_thread = false;
    }
}

void add_blockchain_element(unsigned long answer, unsigned long client_id){
    blockchain_element *tmp, *ptr;

    tmp = malloc(sizeof(blockchain_element));
    fill_block_struct(tmp, answer,riddle, client_id);

    if(init_block == NULL) init_block = tmp;
    else{
        for(ptr = init_block; ptr->next != NULL;) ptr=ptr->next;
        ptr->next = tmp;
    }
}

void delete_blockchain_list(){
    blockchain_element *tmp;
    
    for(;init_block != NULL; init_block = init_block->next){
        tmp = init_block;
        free(tmp);
    }
}

void print_blockchain_list() {
    blockchain_element *current;

    printf("BlockChain view:\n");
    for (current = init_block; current != NULL; current = current->next) 
        printf("{riddle=%5ld, answer=%5ld, approved=%d, client_guessed_id=%ld}\n", current->riddle, current->answer, current->approved, current->client_id);
}

void set_new_riddle(){
    unsigned long unused;

    fscanf(riddle_source, "%lu, %lu", &riddle, &unused);
}

void finish_riddles(){
    fclose(riddle_source);
}

void init_riddles(){
    riddle_source = fopen("/home/qqa/Projects/C/DSK_proj/BlockChainUtilities/1000.txt", "r");
    set_new_riddle();
}
