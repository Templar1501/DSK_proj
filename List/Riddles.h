#include "List.h"
#define RIDDLE_BODY 9697

bool solution_found_by_other_thread;


unsigned long bruteforce_riddle(unsigned long riddle, unsigned long *answer);
bool check_if_proper_answer(unsigned long answer);
void fill_block_struct(node_t * node, unsigned long answer_by_thread, unsigned long riddle, unsigned long client_id);