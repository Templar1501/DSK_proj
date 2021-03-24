#include <stdbool.h>

typedef struct client_properies {
    unsigned long   cash_amount;
    long            trust_level;
    pid_t           process_id;
    bool            is_active;
} P2PUser;

P2PUser *user_properties;
unsigned long n_size;