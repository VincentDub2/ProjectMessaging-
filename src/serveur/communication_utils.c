#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>

#include "../../include/serveur/client_list.h"
#include "../../include/common.h"
#include "../../include/serveur/communication_utils.h"


void mp_client(int sender_index,const char* pseudo, const char* message);
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

void mp_client(int sender_index,const char* pseudo, const char* message) {

    // On crée un nouveau tampon pour ajouter l'index du client
    char message_with_pseudo[BUFFER_SIZE];

    client_info *sender = get_client_by_index(sender_index);
    if (sender == NULL) {
        perror("Erreur: client introuvable");
        return;
    }

    // On ajoute le pseudo du client qui envoie le message
    sprintf(message_with_pseudo, "%s: %s", sender->pseudo, message);

    // On parcourt la liste des clients et on envoie le message à tous les clients

    client_info *recipient = get_client_by_pseudo(pseudo);

    // Si le destinataire n'est pas trouvé, on envoie un message d'erreur au client qui a envoyé la commande

    if (recipient == NULL) {
        char error_msg[100];
        sprintf(error_msg, "Serveur: destinataire %s non trouvé.", pseudo);
        client_info *sender = get_client_by_index(sender_index);

        if (send(sender->socket_fd, error_msg, strlen(error_msg), 0) == -1) {
            perror("Erreur lors de l'envoi du message d'erreur");
        }
    }

    else {
        if (send(recipient->socket_fd, message_with_pseudo, strlen(message_with_pseudo), 0) == -1) {
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
        if (curr->index != sender_index && curr->pseudo!=NULL) {
            if (send(curr->socket_fd, message_with_pseudo, strlen(message_with_pseudo), 0) == -1) {
                perror("Erreur lors de l'envoi du message");
            }
        }
        curr = curr->next;
    }

    // On libère la liste
    pthread_mutex_unlock(mutex);
}

void send_welcome_message_to_clients(const char *new_client_pseudo) {
    char welcome_message[BUFFER_SIZE];
    snprintf(welcome_message, BUFFER_SIZE, "%s a rejoint la conversation.", new_client_pseudo);
    send_message_to_all_clients(0, welcome_message);
}

void send_disconnect_message_to_clients(const char *new_client_pseudo) {
    char welcome_message[BUFFER_SIZE];
    snprintf(welcome_message, BUFFER_SIZE, "%s a viens de quitter la conversation.", new_client_pseudo);
    send_message_to_all_clients(0, welcome_message);
}

void send_client_list(const char * pseudo) {
    // Récupérer le mutex pour la liste des clients
    pthread_mutex_t *mutex = get_mutex_list();
    pthread_mutex_lock(mutex);

    // Parcourir la liste des clients et construire la chaîne de caractères pour la liste
    char client_list[BUFFER_SIZE];
    memset(client_list, 0, BUFFER_SIZE);
    strcat(client_list, "LIST");
    client_info *current = get_head_list();
    while (current != NULL) {
        // Ignorer le compte Serveur
        if (current->index != 0) {
            strcat(client_list, " ");
            strcat(client_list, current->pseudo);
        }
        current = current->next;
    }

    // Libérer le mutex pour la liste des clients
    pthread_mutex_unlock(mutex);

    // Envoyer la liste au client
    mp_client(0,pseudo, client_list);
}


void send_manual(const char *pseudo) {

    FILE *file = fopen("manual.txt", "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier manual.txt");
        return;
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        mp_client(0, pseudo, buffer);
    }
    fclose(file);
}