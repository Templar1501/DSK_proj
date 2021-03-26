#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include "LamportAlgorithm.h"

long get_max_value(long *number, long amount) {
    long index, max_val;

    max_val = number[0];
    for (index = 1; index < amount; index++) 
        if (number[index] > max_val) max_val = number[index];
    
    return max_val + 1;
}

static inline bool is_next(long *number, long th_index, long index) {
    return number[th_index] < number[index] || (number[th_index] == number[index] && th_index < index);
}

void set_variables(long *th_index, long *thread_amount, long *numbers, bool *entrances, ClientThreadProperties *c_l_p) { //fixme: cause errors
    *th_index = c_l_p->client_thread_common.thread_index;
    *thread_amount = c_l_p->client_thread_common.vector_size;
    numbers = c_l_p->client_thread_common.numbers;
    entrances = c_l_p->client_thread_common.entrances;
}

void lamport_lock(long th_index, long thread_amount, bool *entrances, long *numbers) {
    long index;

    entrances[th_index] = true;
    numbers[th_index] = get_max_value(numbers, thread_amount);
    entrances[th_index] = false;

    for (index = 0; index < thread_amount; index++) {
        if (index == th_index) continue;

        while(entrances[index] != 0)/*BUSY WAIT*/;
        while(numbers[index] != 0 && is_next(numbers, index, th_index))/*BUSY WAIT*/;
    }
}

void lamport_unlock(long th_index, long *numbers) {
    numbers[th_index] = 0;
}

int test = 0;

static inline void print_prompt(long th_index,const char *prompt) {
    printf("Thread no.%lu %s (%lu)\n", th_index, prompt,time(NULL));
}

void lamport_exclusive(ClientThreadProperties *c_l_p) {
    long        index, th_index, *numbers, thread_amount;
    bool        *entrances;

    //set_variables(&th_index, &thread_amount, numbers,entrances, c_l_p);
    th_index = c_l_p->client_thread_common.thread_index;
    numbers = c_l_p->client_thread_common.numbers;
    entrances = c_l_p->client_thread_common.entrances;
    thread_amount = c_l_p->client_thread_common.vector_size;
    print_prompt(th_index, "stared work");

    while(global_work_flag) {
        print_prompt(th_index, "in non critical section");
        lamport_lock(th_index, thread_amount, entrances, numbers);
        print_prompt(th_index, "entering critical section");
        sleep(1);
        print_prompt(th_index, "exiting critical section");
        lamport_unlock(th_index, numbers);
    }

    print_prompt(th_index, "finished work");
}