#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void *worker_thread_work(void *arg) {
    pthread_t thread_id;

    thread_id = pthread_self();
    printf("Thread no.%lu started work\n", thread_id);
    sleep(3);
    printf("Thread no.%lu finished work\n", thread_id);
    pthread_exit(0);
}