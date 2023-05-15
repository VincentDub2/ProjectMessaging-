#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdlib.h>
#include "../../include/client/affichage_utils.h"
#include "../../include/client/signal_utils.h"
#include "../../include/client/client_utils.h"
#include "../../include/client/client.h"
#include "../../include/common.h"

#include "../../include/client/file_transfer_threads.h"
#include "../../include/client/message_threads.h"


int main() {
    // Crée un nouveau socket et se connecte au serveur

    int socket_fd = create_socket();

    // Configuration du signal SIGINT (Ctrl+C)
    setup_sigint_handler(socket_fd);

    // Connection au serveur
    connect_to_server(socket_fd, SERVER_IP, PORT);
    printf("Connecté au serveur\n");

    printf("Bienvenue sur le chat ! Pas de spam | Pas d'insulte.\n");

    // Demande le pseudo du client
    char* pseudo = ask_for_pseudo(socket_fd);

    if (pseudo == NULL) {
        printf("Erreur lors de la demande du pseudo\n");
        close(socket_fd);
        free(pseudo);
        exit(EXIT_FAILURE);
    }

    if (get_client_running() == 0) {
        printf("Client fermé\n");
        close(socket_fd);
        free(pseudo);
        exit(EXIT_FAILURE);
    }

    printf("Bienvenue %s ! Vous avez rejoint la conversation.\n", pseudo);
    printf("Pour quitter le chat, tapez Ctrl+C ou /quit\n");
    printf("-------------------------------------------\n");

    printf("Entrez votre message: ");
    fflush(stdout);

    pthread_t send_thread, receive_thread;

    // Crée un thread pour envoyer des messages
    pthread_create(&send_thread, NULL, send_messages, (void *)&socket_fd);

    // Crée un thread pour recevoir des messages
    pthread_create(&receive_thread, NULL, receive_messages, (void *)&socket_fd);

    //Permet d'attendre que les threads se terminent avant de continuer l'exécution du programme principal.
    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);

    // Ferme le socket
    close(socket_fd);
    free(pseudo);

    printf("\r Fin de connection\n");
    return 0;
}



