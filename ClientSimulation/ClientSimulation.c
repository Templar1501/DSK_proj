#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "ClientSimulation.h"

void copy_data_to_thread(ClientThreadArg *client_thread_arg, long *vector_size, pthread_t *thread_id) { //todo: handle errors
    int err;

    *vector_size = client_thread_arg->vector_size;
    err = pthread_barrier_wait(&client_thread_arg->pass_data_mutex);
    *thread_id = pthread_self();
}

void start_prompt(pthread_t thread_id) {
    printf("Thread no.%lu started work\n", thread_id);
}

void end_prompt(pthread_t thread_id) {
    printf("Thread no.%lu finished work\n", thread_id);
}

void alloc_vector(long **vector, long vector_size, pthread_t thread_id) {
    *vector = calloc(vector_size, sizeof(long));
    if (*vector == NULL) {
        printf("Thread no.%lu can not allocate memmory\n", thread_id);
        pthread_exit((void *)-1);
    }
}

void thread_work() {
    sleep(3);
}

void *worker_thread_work(void *client_thread_arg) {
    pthread_t   thread_id;
    long        vector_size, *vector;

    copy_data_to_thread(client_thread_arg, &vector_size, &thread_id);
    start_prompt(thread_id);
    alloc_vector(&vector, vector_size, thread_id);
    thread_work();
 
    free(vector);
    end_prompt(thread_id);
    pthread_exit(0);
}