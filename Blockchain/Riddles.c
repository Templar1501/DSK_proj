#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Blockchain.h"
#include "Riddles.h"

bool bruteforce_riddle(unsigned long riddle,  unsigned long *answer){
    unsigned long tmp_answer;

    srand(time(NULL));
    tmp_answer = rand() % 1000;
    answer -= (tmp_answer % 10);

    while((tmp_answer * riddle) % RIDDLE_BODY != 1 && !solution_found_by_other_thread)
        tmp_answer += 10;

    if (solution_found_by_other_thread) return false;

    *answer = tmp_answer;
    return true;
}

bool check_if_proper_answer(unsigned long answer){
    return answer % 10 == 0;
}

void fill_block_struct(blockchain_element * elem, unsigned long answer_by_thread, unsigned long riddle, unsigned long client_id){
    elem->answer = answer_by_thread;
    elem->riddle = riddle;
    elem->client_id = client_id;
    elem->approved = false;
    elem->clients_votted = 0;
}