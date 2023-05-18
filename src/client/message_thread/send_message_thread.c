#include "../../../include/client/message_thread/send_message_thread.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>


#include "../../../include/common.h"
#include "../../../include/client/affichage_utils.h"
#include "../../../include/client/handle_get_file/handle_get_files.h"
#include "../../../include/client/handle_send_file/handle_send_files.h"


// Envoie des messages au serveur et gère les messages trop longs
void *send_messages_thread(void *arg) {
    SendThreadArgs* args = (SendThreadArgs*)arg;
    int socket_fd = args->socket_fd;

    char buffer[BUFFER_SIZE];

    printf("Entrez votre message: ");

    while (1){

        fgets(buffer, BUFFER_SIZE, stdin);

        send_message(socket_fd, buffer);

        //GESTION DES COMMANDES
        if (strncmp(buffer, "/sendFiles", strlen("/sendFiles"))==0){
            handle_send_files_command();
        }
        //GESTION GET_FILES
        if (strncmp(buffer, "/getFiles", strlen("/getFiles"))==0){
            handle_get_files_command(socket_fd,args->pipe_fd);
        }
        //COMMANDE POUR SE DÉCONNECTER DU SERVEUR
        if (strcmp(buffer, "/quit") == 0) {
            exit(0);
        }
    }
    close(args->pipe_fd[1]); //Mode lecture
    pthread_exit(NULL);
}


// Envoie une commande au serveur
int send_message(int socket_fd, const char* message) {
    display_sent_message("me", message); // Affiche le message envoyé
    if (strlen(message) > MAX_MESSAGE_LENGTH) {
        printf("Message trop long. Veuillez entrer un message de %d caractères maximum.\n", MAX_MESSAGE_LENGTH);
        return -1;
    }
    else {
        if (send(socket_fd, message, strlen(message), 0)==-1){
            perror("Erreur lors de l'envoi du message");
            return -1;
        }
    }
    return 0;
}