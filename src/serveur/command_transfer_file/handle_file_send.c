
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

#include "../../../include/serveur/command_transfer_file/handle_file_send.h"

#include "../../../include/serveur/serveur_utils/socket_utils.h"

#include "../../../include/common.h"

#include "../../../include/serveur/signal_utilis.h"

#include "../../../include/serveur/serveur_communication/communication.h"

#include "../../../include/file_utils.h"



void *send_file_thread_func(void *args) {
    FileSendThreadArgs* thread_args = (FileSendThreadArgs*) args;

    char **file_list = thread_args->file_list;

    char *file_name = file_list[0];

    int index = 0;
    while (file_name !=NULL){
        printf("file name : %s\n", file_name);
        index++;
        file_name = file_list[index];
    }


    printf("En attente de connexion sur le socket de transfert de fichiers...\n");


    // Acceptez une connexion sur le socket de transfert de fichiers
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int client_socket = accept(thread_args->serveur_socket_file, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket < 0) {
        perror("Erreur lors de l'acceptation de la connexion du client sur le socket de transfert de fichiers");
        exit(EXIT_FAILURE);
    }


    // Envoyez les fichiers sélectionnés
    printf("Vous avez sélectionné les fichiers suivants:\n");
    printf("%s\n", thread_args->selected_indices);

    char *saveptr;
    char *token = strtok_r(thread_args->selected_indices, ",", &saveptr);


    char response[] = "READY";

    if (send(client_socket, response, sizeof(response), 0)==-1){
        perror("Erreur lors de l'envoi de la réponse READY");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    while (token != NULL) {
        char *endptr;
       int file_index = strtol(token, &endptr, 10);

        if (endptr == token) {
            perror("Erreur lors de la conversion de l'index du fichier en entier");
        } else {

        char* file_name = file_list[file_index];

        if(file_name != NULL) {
            char full_file_path[256];
            sprintf(full_file_path, "%s/%s", PATCH_SERVEUR_FILE, file_name);

            printf("Envoi du fichier %s\n", full_file_path);

            // Envoyez le fichier
            send_file(full_file_path, client_socket, file_name);
        }

        token = strtok_r(NULL, ",", &saveptr); // get next token

        if (token != NULL) {
            // Indicate that we are ready to send the next file
            char response[] = "READY";
            send(client_socket, response, sizeof(response), 0);
        }
    }
    }

    char end[] = "END";
    send(client_socket, end, sizeof(end), 0);


    close(client_socket);

    // Libérez la mémoire allouée pour les arguments du thread
    free(thread_args->selected_indices);

    for (int i = 0; i < MAX_FILES_LIST && file_list[i] != NULL; i++) {
        free(file_list[i]);
    }

    free(thread_args->file_list);
    free(thread_args);

    pthread_exit(NULL);
}


int handle_get_files_commande(int socket_client, const char *pseudo, int serveur_socket_file) {
    // Créez le thread d'envoi de fichiers
    pthread_t send_files_thread;

    char **file_list = malloc(MAX_FILES_LIST * sizeof(char *));
    if (!file_list) {
        perror("Erreur d'allocation pour file_list");
        return -1;
    }


    list_files_in_directory(file_list, PATCH_SERVEUR_FILE);

    char *file_name = file_list[0];

    int index = 0;
    while (file_name !=NULL){
        printf("file name : %s\n", file_name);
        index++;
        file_name = file_list[index];
    }


    send_file_list(file_list, socket_client);

    printf("After l'envoie de la liste des fichiers\n");

    file_name = file_list[0];

    index = 0;
    while (file_name !=NULL){
        printf("file name : %s\n", file_name);
        index++;
        file_name = file_list[index];
    }

    char ask[BUFFER_SIZE];

    memset(ask, 0, BUFFER_SIZE);
    strcat(ask, "Entrez les indices des fichiers à recevoir, séparés par des virgules:");

    printf("Demande des indices des fichiers au client...\n");

    if(send_to_one_client("file",ask,socket_client)){
        perror("Erreur lors de l'envoi de la demande des indices des fichiers au client");
        return -1;
    }

    char buffer[MAX_FILES_LIST];
    int bytes_received = 0;

    printf("En attente de la réception des indices des fichiers...\n");

    memset(ask, 0, BUFFER_SIZE);
    strcat(ask, "NULL");

    if(send_to_one_client("askIndice",ask,socket_client)){
        perror("Erreur lors de l'envoi de la demande des indices des fichiers au client");
        return -1;
    }

    while (1) {

        memset(buffer, 0, MAX_FILES_LIST);

        bytes_received = recv(socket_client, buffer, MAX_FILES_LIST, 0);

        if (bytes_received < 0) {
            perror("Erreur lors de la réception des indices des fichiers");
            return -1;
        }

        printf("Indices reçus: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0){
            return 1;
        }

        if (check_valid_input(buffer,file_list)) {

            char Confirm_Msg[BUFFER_SIZE];
            memset(Confirm_Msg, 0, BUFFER_SIZE);

            strcat(Confirm_Msg, "OK");

            if (send_to_one_client("indice", Confirm_Msg, socket_client)) {
                perror("Erreur lors de l'envoi de la confirmation au client");
                return -1;
            }
            FileSendThreadArgs *args = malloc(sizeof(FileSendThreadArgs));

            if (args == NULL) {
                perror("Erreur lors de l'allocation de la mémoire pour les arguments du thread");
                return -1;
            }

            printf("buffer indice: %s\n", buffer);

            args->selected_indices = strdup(buffer);
            args->file_list = file_list;
            args->serveur_socket_file = serveur_socket_file;

            if (pthread_create(&send_files_thread, NULL, send_file_thread_func, (void *)args) != 0) {
                perror("Erreur lors de la création du thread d'envoi de fichiers");
                free(args->selected_indices);
                free(args);
                return -1;
            }

            // Vous pouvez utiliser pthread_detach si vous ne voulez pas attendre le thread avec pthread_join
            pthread_detach(send_files_thread);

            printf("Confirmation envoyée au client.\n");
            break;
        }
        else {
            printf("Indices invalides. Envoi d'un message d'erreur au client...\n");
            char buffer[BUFFER_SIZE];
            memset(buffer, 0, BUFFER_SIZE);
            strcat(buffer, "Indices invalides. Veuillez réessayer:");
            if(send_to_one_client("indice",buffer,socket_client)){
                perror("Erreur lors de l'envoi du message d'erreur au client");
                continue;
            }
            printf("Message d'erreur envoyé au client.\n");
        }
    }

    printf("Fichiers sélectionnés: %s\n", buffer);
}



int send_file_list(char *file_list[],int socket_client) {

    // Affichez la liste des fichiers disponibles
    for(int i = 0; i < MAX_FILES_LIST; i++) {
        if(file_list[i] != NULL) {
            char buffer[BUFFER_SIZE];
            sprintf(buffer, "%d: %s\n", i, file_list[i]);
            if(send_to_one_client("file",buffer,socket_client)){
                perror("Erreur lors de l'envoi de la liste des fichiers au client");
                return -1;
            }
        }
    }

    return 0;
}