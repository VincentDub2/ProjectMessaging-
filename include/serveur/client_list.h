#ifndef CLIENT_LIST_H
#define CLIENT_LIST_H

#include <pthread.h>

// Structure pour stocker les informations des clients
typedef struct client_info {
    int socket_fd;
    int index;
    char* pseudo; //pseudo du client
    pthread_t thread_id; // Indentifiant du thread sur lequel le client est connecté
    struct client_info* next;
} client_info;


void add_client_to_list(client_info* info);
void remove_client_from_list(int client_index);
void shutdown_client_list(void);
client_info* get_head_list(void);
pthread_mutex_t* get_mutex_list(void);
client_info* get_client_by_pseudo(char* pseudo);
client_info* get_client_by_index(int client_index);


#endif
