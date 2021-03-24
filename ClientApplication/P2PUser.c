#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include "P2PUser.h"

//Odbieranie danych
void rx() {

}

//Wysyłanie danych
void tx() {

}

//Obsługa informacji socketu
void handle_rxtx() {

}

//Stworzenie socketu dla nasłuchiwania połączenia
void set_out_data_socket() {

}

//Stworzenie socketu do nawiązywania połączenia
void set_in_data_socket() {

}

void parse_args(int argc, char **argv, P2PUser *user_properties) {
    size_t user_name_len;

    if (argc != 2) {
        printf("Usage: %s <name>\n", argv[0]);
        exit(-1);
    }

    user_name_len = strlen(argv[1]);
    printf("%d\n", user_name_len);
    if (user_name_len > USER_NAME_LEN - 1) {
        printf("Usage: %s <name>\n", argv[0]);
        exit(-1);
    }

    strncpy(user_properties->name, argv[1], user_name_len - 1);
    user_properties->name[user_name_len - 1] = '\0';
}

int main(int argc, char **argv) {
    P2PUser user_properties;

    parse_args(argc, argv, &user_properties);

    return 0;
}