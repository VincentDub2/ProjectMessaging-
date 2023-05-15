#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "../serveur/client_list.h"

//Arguments pour le thread
typedef struct {
    int server_socket_message;
    int server_socket_file;
    client_info *info;
} thread_args;

void wait_for_clients(int server_socket_message, int server_socket_file);
//pre : socket du serveur crée
//post : attend les clients et les ajoute à la liste des clients
//      crée un thread pour chaque client

void *handle_client(void *arg);
//pre : place disponible grace au semaphore
//post : traite les messages du client
//      si le client envoie /fin, le supprime de la liste des clients
//    et appelle disconnect_client

void disconnect_client(client_info *info);
//pre : client_info valide
//post : supprime le client de la liste des clients
//   et ferme la connexion / le thread

#endif // CLIENT_HANDLER_H
