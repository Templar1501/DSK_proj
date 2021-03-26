#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include "../ClientSimulation/ClientSimulation.h"

void err_exit_from_parse_args(char **argv){
    printf("Usage: %s [thread_number > 1]\n", argv[0]);
    exit(-1);
}

void parse_args(int argc, char **argv, long *thread_number) {
    if (argc != 2) err_exit_from_parse_args(argv);
    *thread_number = strtol(argv[1], NULL, 10);
    if (*thread_number < 2) err_exit_from_parse_args(argv);
}

void set_data_in_struct(ClientThreadCommon *client_thread_common, bool *entrances, long *numbers,long thread_amount) {
    client_thread_common->vector_size = thread_amount;
    client_thread_common->entrances = entrances;
    client_thread_common->numbers = numbers;
}

void init_exclude_system(ClientThreadArg *client_properties, long thread_amount) {//todo: handle errors
    int err;

    err = pthread_barrier_init(&client_properties->pass_data_barrier, NULL, thread_amount + 1);
    err = sem_init(&client_properties->pass_index_sem, 0, 1);
}

void spawn_clients(ClientThreadArg *client_properties, pthread_t *threads_id, long thread_amount) { //fixme: cause error
    long    index;
    int     err;

    for (index = 0; index < thread_amount; index++) {
        err = sem_wait(&client_properties->pass_index_sem);
        client_properties->client_thread_common.thread_index = index;
        pthread_create(&threads_id[index], NULL, &worker_thread_work, &client_properties);
    }
}

void finish_exclude_system(ClientThreadArg *client_properties) {//todo: handle errors
    int err;

    err = sem_wait(&client_properties->pass_index_sem);
    err = pthread_barrier_wait(&client_properties->pass_data_barrier);
    err = pthread_barrier_destroy(&client_properties->pass_data_barrier);
    err = sem_destroy(&client_properties->pass_index_sem);
}

void init_clients(pthread_t *threads_id, bool *entrances, long *numbers,long thread_amount) { //todo: handle errors
    ClientThreadArg     client_properties;
    long                index;
    int                 err;

    global_work_flag = true;
    set_data_in_struct(&client_properties.client_thread_common, entrances, numbers, thread_amount);
    init_exclude_system(&client_properties, thread_amount);
    
    //spawn_clients
    for (index = 0; index < thread_amount; index++) {
        err = sem_wait(&client_properties.pass_index_sem);
        client_properties.client_thread_common.thread_index = index;
        pthread_create(&threads_id[index], NULL, &worker_thread_work, &client_properties);
    }

    finish_exclude_system(&client_properties);
}

void wait_for_clients(pthread_t *threads_id, long thread_number) {
    long index;

    for (index = 0; index < thread_number; index++) pthread_join(threads_id[index], NULL);
}

void signal_handler(int signal) {
    global_work_flag = false;
}

void enable_sigint(void) {
    signal(SIGINT, signal_handler);
}

void alloc_threads_id(pthread_t **threads_id, bool **entrances, long **numbers,long thread_number) {
    *threads_id = malloc(sizeof(long) * thread_number);
    *entrances = calloc(thread_number, sizeof(bool));
    *numbers = calloc(thread_number, sizeof(long));
    if (*threads_id == NULL || *entrances == NULL || *numbers == NULL) {
        printf("Can not allocate memmory!\n");
        exit(-1);
    }
}

void free_threads_id(pthread_t *threads_id, bool *entrances, long *numbers) {
    free(threads_id);
    free(entrances);
    free(numbers);
}

int main(int argc, char **argv) {
    long        thread_number, *numbers;
    pthread_t   *threads_id;
    bool        *entrances;

    parse_args(argc, argv, &thread_number);
    enable_sigint();
    alloc_threads_id(&threads_id, &entrances, &numbers,thread_number);
    init_clients(threads_id, entrances, numbers, thread_number);
    wait_for_clients(threads_id, thread_number);
    free_threads_id(threads_id, entrances, numbers);

    return 0;
}