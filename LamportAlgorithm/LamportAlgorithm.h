#ifndef LAMPORTALGORITHM_H
#define LAMPORTALGORITHM_H

#include "../ClientSimulation/ClientSimulation.h"
#include "../BlockChainUtilities/Riddles.h"
#include <stdbool.h>

typedef struct LamportAlgorithmLock{
    long    current_thread_index, thread_amount; 
    long    *lp_vector;
    bool    *entrances;
} LamportMutex;

typedef struct LamportAlgorithmBarrier {
    LamportMutex     *lamport_mutex;
    bool             *queue;
    char             *flag, local_sense;
} LamportBarrier;

void lamport_exclusive(ClientThreadCommon *c_l_p);

#endif