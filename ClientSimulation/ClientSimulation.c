#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include "../LamportAlgorithm/LamportAlgorithm.h"

void copy_data_to_thread(ClientThreadArg *c_t_a, ClientThreadProperties *c_t_p) { //todo: handle errors
    int err;

    c_t_p->client_thread_common.thread_index = c_t_a->client_thread_common.thread_index;
    err = sem_post(&c_t_a->pass_index_sem);
    c_t_p->client_thread_common.vector_size = c_t_a->client_thread_common.vector_size;
    c_t_p->client_thread_common.entrances = c_t_a->client_thread_common.entrances;
    c_t_p->client_thread_common.numbers = c_t_a->client_thread_common.numbers;
    c_t_p->thread_id = pthread_self();
}

void wait_for_all_clients(ClientThreadArg *client_thread_arg) {  //todo: handle errors
    int err;

    err = pthread_barrier_wait(&client_thread_arg->pass_data_barrier);
}

void *worker_thread_work(void *client_thread_arg) {
    ClientThreadProperties client_thread_prop;

    copy_data_to_thread(client_thread_arg, &client_thread_prop);   
    wait_for_all_clients(client_thread_arg);
    lamport_exclusive(&client_thread_prop);
    pthread_exit(0);
}