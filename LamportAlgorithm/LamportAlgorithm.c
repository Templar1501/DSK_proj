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
    printf("Thread no.%lu %s (%lu)\n", current_thread_index, prompt, time(NULL));
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
    bool    is_full;

    is_full = true;
    for (index = 0; index < lamport_barrier->lamport_mutex->thread_amount; index++) is_full &= lamport_barrier->queue[index];
    return is_full;
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

void lamport_exclusive(ClientThreadCommon *c_l_p) {
    LamportMutex        lamport_mutex;
    LamportBarrier      lamport_barrier;
   
    set_lamport_lock(&lamport_mutex, c_l_p);
    set_lamport_barrier(&lamport_barrier, &lamport_mutex ,c_l_p);
    print_prompt(lamport_mutex.current_thread_index, "stared work");

    while(global_work_flag) {       //tutaj zmienic
        print_prompt(lamport_mutex.current_thread_index, "in non critical section");

        lamport_lock(&lamport_mutex);
        print_prompt(lamport_mutex.current_thread_index, "entering critical section");
        sleep(1);
        print_prompt(lamport_mutex.current_thread_index, "exiting critical section");
        lamport_unlock(&lamport_mutex);

        print_prompt(lamport_mutex.current_thread_index, "Waiting on barrier");
        sleep(lamport_mutex.current_thread_index * 3);
        lamport_block(&lamport_barrier);
        print_prompt(lamport_mutex.current_thread_index, "Exiting barrier");
    }

    print_prompt(lamport_mutex.current_thread_index, "finished work");
}