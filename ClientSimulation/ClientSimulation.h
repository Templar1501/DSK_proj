#ifndef CLIENTSIMULATION_H
#define CLIENTSIMULATION_H

#include <stdbool.h>
#include <semaphore.h>
#include "../BlockChainUtilities/BlockChain.h"

typedef struct ClientSimulationCommon {
    long    thread_amount, *lp_vector, thread_index; //cash_earned, ridles_solved;
    bool    *entrances, *queue;
    char    *flag;
} ClientThreadCommon;

typedef struct ClientSimulation {
    ClientThreadCommon  client_thread_common;
    pthread_barrier_t   pass_data_barrier;
    sem_t               pass_index_sem;
} ClientThreadArg;

bool global_work_flag;

void *worker_thread_work(void *client_thread_arg);

#endif