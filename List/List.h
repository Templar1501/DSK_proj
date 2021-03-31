#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    unsigned long riddle;
    unsigned long answer;
    unsigned long client_id;
    struct node * next;
} node_t;
