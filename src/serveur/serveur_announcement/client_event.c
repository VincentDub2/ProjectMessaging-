#include "../../../include/serveur/serveur_annoucement/client_event.h"

#include <string.h> // pour memset et sprintf
#include <sys/socket.h> // pour send
#include <unistd.h> // pour close
#include <stdio.h> // pour perror
#include <string.h>
#include "../../../include/common.h" // pour BUFFER_SIZE
#include "../../../include/serveur/serveur_communication/communication.h"



int send_join_message_to_clients(const char *client_pseudo) {
    char welcome_message[BUFFER_SIZE];
    snprintf(welcome_message, BUFFER_SIZE, "%s a rejoint la conversation.", client_pseudo);
    if (send_to_all_client("event",welcome_message,0)==-1){
        perror("Error lors de join message");
        return -1;
    }
    return 0;
}

int send_disconnect_message_to_clients(const char *client_pseudo) {
    char disconnect_message[BUFFER_SIZE];
    snprintf(disconnect_message, BUFFER_SIZE, "%s a rejoint la conversation.", client_pseudo);
    if (send_to_all_client("event",disconnect_message,0)==-1){
        perror("Error lors de disconnect message");
        return -1;
    }
    return 0;
}

int send_shutdown_message_to_clients() {
    char shutdown_message[BUFFER_SIZE];
    snprintf(shutdown_message, BUFFER_SIZE, "Le serveur va s'arrêter dans 1 seconde.");
    if (send_to_all_client("event",shutdown_message,0)==-1){
        perror("Error lors de shutdown message");
        return -1;
    }
    if (send_to_all_client("fin",shutdown_message,0)==-1){
        perror("Error lors de shutdown message");
        return -1;
    }
    return 0;
}