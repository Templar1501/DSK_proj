#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include "P2PUser.h"

//Zakończ działanie wątków
void set_user_inactive(int sig) {
    user_properties->is_active = false;
}

//Przypisanie obsługi SIGINT do zakończenia działania
void set_sigint_handler(void) {
    signal(SIGINT, set_user_inactive);
}

//Zwolenienie przydzielonych zasobów
void release_resources(void) {  //TODO; Obsługa błedów
    char sock_file[20];

    sprintf(sock_file, "%d_listen", user_properties->process_id);
    free(user_properties);
    unlink(sock_file);
}

//Wyjscie z błedem przy parsowaniu argumentów wejsciowych
void err_exit_parse(char *argv) {
    printf("Usage: %s [number > 0]\n", argv);
    exit(-1);
}

//Wyjscie i komunikat błedu dla tworzenie wątków 
void err_exit_thread(char *which, pthread_t thread_rx) {
    if (thread_rx != NULL) {
        set_user_inactive(0);
        pthread_join(thread_rx, NULL);
    }

    printf("Can not run %s thread!\n", which);
    release_resources();
    exit(-1);
}

//Podstawowa operacja na sockecie 
void common_un_sock_operation(struct sockaddr_un *un_socket, int *un_socket_fd, pid_t process_to_connect_id) {
    *un_socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (*un_socket_fd == -1) return;

    memset(un_socket, 0, sizeof(struct sockaddr_un));
    sprintf(un_socket->sun_path, "%d_listen", process_to_connect_id);
    un_socket->sun_family = AF_UNIX;
}

//Stworzenie socketu dla odbierania informacji
int set_in_data_socket(void) {
    struct sockaddr_un  un_socket;
    int                 un_socket_fd, err;

    common_un_sock_operation(&un_socket, &un_socket_fd, user_properties->process_id);
    if (un_socket_fd == -1) {
        printf("Can not create socket!\n");
        return -1;
    }

    err = bind(un_socket_fd, (struct sockaddr*)&un_socket, sizeof(struct sockaddr_un));
    if (err == -1) {
        printf("Can not bind!\n");
        return -1;
    }

    err = listen(un_socket_fd, 0);
    if (err == -1) {
        printf("Can not listen!\n");
        return -1;
    }

    return un_socket_fd;
}

//Stworzenie socketu do wysyłania infomracji
int set_out_data_socket(pid_t process_to_connect_id) {
    struct sockaddr_un  un_socket;
    int                 un_socket_fd, err;

    common_un_sock_operation(&un_socket, &un_socket_fd, process_to_connect_id);
    if (un_socket_fd == -1) {
        printf("Can not create socket!\n");
        return -1;
    }

    err = connect(un_socket_fd, (struct sockaddr*)&un_socket, sizeof(struct sockaddr_un));
    if (err == -1) {
        printf("Can not connect!\n");
        return -1;
    }
  
    return un_socket_fd;
}

//Odbieranie danych
void *rx(void *arg) {
    int un_socket_fd;

    while (user_properties->is_active) {
        un_socket_fd = set_in_data_socket();
        
        if (un_socket_fd == -1) sleep(1);
        else {

        }
    }

    pthread_exit(0);
}

//Wysyłanie danych
void *tx(void *arg) {
    int un_socket_fd;

    while (user_properties->is_active) { //TODO: znac pidy innych klientow
        //un_socket_fd = set_out_data_socket();
        //if (un_socket_fd == -1) sleep(1);
        //else {
        //
        //}
    }

    pthread_exit(0);
}

//Obsługa głownej funkcjonalności programu
void main_work(void) {
    pthread_t   thread_rx, thread_tx;
    int         err;
    
    err = pthread_create(&thread_rx, NULL, &rx, NULL);
    if (err) err_exit_thread("rx", NULL);

    err = pthread_create(&thread_tx, NULL, &tx, NULL);
    if (err) err_exit_thread("tx", thread_rx);
    
    pthread_join(thread_tx, NULL);
    pthread_join(thread_rx, NULL);
}

//Parsowanie argumentów wejsciowcyh
void parse_args(int argc, char **argv) {
    if (argc != 2) err_exit_parse(argv[0]);
    n_size = strtoul(argv[1], NULL, 10);
    if (n_size == 0) err_exit_parse(argv[0]);
}

//Wypełnianie podstawowymi danymi strkutury P2PUser
void fill_P2PUser_struct(void) {
    user_properties = calloc(1, sizeof(P2PUser));

    if (user_properties == NULL) {
        printf("Can not allocate memmory for P2PUser!\n");
        exit(-1);
    }

    user_properties->process_id = getpid();
    if (user_properties->process_id < 0) {
        printf("Can not get proces ID!\n");
        exit(-1);
    }

    user_properties->is_active = true;
    user_properties->trust_level = 0; //TODO: przemyslec
    user_properties->cash_amount = 200; //TODO: dac random
    printf("Client id: %d\n", user_properties->process_id);
    printf("Listening socket: %d_listen\n", user_properties->process_id);
}

//Punkt wejścia
int main(int argc, char **argv) {
    parse_args(argc, argv);
    fill_P2PUser_struct();
    set_sigint_handler();
    main_work();
    release_resources();

    return 0;
}