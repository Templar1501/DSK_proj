#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
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

void lamport_lock(long current_thread_index, long thread_amount, bool *entrances, long *lp_vector) {
    long index;

    entrances[current_thread_index] = true;
    lp_vector[current_thread_index] = get_max_value(lp_vector, thread_amount);
    entrances[current_thread_index] = false;

    for (index = 0; index < thread_amount; index++) {
        if (index == current_thread_index) continue;

        while(entrances[index] != 0)/*BUSY WAIT*/;
        while(lp_vector[index] != 0 && is_next(lp_vector, index, current_thread_index))/*BUSY WAIT*/;
    }
}

static inline void lamport_unlock(long current_thread_index, long *lp_vector) {
    lp_vector[current_thread_index] = 0;
}

static inline void print_prompt(long current_thread_index, const char *prompt) {
    printf("Thread no.%lu %s (%lu)\n", current_thread_index, prompt,time(NULL));
}

void set_variables(long *current_thread_index, long *thread_amount, long **lp_vector, bool **entrances, ClientThreadCommon *c_l_p) { 
    *current_thread_index = c_l_p->thread_index;
    *thread_amount = c_l_p->thread_amount;
    *lp_vector = c_l_p->lp_vector;
    *entrances = c_l_p->entrances;
}

void lamport_exclusive(ClientThreadCommon *c_l_p) {
    long        current_thread_index, thread_amount, *lp_vector;
    bool        *entrances;

    set_variables(&current_thread_index, &thread_amount, &lp_vector, &entrances, c_l_p);
    print_prompt(current_thread_index, "stared work");

    while(global_work_flag) {
        print_prompt(current_thread_index, "in non critical section");
        lamport_lock(current_thread_index, thread_amount, entrances, lp_vector);
        print_prompt(current_thread_index, "entering critical section");

        //Insert bate's algorithm
        sleep(1);

        print_prompt(current_thread_index, "exiting critical section");
        lamport_unlock(current_thread_index, lp_vector);
    }

    print_prompt(current_thread_index, "finished work");
}