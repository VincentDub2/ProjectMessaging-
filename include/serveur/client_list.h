#ifndef CLIENT_LIST_H
#define CLIENT_LIST_H

#include <pthread.h>

// Structure pour stocker les informations des clients
typedef struct client_info {
    int socket_fd; // socket du client
    int index;  // index du client dans la liste
    char* pseudo; //pseudo du client
    pthread_t thread_id; // identifiant du thread sur lequel le client est connecté
    struct client_info* next;
} client_info;


void add_client_to_list(client_info* info);
//pre : client_info valide
//post : ajoute le client à la liste des clients

void remove_client_from_list(int client_index);
//pre : client_index valide
//post : supprime le client de la liste des clients

void shutdown_client_list(void);
// pre : -
// post : libère la liste des clients

client_info* get_head_list(void);
// pre : -
// post : retourne le premier client de la liste

pthread_mutex_t* get_mutex_list(void);
// pre : -
// post : retourne le mutex de la liste des clients

client_info* get_client_by_pseudo(const char* pseudo);
// pre : pseudo valide
// post : retourne le client dont le pseudo est pseudo

client_info* get_client_by_index(int client_index);
// pre : client_index valide
// post : retourne le client dont l'index est client_index

int is_pseudo_available(char *pseudo);
// pre : pseudo valide
// post : retourne 1 si le pseudo est disponible, 0 sinon


void add_serveur_account(void);
// pre : -
// post : ajoute le compte serveur à la liste des clients


#endif
