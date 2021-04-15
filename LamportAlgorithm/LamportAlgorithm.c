#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <memory.h>
#include <stdlib.h>

#include "LamportAlgorithm.h"

long get_max_value(long *lp_vector, long thread_amount) {
    long index, max_val;

    max_val = lp_vector[0];
    for (index = 1; index < thread_amount; index++) 
        if (lp_vector[index] > max_val) max_val = lp_vector[index];
    
    return max_val + 1;
}

static inline bool is_next(long *lp_vector, long current_thread_index, long index) {
    return lp_vector[current_thread_index] < lp_vector[index] || (lp_vector[current_thread_index] == lp_vector[index] && current_thread_index < index);
}

void lamport_lock(LamportMutex *lamport_mutex) {
    long index;

    lamport_mutex->entrances[lamport_mutex->current_thread_index] = true;
    lamport_mutex->lp_vector[lamport_mutex->current_thread_index] = get_max_value(lamport_mutex->lp_vector, lamport_mutex->thread_amount);
    lamport_mutex->entrances[lamport_mutex->current_thread_index] = false;

    for (index = 0; index < lamport_mutex->thread_amount; index++) {
        if (index == lamport_mutex->current_thread_index) continue;

        while(lamport_mutex->entrances[index] != 0)/*BUSY WAIT*/;
        while(lamport_mutex->lp_vector[index] != 0 && is_next(lamport_mutex->lp_vector, index, lamport_mutex->current_thread_index))/*BUSY WAIT*/;
    }
}

static inline void lamport_unlock(LamportMutex *lamport_mutex) {
    lamport_mutex->lp_vector[lamport_mutex->current_thread_index] = 0;
}

static inline void print_prompt(long current_thread_index, const char *prompt) {
    printf("Client no.%02lu %s (%lu)\n", current_thread_index, prompt, time(NULL));
}

void set_lamport_lock(LamportMutex *lamport_mutex, ClientThreadCommon *c_l_p) {
    lamport_mutex->current_thread_index = c_l_p->thread_index;
    lamport_mutex->entrances = c_l_p->entrances;
    lamport_mutex->lp_vector = c_l_p->lp_vector;
    lamport_mutex->thread_amount = c_l_p->thread_amount;
}

void set_lamport_barrier(LamportBarrier *lamport_barrier, LamportMutex *lamport_mutex, ClientThreadCommon *c_l_p) { 
    lamport_barrier->lamport_mutex = lamport_mutex;
    lamport_barrier->queue = c_l_p->queue;
    lamport_barrier->flag = c_l_p->flag;
    lamport_barrier->local_sense = 0;
}

static inline void clear_queue(LamportBarrier *lamport_barrier) {
    long index;

    for (index = 0; index < lamport_barrier->lamport_mutex->thread_amount; index++) lamport_barrier->queue[index] = false;
}

static inline void set_position_queue(LamportBarrier *lamport_barrier) {
    lamport_barrier->queue[lamport_barrier->lamport_mutex->current_thread_index] = true;
}

static inline bool is_queue_full(LamportBarrier *lamport_barrier) {
    long    index;
    
    for (index = 0; index < lamport_barrier->lamport_mutex->thread_amount; index++) 
        if (!lamport_barrier->queue[index]) return false;

    return true;
}

void lamport_block(LamportBarrier *lamport_barrier) {
    lamport_barrier->local_sense = 1 - lamport_barrier->local_sense;
    lamport_lock(lamport_barrier->lamport_mutex);
    set_position_queue(lamport_barrier);

    if (is_queue_full(lamport_barrier)) {
        clear_queue(lamport_barrier);
        lamport_unlock(lamport_barrier->lamport_mutex);
        (*lamport_barrier->flag) = lamport_barrier->local_sense;
    } else {
        lamport_unlock(lamport_barrier->lamport_mutex);
        while (*(lamport_barrier->flag) != lamport_barrier->local_sense && global_work_flag)/*BUSY WAIT*/; 
    }
}

void first_answer_occurence(unsigned long answer, LamportMutex lamport_mutex) {
    solution_found_by_other_thread = true;
    add_blockchain_element(answer, lamport_mutex.current_thread_index);
    set_new_riddle();
    print_prompt(lamport_mutex.current_thread_index, "founded solution");
}

void next_answer_occurence(unsigned long answer, LamportMutex lamport_mutex, bool *riddle_solved) {
    *riddle_solved = false;
    vote_for_blockchain_element(lamport_mutex.thread_amount);
    print_prompt(lamport_mutex.current_thread_index, "founded solution, but not first");
}

void votting_action(LamportMutex lamport_mutex) {
    vote_for_blockchain_element(lamport_mutex.thread_amount);
    print_prompt(lamport_mutex.current_thread_index, "abandoned computing, votting");
}

void lamport_exclusive(ClientThreadCommon *c_l_p) {
    LamportMutex        lamport_mutex;
    LamportBarrier      lamport_barrier;
    bool                riddle_solved;
    unsigned long       answer;
   
    set_lamport_lock(&lamport_mutex, c_l_p);
    set_lamport_barrier(&lamport_barrier, &lamport_mutex ,c_l_p);
    print_prompt(lamport_mutex.current_thread_index, "stared work");

    while(global_work_flag) {       
        riddle_solved = try_bruteforce_riddle(&answer);

        lamport_lock(&lamport_mutex);
        if (riddle_solved && !solution_found_by_other_thread) first_answer_occurence(answer, lamport_mutex);
        else if (riddle_solved) next_answer_occurence(answer, lamport_mutex, &riddle_solved);
        else votting_action(lamport_mutex);
        lamport_unlock(&lamport_mutex);

        sleep(1);
        lamport_block(&lamport_barrier);
    }

    print_prompt(lamport_mutex.current_thread_index, "finished work");
}