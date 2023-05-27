#include "../../../include/client/message_thread/receive_message_thread.h"
#include "../../../include/client/message_thread/receive_message_thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>

#include "../../../include/common.h"
#include "../../../include/client/client_utils.h"

//#include "../../../include/client/display_message/display_message.h"
#include "../../../include/client/display_error/display_error_message.h"
#include "../../../include/client/display_event/display_event_message.h"
#include "../../../include/client/display_message/display_all_message.h"
#include "../../../include/client/display_message/display_mp_message.h"
#include "../../../include/client/display_manual/display_manual.h"

#include "../../../include/client/display_liste_users/display_liste_users.h"
#include "../../../include/client/display_tag/display_tag.h"
#include "../../../include/client/display_file/display_liste_file.h"
#include "../../../include/client/affichage_utils.h"
#include "../../../include/client/handle_get_file/get_file_valid_indice.h"
#include "../../../include/client/display_success/display_success.h"
#include "../../../include/client/display_channels/display_channels.h"
#include "../../../include/client/display_channels/display_channels_message.h"



// Structure pour stocker les arguments du thread
void *receive_messages_thread(void *arg) {
    //On récupère le socket
    ReceiveThreadArgs *args = (ReceiveThreadArgs *)arg;
    int socket_fd = args->socket_fd;
    const char* pseudo = args->pseudo;

    //On crée un buffer pour stocker les messages
    char buffer[BUFFER_SIZE];

    //On crée une variable pour stocker le nombre d'octets reçus
    int bytes_received;

    while (1) {
        // Réinitialise le tampon et reçoit un message du serveur
        memset(buffer, 0, BUFFER_SIZE);
        bytes_received = recv(socket_fd, buffer, BUFFER_SIZE, 0);
        // Si la réception échoue, sortir de la boucle
        if (bytes_received == 0) {
            printf("Erreur de connection avec le serveur \n");
            sleep(1);
            break;
        }

        // Recherche la position du caractère ':' qui sépare le protocole du message
        char *colon_position = strchr(buffer, ':');
        if (colon_position == NULL) {
            printf("Erreur: format de message invalide\n");
            continue;
        }

        // Extrayez le protocole et le message à partir de la chaîne d'origine
        int protocol_length = colon_position - buffer;

        char protocol[protocol_length + 1];

        strncpy(protocol, buffer, protocol_length);
        protocol[protocol_length] = '\0';

       char *message = colon_position + 2;

        // Gestion des données envoyées par le serveur
        // Traiter la commande
        if (strcmp(protocol, "event") == 0){
            display_event_message(message);
        }
        else if (strcmp(protocol, "error") == 0){
            display_error_message(message);
        }
        else if(strcmp(protocol, "mp") == 0){
            display_mp_message(message);
        }
        else if(strcmp(protocol,"all")==0){
            display_all_message(message);
        }
        else if (strcmp(protocol, "liste_user") == 0) {
            display_client_list(message);
        }
        else if(strcmp(protocol,"tag")==0){
            display_tag_message(message,pseudo);
        }
        else if(strcmp(protocol,"help")==0){
            display_manuel(message);
        }
        else if (strcmp(protocol, "fin") == 0) {
            printf("Le serveur a été arrêté \n");
            exit(EXIT_SUCCESS);
        }else if(strcmp(protocol, "file") == 0){
            display_liste_file(message);
        }
        else if (strcmp(protocol,"sendFile") == 0){
            //sendfile();
            printf("Le fichier a été envoyé \n");
        } else if(strcmp(protocol,"askIndice") == 0) {
            get_file_valid_indice(args->pipe_fd,socket_fd);
        } else if (strcmp(protocol,"channelList")==0){
            display_single_channel(message);
        } else if (strcmp(protocol,"success")==0){
            display_success_message(message);
        } else if (strcmp(protocol,"channel")==0){
              display_channel_message(message);
        }
        else {
            printf("Erreur: protocole inconnu\n");
            printf("Protocole: %s message : %s \n", protocol,message);
        }
    }

    close(args->pipe_fd[0]);

    pthread_exit(NULL);
}
