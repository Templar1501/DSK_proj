#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Riddles.h"


bool try_bruteforce_riddle(unsigned long *answer){
    unsigned long tmp_answer;

    srand(time(NULL));
    tmp_answer = rand() % 1000;
    tmp_answer -= (tmp_answer % 10);
    
    while((tmp_answer * riddle) % RIDDLE_BODY != 1 && !solution_found_by_other_thread) tmp_answer += 10;
    if (solution_found_by_other_thread) return false;

    *answer = tmp_answer;
    return true;
}

bool check_if_proper_answer(unsigned long answer){
    return answer % 10 == 0;
}

void fill_block_struct(blockchain_element *blockchain_node, unsigned long answer_by_thread, unsigned long riddle, unsigned long client_id){
    blockchain_node->answer = answer_by_thread;
    blockchain_node->riddle = riddle;
    blockchain_node->client_id = client_id;
    blockchain_node->approved = false;
    blockchain_node->clients_votted = 0;
    blockchain_node->next = NULL;
}
