#include "../../../include/serveur/handle_tag/handle_tag.h"

#include <string.h> // pour memset et sprintf
#include <sys/socket.h> // pour send
#include <unistd.h> // pour close
#include <stdio.h> // pour perror
#include <string.h>
#include "../../../include/serveur/serveur_communication/communication.h"
#include "../../../include/serveur/serveur_utils/client_list.h"
#include "../../../include/common.h" // pour BUFFER_SIZE


int handle_tag_message(const char *buffer, int client_index, int client_socket, const char *pseudo) {
    // Trouver la position de l'arobase
    char *at_position = strchr(buffer, '@');

    // Extraire le pseudo qui suit l'arobase
    char extracted_pseudo[MAX_PSEUDO_LENGTH + 1]; // +1 pour le caractère de fin de chaîne
    int extracted_length = 0;
    sscanf(at_position + 1, "%s%n", extracted_pseudo, &extracted_length);

    // Vérifier si le pseudo extrait est valide
    if (extracted_length == 0) {
        // Envoyer un message d'erreur au client
        if (send_to_one_client("error", "Pseudo invalide", client_socket) == -1) {
            printf("Erreur lors de l'envoi du message d'erreur au client (pseudo invalide) \n");
            return -1;
        }
        return 1;
    }

    // Vérifier si le pseudo existe
    if (is_pseudo_available(extracted_pseudo) == 1) {
        // Envoyer un message d'erreur au client
        if (send_to_one_client("error", "Pseudo inexistant", client_socket)==-1) {
            printf("Erreur lors de l'envoi du message d'erreur au client (pseudo inexistant) \n");
            return -1;
        }
        return 1;
    }

    // Vérifier la longueur du pseudo extrait
    if (extracted_length <= MAX_PSEUDO_LENGTH) {
        // Traiter le message avec le pseudo extrait
        char message[BUFFER_SIZE];
        snprintf(message, BUFFER_SIZE, "%s:%s:%s", extracted_pseudo, pseudo,buffer);
        if(send_to_all_client("tag", message, client_index)==-1) {
            printf("Erreur lors de l'envoi du message tag à tous les clients \n");
            return -1;
        }
        return 0;
    } else {
        // Envoyer un message d'erreur au client
        if (send_to_one_client("error", "Pseudo trop long", client_socket)==-1) {
            printf("Erreur lors de l'envoi du message d'erreur au client (pseudo trop long) \n");
            return -1;
        }
        return 1;
    }
}
