#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>


#include "../../include/serveur/client_list.h"
#include "../../include/serveur/common.h"
#include "../../include/serveur/communication_utils.h"


void mp_client(int sender_index,char* pseudo, const char* message);
void send_message_to_all_clients(int sender_index, const char* message);

void send_fin(){
    char message[100];
    sprintf(message, "/fin");

    pthread_mutex_t* mutex = get_mutex_list();
    // On verrouille l'accès à la liste des clients
    pthread_mutex_lock(mutex);

    // On parcourt la liste des clients et on envoie le message à tous les clients
    client_info *curr = get_head_list();
    while (curr != NULL) {
        if (send(curr->socket_fd,message, strlen(message), 0) == -1) {
            perror("Erreur lors de l'envoi du message");
        }
        curr = curr->next;
    }

    // On libère la liste
    pthread_mutex_unlock(mutex);

}

void mp_client(int sender_index,char* pseudo, const char* message) {

    // On crée un nouveau tampon pour ajouter l'index du client
    char message_with_index[BUFFER_SIZE];

    // On ajoute l'index du client qui envoie le message
    sprintf(message_with_index, "client %d: %s", sender_index + 1, message);

    // On parcourt la liste des clients et on envoie le message à tous les clients

    client_info *recipient = get_client_by_pseudo(pseudo);

    // Si le destinataire n'est pas trouvé, on envoie un message d'erreur au client qui a envoyé la commande

    if (recipient == NULL) {
        char error_msg[100];
        sprintf(error_msg, "Erreur : destinataire %s non trouvé.", pseudo);
        client_info *sender = get_client_by_index(sender_index);

        if (send(sender->socket_fd, error_msg, strlen(error_msg), 0) == -1) {
            perror("Erreur lors de l'envoi du message d'erreur");
        }
    }

    else {
        if (send(recipient->socket_fd, message_with_index, strlen(message_with_index), 0) == -1) {
            perror("Erreur lors de l'envoi du message");
        }

    }



    // Si le destinataire n'est pas trouvé, on envoie un message d'erreur au client qui a envoyé la commande


}


void send_message_to_all_clients(int sender_index, const char* message) {
    pthread_mutex_t* mutex = get_mutex_list();

    // On crée un nouveau tampon pour ajouter le pseudo du client
    char message_with_pseudo[BUFFER_SIZE];

    // On récupère le pseudo du client qui envoie le message
    client_info *sender = get_client_by_index(sender_index);
    if (sender == NULL) {
        perror("Erreur: client introuvable");
        return;
    }

    // On ajoute le pseudo du client qui envoie le message
    sprintf(message_with_pseudo, "%s: %s", sender->pseudo, message);

    // On verrouille l'accès à la liste des clients
    pthread_mutex_lock(mutex);

    // On parcourt la liste des clients et on envoie le message à tous les clients
    client_info *curr = get_head_list();
    while (curr != NULL) {
        if (curr->index != sender_index) {
            if (send(curr->socket_fd, message_with_pseudo, strlen(message_with_pseudo), 0) == -1) {
                perror("Erreur lors de l'envoi du message");
            }
        }
        curr = curr->next;
    }

    // On libère la liste
    pthread_mutex_unlock(mutex);
}

