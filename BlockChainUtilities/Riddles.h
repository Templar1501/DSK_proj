#ifndef RIDDLES_H
#define RIDDLES_H

#include "BlockChain.h"
#define RIDDLE_BODY 9697

bool try_bruteforce_riddle(unsigned long *answer);
bool check_if_proper_answer(unsigned long answer);
void fill_block_struct(blockchain_element * blockchain_node, unsigned long answer_by_thread, unsigned long riddle, unsigned long client_id);

#endif