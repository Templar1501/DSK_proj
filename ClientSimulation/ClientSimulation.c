#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include "../LamportAlgorithm/LamportAlgorithm.h"

void copy_data_to_thread(ClientThreadArg *c_t_a, ClientThreadCommon *c_t_c) { //todo: handle errors
    int err;

    c_t_c->thread_index = c_t_a->client_thread_common.thread_index;
    err = sem_post(&c_t_a->pass_index_sem);
    c_t_c->thread_amount = c_t_a->client_thread_common.thread_amount;
    c_t_c->entrances = c_t_a->client_thread_common.entrances;
    c_t_c->lp_vector = c_t_a->client_thread_common.lp_vector;
}

void wait_for_all_clients(ClientThreadArg *c_t_a) {  //todo: handle errors
    int err;

    err = pthread_barrier_wait(&c_t_a->pass_data_barrier);
}

void *worker_thread_work(void *client_thread_arg) {
    ClientThreadCommon client_thread_comm;

    copy_data_to_thread(client_thread_arg, &client_thread_comm);   
    wait_for_all_clients(client_thread_arg);
    lamport_exclusive(&client_thread_comm);
    pthread_exit(0);
}