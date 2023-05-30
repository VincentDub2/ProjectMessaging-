
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <ctype.h>



#include <dirent.h>



#include "../include/file_utils.h"
#include "../include/common.h"

#define SIZE 1024



void send_file(const char *patch_file_name, int sockfd,const char *file_name){

    FILE *fp = fopen(patch_file_name, "rb");
    if (fp == NULL) {
        perror("[-]Error in reading file.");
        exit(1);
    }


    printf("File name : %s",file_name);
    if (send(sockfd, file_name, strlen(file_name) + 1, 0) <= 0) {
        perror("[-]Error in sending file name.");
        exit(1);
    }
  
    // Get file size
    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET); // send file from beginning

    // Send file size
    if (send(sockfd, &file_size, sizeof(file_size), 0) == -1) {
        perror("[-]Error in sending file size.");
        exit(1);
    }

    // Send file content
    char data[SIZE] = {0};
    ssize_t n;
    while((n = fread(data, sizeof(char), SIZE, fp)) > 0) {
        if(ferror(fp)){
            perror("[-]Error in reading file.");
            exit(1);
        }

        char *ptr = data;
        while (n > 0) {
            ssize_t sent = send(sockfd, ptr, n, 0);
            if(sent < 0){
                perror("[-]Error in sending file.");
                exit(1);
            }

            n -= sent;
            ptr += sent;
        }

        memset(data, 0, SIZE);
    }

    fclose(fp);

    struct stat st;

    stat(patch_file_name, &st);
    int size = st.st_size;

    if (size != file_size) {
        perror("Erreur: la taille du fichier reçu ne correspond pas à la taille attendue");
        // renvoyer une erreur au client
    } else {
        printf("Le fichier a été reçu avec succès\n");
        // envoyer une confirmation au client
    }

    char buffer[SIZE];

    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("[+]File data sent successfully. %s\n", buffer);
    if (strcmp(buffer, "READY") != 0) {
        fprintf(stderr, "[-]Error: server or client is not ready for the next file.\n");
        exit(1);
    }
}



int check_valid_input(char *selected_indices, char *file_list[]) {
    int valid_input = 1;

    for(int i = 0; i < strlen(selected_indices); i++) {
        if(selected_indices[i] != ',' && !isdigit(selected_indices[i])) {
            valid_input = 0;
            break;
        }
    }

    if(valid_input) {
        char selected_indices_copy[100];
        strcpy(selected_indices_copy, selected_indices);

        char *token = strtok(selected_indices_copy, ",");

        while(token != NULL) {
            char *endptr;
            long num = strtol(token, &endptr, 10);

            if (*endptr != '\0' || num < 0 || num >= MAX_FILES_LIST || file_list[num] == NULL) {
                printf("Entrée invalide. Le nombre doit être un index de fichier valide.\n");
                return 0;
            }
            token = strtok(NULL, ",");
        }

        if(token == NULL) {
            return 1;
        }
    }
    else {
        printf("Entrée invalide. Veuillez entrer uniquement des chiffres séparés par des virgules.\n");
    }

    return 0;
}


void free_file_list(char *file_list[]) {
    for(int i = 0; i < MAX_FILES_LIST; i++) {
        if(file_list[i] != NULL) {
            free(file_list[i]);
        }
    }
}


// Cette fonction liste tous les fichiers dans un répertoire spécifié
void list_files_in_directory(char *file_list[],const char *patch_file ) {
    struct dirent *de;  // Pointer pour le répertoire
    DIR *dr = opendir(patch_file);
    int i = 0;

    if (dr == NULL) {  // opendir renvoie NULL si ne peut pas ouvrir le répertoire
        printf("Le fichier n'a pas pu être ouvert" );
        return;
    }

    // pour chaque fichier dans le répertoire
    while ((de = readdir(dr)) != NULL && i < MAX_FILES_LIST) {
        if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
            file_list[i] = strdup(de->d_name);  // Copiez le nom du fichier dans le tas
            i++;
        }
    }
    closedir(dr); //ferme le répertoire
}


