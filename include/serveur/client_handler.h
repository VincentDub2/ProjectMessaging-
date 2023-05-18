#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "serveur_utils/client_list.h"

//Arguments pour le thread
typedef struct {
    int server_socket_message;
    int server_socket_file;
    client_info *info;
} thread_args;

/**
 * @brief Fonction exécutée par un thread pour gérer les interactions avec un client.
 *
 * @param arg Les arguments du thread.
 * @return NULL.
 */
void *handle_client(void *arg);

/**
 * @brief Déconnecte un client en fermant son socket, le retirant de la liste des clients et libérant le slot.
 *
 * @param info Les informations sur le client à déconnecter.
 * @return NULL.
 */
void disconnect_client(client_info *info);


#endif // CLIENT_HANDLER_H
