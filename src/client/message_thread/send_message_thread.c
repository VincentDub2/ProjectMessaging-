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


char default_channel[BUFFER_SIZE] = "/all";

// Envoie des messages au serveur et gère les messages trop longs
void *send_messages_thread(void *arg) {
    SendThreadArgs* args = (SendThreadArgs*)arg;
    int socket_fd = args->socket_fd;

    char buffer[BUFFER_SIZE];

    printf("Entrez votre message: ");


    while (1){
        int message_send= 0;

        fgets(buffer, BUFFER_SIZE, stdin);

        if (strncmp(buffer,"/set", strlen("/set"))==0) {
            // Obtient le pointeur sur la valeur après "/set" (plus le " " après "/set").
            char *value_after_set = strtok(buffer, " ");
            value_after_set = strtok(NULL, "\n"); // obtient la valeur après "/set"

            if (value_after_set != NULL) { // s'il y a une valeur après "/set"
                strncpy(default_channel, value_after_set, BUFFER_SIZE - 1);
                default_channel[BUFFER_SIZE - 1] = '\0';

                if (strcmp(value_after_set, "all") == 0) {
                    printf("Vous êtes maintenant sur le channel all\n");
                    strncmp(default_channel, "/all", BUFFER_SIZE - 1);
                } else {
                    printf("Vous êtes maintenant sur le channel %s\n", default_channel);
                    char send_command[BUFFER_SIZE];
                    sprintf(send_command, "/send %s", default_channel);
                    strncpy(default_channel, send_command, BUFFER_SIZE - 1);
                    default_channel[BUFFER_SIZE - 1] = '\0';
                }
            } else {
                printf("Pas de channel spécifié après '/set'\n");
            }
            printf("Entrez votre message: ");
            continue;
        }
        if (buffer[0] == '/') {
            send_message(socket_fd, buffer);
            message_send=1;
        }

        //GESTION DES COMMANDES
        if (strncmp(buffer, "/sendFiles", strlen("/sendFiles"))==0){
            handle_send_files_command();
        }
        //GESTION GET_FILES
        else if (strncmp(buffer, "/getFiles", strlen("/getFiles"))==0){
            handle_get_files_command(socket_fd,args->pipe_fd);
        }
        //COMMANDE POUR SE DÉCONNECTER DU SERVEUR
        else if (strncmp(buffer, "/quit", strlen("/quit")) == 0) {
            exit(0);
            break;
        } else if (message_send==0){
            // Envoie le message au serveur
            char message[BUFFER_SIZE];
            sprintf(message, "%s %s", default_channel, buffer);
            send_message(socket_fd, message);
        }
    }
    close(args->pipe_fd[1]); //Mode lecture
    pthread_exit(NULL);
}


// Envoie une commande au serveur
int send_message(int socket_fd, char* buffer) {

    char sendMsg[BUFFER_SIZE];
    stpcpy(sendMsg, buffer);

    char *command, *channel, *message;

    command = strtok(buffer, " ");

    if(strcmp(command, "/all") == 0){
        message = strtok(NULL, "\0"); // obtient la partie du message après "/all"
        display_sent_message("me", message, "all");
    }
    else if(strcmp(command, "/send") == 0){
        channel = strtok(NULL, " "); // obtient le nom du channel après "/send"
        message = strtok(NULL, "\0"); // obtient la partie du message après le nom du channel
        display_sent_message("me", message, channel);
    }
    else {
        display_sent_message("me", sendMsg,"serveur");
    }


    // Affiche le message envoyé
    if (strlen(sendMsg) > MAX_MESSAGE_LENGTH) {
        printf("Message trop long. Veuillez entrer un message de %d caractères maximum.\n", MAX_MESSAGE_LENGTH);
        return -1;
    }
    else {
        if (send(socket_fd,sendMsg, strlen(sendMsg), 0)==-1){
            perror("Erreur lors de l'envoi du message");
            return -1;
        }
    }
    return 0;
}