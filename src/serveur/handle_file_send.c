
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "../../include/serveur/handle_file_send.h"

#include "../../include/serveur/communication_utils.h"

#include "../../include/serveur/socket_utils.h"

#include "../../include/common.h"

#include "../../include/serveur/signal_utilis.h"

#include "../../include/file_utils.h"

void *send_file_thread_func(void *args) {
    FileSendThreadArgs* thread_args = (FileSendThreadArgs*) args;

    printf("En attente de connexion sur le socket de transfert de fichiers...\n");

    // Acceptez une connexion sur le socket de transfert de fichiers
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int client_socket = accept(thread_args->serveur_socket_file, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket < 0) {
        perror("Erreur lors de l'acceptation de la connexion du client sur le socket de transfert de fichiers");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Envoyez les fichiers sélectionnés
    printf("Vous avez sélectionné les fichiers suivants:\n");
    printf("%s\n", thread_args->selected_indices);

    char *token = strtok(thread_args->selected_indices, ",");

    char response[] = "READY";
    send(client_socket, response, sizeof(response), 0);


    while (token != NULL) {
        int file_index = atoi(token);
        char* file_name = thread_args->file_list[file_index];

        printf("Envoi du fichier %s\n", file_name);

        printf("L'index du fichier est : %d \n",file_index);

        printf("PATCH_CLIENT_FILE: %s\n", PATCH_SERVEUR_FILE);

        if(file_name != NULL) {
            char full_file_path[256];
            sprintf(full_file_path, "%s/%s", PATCH_SERVEUR_FILE, file_name);

            printf("Envoi du fichier %s\n", full_file_path);

            // Envoyez le fichier
            send_file(full_file_path, client_socket, file_name);
        }

        token = strtok(NULL, ",");

        if (token != NULL) {
            // Indicate that we are ready to send the next file
            char response[] = "READY";
            send(client_socket, response, sizeof(response), 0);
        }
    }

    char end[] = "END";
    send(client_socket, end, sizeof(end), 0);


    close(client_socket);

    // Libérez la mémoire allouée pour les arguments du thread
    free(thread_args->selected_indices);
    free(thread_args);
    pthread_exit(NULL);
}


void handle_get_files_commande(int socket_client, const char *pseudo, int serveur_socket_file) {
    // Créez le thread d'envoi de fichiers
    pthread_t send_files_thread;

    char *file_list[MAX_FILES_LIST] = {NULL};
    list_files_in_directory(file_list, PATCH_SERVEUR_FILE);

    printf("sending file list...\n");

    send_file_list(file_list, pseudo);

    char ask[BUFFER_SIZE];
    memset(ask, 0, BUFFER_SIZE);
    strcat(ask, "Entrez les indices des fichiers à recevoir, séparés par des virgules:");

    printf("Demande des indices des fichiers au client...\n");
    mp_client(0, pseudo, ask);

    char buffer[MAX_FILES_LIST];
    int bytes_received = 0;

    printf("En attente de la réception des indices des fichiers...\n");

    while (get_server_running()) {

        memset(buffer, 0, MAX_FILES_LIST);

        bytes_received = recv(socket_client, buffer, MAX_FILES_LIST, 0);

        if(bytes_received < 0){
            if(errno == EAGAIN || errno == EWOULDBLOCK) {
                // Ressource temporairement non disponible, attendez et réessayez
                usleep(200000); // Attendre 200 ms pour éviter une utilisation élevée du processeur
                continue; // Permet de passer directement à la prochaine itération de la boucle
            } else {
                // Une véritable erreur s'est produite
                perror("Erreur lors de la réception des indices des fichiers");
                break;
            }
        }

        printf("Indices reçus: %s\n", buffer);
        if (strcmp(buffer, "exit") == 0){
            return;
        }
        if (check_valid_input(buffer,file_list)) {
            printf("Indices valides. Envoi de confirmation au client...\n");

            char Confirm_Msg[BUFFER_SIZE];
            memset(Confirm_Msg, 0, BUFFER_SIZE);

            strcat(Confirm_Msg, "OK");

            printf("voici le confirm msg : %s\n", Confirm_Msg);

            if(send(socket_client,Confirm_Msg, BUFFER_SIZE, 0) < 0){
                perror("Erreur lors de l'envoi de la confirmation au client");
                break;
            }
            FileSendThreadArgs *args = malloc(sizeof(FileSendThreadArgs));

            if (args == NULL) {
                perror("Erreur lors de l'allocation de la mémoire pour les arguments du thread");
                return;
            }

            args->selected_indices = strdup(buffer);
            args->file_list = file_list;
            args->serveur_socket_file = serveur_socket_file;

            if (pthread_create(&send_files_thread, NULL, send_file_thread_func, (void *)args) != 0) {
                perror("Erreur lors de la création du thread d'envoi de fichiers");
                free(args->selected_indices);
                free(args);
                return;
            }

            // Vous pouvez utiliser pthread_detach si vous ne voulez pas attendre le thread avec pthread_join
            pthread_detach(send_files_thread);

            printf("Confirmation envoyée au client.\n");
            break;
        }
        else {
            printf("Indices invalides. Envoi d'un message d'erreur au client...\n");
            if(send(socket, "invalid", strlen("invalid"), 0) < 0){
                perror("Erreur lors de l'envoi du message d'erreur au client");
                break;
            }
            printf("Message d'erreur envoyé au client.\n");
        }
    }

    printf("Fichiers sélectionnés: %s\n", buffer);
}


