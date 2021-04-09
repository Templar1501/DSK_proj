#include "Blockchain.h"
#define RIDDLE_BODY 9697

bool solution_found_by_other_thread;


bool bruteforce_riddle(unsigned long riddle, unsigned long *answer);
bool check_if_proper_answer(unsigned long answer);
void fill_block_struct(blockchain_element * elem, unsigned long answer_by_thread, unsigned long riddle, unsigned long client_id);