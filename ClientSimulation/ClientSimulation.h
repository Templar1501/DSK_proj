#include <stdbool.h>
#include <semaphore.h>

typedef struct ClientSimulationCommon {
    long    vector_size, *numbers, thread_index;
    bool    *entrances;
} ClientThreadCommon;

typedef struct ClientSimulation {
    ClientThreadCommon  client_thread_common;
    sem_t               pass_index_sem;
    pthread_barrier_t   pass_data_barrier;
} ClientThreadArg;

typedef struct ClientSimulationShared {
    ClientThreadCommon  client_thread_common;
    pthread_t           thread_id;
    long                thread_index;
} ClientThreadProperties;


bool global_work_flag;

void *worker_thread_work(void *client_thread_arg);