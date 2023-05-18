#include "../../../include/serveur/serveur_communication/communication.h"

#include <string.h> // pour memset et sprintf
#include <sys/socket.h> // pour send
#include <unistd.h> // pour close
#include <stdio.h> // pour perror
#include <string.h>
#include <pthread.h> // pour pthread_mutex_lock
#include "../../../include/common.h" // pour BUFFER_SIZE
#include "../../../include/serveur/serveur_utils/client_list.h"

int send_to_one_client(const char* protocol,const char* message, int socket_client){
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    sprintf(buffer, "%s: %s",protocol, message);

    // Envoie le message au client
    if (send(socket_client, buffer, BUFFER_SIZE, 0) == -1){
        char error_message[BUFFER_SIZE];
        sprintf(error_message,"Erreur lors de l'envoi de %s :%s",protocol,message);
        perror(error_message);
        return -1; // Indique une erreur
    }

    return 0; // Tout s'est bien passé
}

int send_to_all_client(const char* protocol,const char* message, int sender_index){

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    sprintf(buffer, "%s: %s", protocol, message);

    pthread_mutex_t* mutex = get_mutex_list();

    pthread_mutex_lock(mutex);

    // On parcourt la liste des clients et on envoie le message à tous les clients

    client_info *curr = get_head_list();

    // On parcourt la liste des clients et on envoie le message à tous les clients
    // Sauf au client qui a envoyé le message
    //Note a l'index 0 il y a le serveur

    while (curr != NULL) {
        if (curr->pseudo!=NULL && curr->index!=sender_index && curr->socket_fd!=NULL) {
            if (send(curr->socket_fd, buffer, BUFFER_SIZE, 0) == -1) {
                char error_message[BUFFER_SIZE];
                sprintf(error_message,"Erreur lors de l'envoi de %s :%s a tous les clients",protocol,message);
                perror(error_message);
                return -1; // Indique une erreur
            }
        }
        curr = curr->next;
    }

    // On libère la liste
    pthread_mutex_unlock(mutex);

    return 0; // Tout s'est bien passé
}