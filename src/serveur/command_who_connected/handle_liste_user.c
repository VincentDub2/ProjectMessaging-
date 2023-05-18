#include "../../../include/serveur/command_who_connected/handle_liste_user.h"

#include <string.h> // pour memset et sprintf
#include <sys/socket.h> // pour send
#include <unistd.h> // pour close
#include <stdio.h> // pour perror
#include <string.h>
#include "../../../include/common.h" // pour BUFFER_SIZE
#include "../../../include/serveur/serveur_communication/communication.h"
#include "../../../include/serveur/serveur_utils/client_list.h"


int send_all_client_connected(int client_socket) {
    // Récupérer le mutex pour la liste des clients
    pthread_mutex_t *mutex = get_mutex_list();

    pthread_mutex_lock(mutex);

    // Parcourir la liste des clients et construire la chaîne de caractères pour la liste
    char client_list[BUFFER_SIZE];
    memset(client_list, 0, BUFFER_SIZE);

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
    if (send_to_one_client("liste_user", client_list, client_socket) == -1) {
        perror("Erreur lors de l'envoi de la liste des utilisateurs connectés");
        return -1;
    }

    return 0;

}
