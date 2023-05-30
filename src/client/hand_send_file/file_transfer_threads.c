#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../../../include/client/handle_send_file/file_transfer_threads.h"

#include "../../../include/file_utils.h"

// Cette fonction envoie les fichiers sélectionnés au serveur
void *send_file_thread_func(void *args) {
    FileSendThreadArgs* thread_args = (FileSendThreadArgs*) args;

    int socket_file = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_file < 0) {
        perror("Erreur lors de la création du socket");
        pthread_exit(NULL);
    }

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT_FILE);

    if (connect(socket_file, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Erreur de connexion au serveur avec le port spécifié %d et l'adresse IP spécifiée %s \n", SERVER_PORT_FILE, SERVER_IP);
        perror("Info  ");
        exit(EXIT_FAILURE);
    }


    // Envoyez les fichiers sélectionnés
    printf("Vous avez sélectionné les fichiers suivants:\n");
    printf("%s\n", thread_args->selected_indices);

    char **file_list = thread_args->file_list;



    char *saveptr;
     char *token = strtok_r(thread_args->selected_indices, ",", &saveptr);

    char response[] = "READY";
    if (send(socket_file, response, sizeof(response), 0)==-1){
        perror("Erreur lors de l'envoi de la réponse READY");
        close(socket_file);
        exit(EXIT_FAILURE);
    }

    while(token != NULL) {
         char *endptr;
       int file_index = strtol(token, &endptr, 10);

       if (endptr == token) {
            perror("Erreur lors de la conversion de l'index du fichier en entier");
        } else {


        char* file_name = file_list[file_index];

        printf("Le nom fichier est : %s\n",file_name);


        if(file_name != NULL) {
            char full_file_path[256];

            memset(full_file_path,0,256);

            sprintf(full_file_path, "%s/%s", PATCH_CLIENT_FILE, file_name);

            //printf("Envoi du fichier %s\n", full_file_path);

            // Envoyez le fichier
            send_file(full_file_path, socket_file, file_name);


        }

         token = strtok_r(NULL, ",", &saveptr);

        if (token != NULL) {
            // Indicate that we are ready to send the next file
            char response[] = "READY";
            send(socket_file, response, sizeof(response), 0);
        }
        }
    }

    char end[] = "END";
    send(socket_file, end, sizeof(end), 0);

    close(socket_file);

    // Libérez la mémoire allouée pour les arguments du thread
    free(thread_args->selected_indices);

    for (int i = 0; i < MAX_FILES_LIST && file_list[i] != NULL; i++) {
        free(file_list[i]);
    }

    free(thread_args->file_list);
    free(thread_args);

    printf("Le transfert de fichier est terminé\n");
    printf("----------------------------------\n");
    printf("Entrée un message :");
    fflush(stdout);

    pthread_exit(NULL);
}