#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include "List.h"

bool solution_found_by_other_thread;

unsigned long bruteforce_riddle(unsigned long riddle, unsigned long body_riddle){
    unsigned long tmp_answer;
    srand(time(NULL));
    tmp_answer = rand();

    while((tmp_answer * riddle) % body_riddle != 1 && !solution_found_by_other_thread){
        tmp_answer++;
    }

    return tmp_answer;
}

bool check_if_proper_answer(unsigned long answer){
    return answer % 10 == 0;
}

void fill_block_struct(node_t * node, unsigned long answer_by_thread, unsigned long riddle, unsigned long client_id){
    node->answer = answer_by_thread;
    node->riddle = riddle;
    node->client_id = client_id;
}