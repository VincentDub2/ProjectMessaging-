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


//Détruit le mutex client_list_mutex.
//Libère la liste des clients en supprimant tous les éléments.
void shutdown_client_list(void);

//Ajoute un nouvel élément client_info à la liste des clients.
//Verrouille l'accès à la liste des clients pendant la modification.
void add_client_to_list(client_info* info);

//Supprime un élément client_info de la liste des clients en fonction de son index.
//Verrouille l'accès à la liste des clients pendant la modification.
void remove_client_from_list(int client_index);


//Envoie un message à tous les clients, sauf à celui qui l'a envoyé.
//Verrouille l'accès à la liste des clients pendant l'envoi du message.
void send_message_to_all_clients(int sender_index, const char* message,int buffer_size);

void mp_client(int sender_index,char* pseudo, const char* message, int buffer_size);

#endif
