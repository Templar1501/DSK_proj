#define USER_NAME_LEN 10

typedef struct client_properies {
    unsigned long   cash_amount;
    long            trust_level;
    char            name[USER_NAME_LEN];
} P2PUser;
