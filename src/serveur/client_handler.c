#include "../include/serveur/client_handler.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../../include/serveur/serveur_utils/rk_sema.h"

#include "../../include/serveur/serveur_utils/serveur_utils.h"

#include "../include/common.h"

#include "../include/serveur/serveur_utils/handle_pseudo.h"

#include "../../include/serveur/command_transfer_file/handle_file_reception.h"

#include "../../include/serveur/command_transfer_file/handle_file_send.h"

#include "../../include/serveur/serveur_utils/client_list.h"

#include "../../include/serveur/serveur_annoucement/client_event.h"

#include "../../include/serveur/command_message/handle_mp_message.h"

#include "../../include/serveur/command_message/handle_all_message.h"

#include "../../include/serveur/command_who_connected/handle_liste_user.h"

#include "../../include/serveur/serveur_communication/communication.h"

#include "../../include/serveur/command_help/handle_help.h"

#include "../../include/serveur/handle_tag/handle_tag.h"

void *handle_client(void *arg) {

    thread_args *args = (thread_args *) arg;

    int server_socket_message = args->server_socket_message; // PAS UTILISER POUR L'INSTANT
    int server_socket_file = args->server_socket_file;
    client_info *info = args->info;


    int client_socket = info->socket_fd;
    int client_index = info->index;

    printf("Client %d choisi un pseudo \n", client_index);

    char pseudo_buffer[MAX_PSEUDO_LENGTH + 1]; // +1 pour le caractère de fin de chaîne

    receive_pseudo_from_client(pseudo_buffer, client_socket);

    info->pseudo = strdup(pseudo_buffer); // Copier le pseudo dans la structure client_info

    // Envoie du message "x a rejoint le chat" à tous les clients
    send_join_message_to_clients(pseudo_buffer);

    char buffer[BUFFER_SIZE];
    int bytes_received;

    printf("Client %d est connecté\n", client_index);

    while (1) {

        // Réinitialiser le tampon
        memset(buffer, 0, BUFFER_SIZE);

        // Recevoir un message du client
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received == -1) {
            perror("Erreur lors de la réception du message du client");
            break;
        }

        // Si la réception échoue sortir de la boucle
        if (bytes_received == 0) {
            printf("Erreur de connection avec le client %d \n", client_index);
            break;
        }
        printf("Client %d : %s\n", client_index, buffer);

        // Gestion des commandes
        if (buffer[0] == '/'){
            printf("Commande reçue\n");
            // Extraire la commande
            char *commande = strtok(buffer, " ");
            commande++; // Ignorer le caractère '/' dans la commande
            printf("Commande : %s\n", commande);

            if (strncmp(commande, "list", strlen("list")) == 0) {
                printf("Sending list to client");
                send_all_client_connected(client_socket);
            }
            else if(strncmp(commande, "sendFiles", strlen("sendFiles"))==0) {
                handle_file_reception_command(server_socket_file);
            }
            else if (strncmp(commande, "getFiles", strlen("getFiles")) == 0) {
                printf("getFiles");
                handle_get_files_commande(client_socket,pseudo_buffer,server_socket_file);
            }

            else if (strcmp(commande, "quit") == 0) {
                // Déconnecter le client
                break;
            }
            else if (strncmp(commande, "help",strlen("help")) == 0){
                send_manual(client_socket);
            }
            else if (strcmp(commande, "mp") == 0){
                //Message privé
                //Extraire le pseudo et le message
                char *pseudo_target = strtok(NULL, " ");
                char *message = strtok(NULL, "");

                // Vérifier que le pseudo et le message sont valides
                if (pseudo_target != NULL && message != NULL) {
                    // Envoyer le message privé
                    mp_client(client_socket,info->pseudo,pseudo_target,message);
                } else {
                    // Envoyer un message d'erreur au client
                    send_to_one_client("error","format de commande invalide",client_socket);
                }
            }
            // Gestion des tags
            else {
                // Envoyer un message d'erreur au client
                send_to_one_client("error","commande inconnue",client_socket);
            }
        }
        else if (strstr(buffer, "@")) {
            handle_tag_message(buffer,client_index,client_socket,info->pseudo);
        }
        else {
            // Renvoyer le message à tous les autres clients
            send_message_to_all_clients(client_index, info->pseudo, buffer);
        }
    }

    //déconnexion du client

    // Envoyer le message de déconnexion à tous les clients
    send_disconnect_message_to_clients(info->pseudo);

    free(args);
    disconnect_client(info);
}

void disconnect_client(client_info *info) {
    int client_socket = info->socket_fd;
    int client_index = info->index;

    printf("En cours de déconnexion du client %d\n", client_index);

    // Fermer le socket et retirer le client de la liste chaînée
    close(client_socket);


    remove_client_from_list(client_index);


    printf("Client %d déconnecté\n", client_index);

    // Lorsque le client se déconnecte, libérer le slot
    rk_sema_post(get_client_slot());

    // Terminer le thread associé au client
    pthread_exit(NULL);
}


