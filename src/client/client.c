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

#include "../../include/client/handle_send_file/file_transfer_threads.h"

#include "../../include/client/message_thread/receive_message_thread.h"
#include "../../include/client/message_thread/send_message_thread.h"


int main() {
    // Crée un nouveau socket et se connecte au serveur

    int socket_fd = create_socket();

    // Configuration du signal SIGINT (Ctrl+C)
    setup_sigint_handler(socket_fd);

    // Connection au serveur
    //Scoket_fd est le socket du client
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

    printf("Bienvenue %s ! Vous avez rejoint la conversation.\n", pseudo);
    printf("Pour quitter le chat, tapez Ctrl+C ou /quit\n");
    printf("-------------------------------------------\n");

    printf("Entrez votre message: ");
    fflush(stdout);

    pthread_t send_thread, receive_thread;

    // Création du pipe
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("Erreur lors de la création du pipe");
        exit(EXIT_FAILURE);
    }

    // Création de la structure d'arguments pour le thread de réception
    ReceiveThreadArgs thread_args;
    thread_args.pipe_fd[0] = -1; // Extrémité de lecture du pipe
    thread_args.pipe_fd[1] = pipefd[1]; // Pas utilisée dans le thread de réception
    thread_args.socket_fd = socket_fd;
    thread_args.pseudo = pseudo;

// Création de la structure d'arguments pour le thread d'envoi
    SendThreadArgs send_args;
    send_args.pipe_fd[0] =  pipefd[0]; // Pas utilisée dans le thread d'envoi
    send_args.pipe_fd[1] = -1; // Extrémité d'écriture du pipe
    send_args.socket_fd = socket_fd;

    // Crée un thread pour envoyer des messages
    pthread_create(&send_thread, NULL, send_messages_thread, (void *)&send_args);

    // Crée un thread pour recevoir des messages
    pthread_create(&receive_thread, NULL, receive_messages_thread, (void *)&thread_args);

    //Permet d'attendre que les threads se terminent avant de continuer l'exécution du programme principal.
    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);

    // Ferme le socket
    close(socket_fd);
    free(pseudo);

    printf("\r Fin de connection\n");
    return 0;
}



