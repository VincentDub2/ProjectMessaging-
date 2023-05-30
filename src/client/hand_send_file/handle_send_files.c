#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

#include "../../../include/client/handle_send_file/file_transfer_threads.h"

#include "../../../include/file_utils.h"


#include "../../../include/client/handle_send_file/handle_send_files.h"


// Cette fonction envoie les fichiers sélectionnés au serveur


void display_files(char *file_list[]) {
    // Efface la ligne actuelle
    printf("\033[2K\r");

    // Affichez la liste des fichiers disponibles
    for(int i = 0; i < MAX_FILES_LIST; i++) {
        if(file_list[i] != NULL) {
            printf("%d: %s\n", i, file_list[i]);
        }
    }
}


void handle_send_files_command() {

    pthread_t send_file_thread;

    char **file_list = malloc(MAX_FILES_LIST * sizeof(char *));

    if (!file_list) {
        perror("Erreur d'allocation pour file_list");
        return -1;
    }

    list_files_in_directory(file_list,PATCH_CLIENT_FILE);

    display_files(file_list);
    printf("----------------------------------\n");

    printf("Entrez les indices des fichiers à envoyer, séparés par des virgules:\n");

    int file_get_selected = 0;
    int file_can_be_sent = 0;

    char selected_indices[100];

    while (!file_get_selected){

        fgets(selected_indices, 100, stdin);
        selected_indices[strcspn(selected_indices, "\n")] = 0;

        if(strcmp(selected_indices,"/quit") == 0) {
            printf("Sortie de la sélection de fichier.\n");
            file_get_selected = 1;
        }
        else {
            if(check_valid_input(selected_indices, file_list)) {
                file_get_selected = 1;
                file_can_be_sent = 1;
            }
        }
    }

    if (file_can_be_sent) {
        /*
        printf("Vous avez sélectionné les fichiers suivants:\n");
        printf("%s\n",selected_indices);
         */

        FileSendThreadArgs* args = malloc(sizeof(FileSendThreadArgs));
        args->file_list = file_list;
        strcpy(args->selected_indices, selected_indices);

        //printf("%s",args->selected_indices);

        if (pthread_create(&send_file_thread, NULL, send_file_thread_func, args) != 0) {
            perror("Erreur lors de la création du thread d'envoi de fichiers");
        }

        // return send_file_thread;

    }

    // Libérez la mémoire allouée pour les noms de fichiers
    //free_file_list(file_list);

    // return send_file_thread;

}