#include "../../../include/serveur/command_message/handle_all_message.h"

#include <string.h> // pour memset et sprintf
#include <sys/socket.h> // pour send
#include <unistd.h> // pour close
#include <stdio.h> // pour perror
#include <string.h>
#include "../../../include/common.h" // pour BUFFER_SIZE
#include "../../../include/serveur/serveur_communication/communication.h"


int send_message_to_all_clients(int sender_index,const char * pseudo, const char* message) {
    // On crée un nouveau tampon pour ajouter le pseudo du client
    char message_with_pseudo[BUFFER_SIZE];

    // On ajoute le pseudo du client qui envoie le message
    sprintf(message_with_pseudo, "%s : %s", pseudo, message);

    if (send_to_all_client("all",message_with_pseudo,sender_index)==-1){
        perror("Erreur lors de l'envoi du message");
        return -1;
    }
    return 0;
}