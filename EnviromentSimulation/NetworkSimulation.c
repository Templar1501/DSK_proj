#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../ClientSimulation/ClientSimulation.h"

void err_exit_from_parse_args(char **argv){
    printf("Usage: ./%s [thread_number > 1]\n", argv[0]);
    exit(-1);
}

void parse_args(int argc, char **argv, long *thread_number) {
    if (argc != 2) err_exit_from_parse_args(argv);
    *thread_number = strtol(argv[1], NULL, 10);
    if (*thread_number < 2) err_exit_from_parse_args(argv);
}

void spawn_clients(pthread_t *threads_id, long thread_number) { //todo: handle errors
    ClientThreadArg     client_properties;
    long                index;
    int                 err;
    
    client_properties.vector_size = thread_number;
    err = pthread_barrier_init(&client_properties.pass_data_mutex, NULL, thread_number + 1);
    for (index = 0; index < thread_number; index++) pthread_create(&threads_id[index], NULL, &worker_thread_work, &client_properties);
    err = pthread_barrier_wait(&client_properties.pass_data_mutex);
    err = pthread_barrier_destroy(&client_properties.pass_data_mutex);
}

void wait_for_clients(pthread_t *threads_id, long thread_number) {
    long index;

    for (index = 0; index < thread_number; index++) pthread_join(threads_id[index], NULL);
}

void signal_handler(int signal) {
    
}

void alloc_threads_id(pthread_t **threads_id, long thread_number) {
    *threads_id = malloc(sizeof(long) * thread_number);
    if (*threads_id == NULL) {
        printf("Can not allocate memmory!\n");
        exit(-1);
    }
}

void free_threads_id(pthread_t *threads_id) {
    free(threads_id);
}

int main(int argc, char **argv) {
    long        thread_number;
    pthread_t   *threads_id;

    parse_args(argc, argv, &thread_number);
    alloc_threads_id(&threads_id, thread_number);
    spawn_clients(threads_id, thread_number);
    wait_for_clients(threads_id, thread_number);
    free_threads_id(threads_id);

    return 0;
}