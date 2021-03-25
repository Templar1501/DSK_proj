#include <stdbool.h>

typedef struct ClientSimulation {
    long                vector_size;
    pthread_barrier_t   pass_data_mutex;
} ClientThreadArg;

bool global_work_flag;

void *worker_thread_work(void *client_thread_arg);