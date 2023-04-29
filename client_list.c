#include "client_list.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>



// Pointeur vers la tête de la liste chaînée des clients
static client_info* client_list_head = NULL;

// Mutex pour protéger l'accès à la liste des clients
static pthread_mutex_t client_list_mutex = PTHREAD_MUTEX_INITIALIZER;



void add_client_to_list(client_info* info) {
    // Verrouiller l'accès à la liste des clients
    // pour éviter les conflits d'accès entre les différents threads
    pthread_mutex_lock(&client_list_mutex);
    info->next = client_list_head;

    // On ajoute le client au niveau de la tete de liste
    // car c'est plus rapide que de parcourir toute la liste
    // a voir si plus tard il faut pas l'ajouter a la fin plus tot
    client_list_head = info;

    // On peut libérer le thread, car on a fini de modifier la liste
    pthread_mutex_unlock(&client_list_mutex);
}

void remove_client_from_list(int client_index) {
    //On verrouille l'accès à la liste des clients
    pthread_mutex_lock(&client_list_mutex);


    client_info* prev = NULL;
    client_info* curr = client_list_head;

    //Tant que le client n'est pas trouvé on parcourt la liste
    while (curr != NULL) {
        if (curr->index == client_index) {

            // On vérifie le cas ou le client cherché est en tete de liste
            if (prev == NULL) {
                client_list_head = curr->next;
            } else {
                prev->next = curr->next;
            }
            // Permet de sortir de la liste une fois le client trouvé
            // Peu-etre reflechir a un autre moyen de quitter cette bloucle
            // car pas tres "propre"
            break;
        }

        prev = curr;
        curr = curr->next;
    }

    //On peut libérer le thread, car on a fini de modifier la liste
    pthread_mutex_unlock(&client_list_mutex);
}

void mp_client(int sender_index,char* pseudo, const char* message, int buffer_size) {

    // On crée un nouveau tampon pour ajouter l'index du client
    char message_with_index[buffer_size];

    // On ajoute l'index du client qui envoie le message
    sprintf(message_with_index, "client %d: %s", sender_index + 1, message);

    // On verrouille l'accès à la liste des clients
    pthread_mutex_lock(&client_list_mutex);

    // On parcourt la liste des clients et on envoie le message à tous les clients
    client_info *curr = client_list_head;
    while (curr != NULL) {
        if (curr->pseudo == pseudo) {
            if (send(curr->socket_fd, message_with_index, strlen(message_with_index), 0) == -1) {
                perror("Erreur lors de l'envoi du message");
            }
            break;
        }
        curr = curr->next;
    }
    // On libère la liste
    pthread_mutex_unlock(&client_list_mutex);

    // Si le destinataire n'est pas trouvé, on envoie un message d'erreur au client qui a envoyé la commande
    if (curr == NULL) {
        char error_msg[100];
        sprintf(error_msg, "Erreur : destinataire %s non trouvé.", pseudo);

        if (send(client_list_head->socket_fd, error_msg, strlen(error_msg), 0) == -1) {
            perror("Erreur lors de l'envoi du message d'erreur");
        }
    }

}

void send_message_to_all_clients(int sender_index, const char* message,int buffer_size) {

    // On crée un nouveau tampon pour ajouter l'index du client
    char message_with_index[buffer_size];

    // On ajoute l'index du client qui envoie le message
    sprintf(message_with_index, "client %d: %s", sender_index + 1, message);

    // On verrouille l'accès à la liste des clients
    pthread_mutex_lock(&client_list_mutex);

    // On parcourt la liste des clients et on envoie le message à tous les clients
    client_info *curr = client_list_head;
    while (curr != NULL) {
        if (curr->index != sender_index) {
            if (send(curr->socket_fd, message_with_index, strlen(message_with_index), 0) == -1) {
                perror("Erreur lors de l'envoi du message");
            }
        }
        curr = curr->next;
    }
    // On libère la liste
    pthread_mutex_unlock(&client_list_mutex);
}

void shutdown_client_list() {
    pthread_mutex_destroy(&client_list_mutex);

    while (client_list_head != NULL) {
        remove_client_from_list(client_list_head->index);
    }
}