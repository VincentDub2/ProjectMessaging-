#include "../../../include/client/handle_get_file/handle_get_files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>



#include "../../../include/common.h"

void* receive_files(void* arg) {

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


    // Réception du message de démarrage du transfert de fichier


    // Efface la ligne actuelle
    printf("\033[2K\r");

    // Réception des fichiers
    while (1) {
        char buffer[BUFFER_SIZE];
        int bytes_received;
        bytes_received = recv(socket_file, buffer, sizeof(buffer), 0);

        if (bytes_received == -1){
            perror("Erreur lors de la réception du message pour démarrer le transfert de fichier");
            break;
        }

        if (strcmp(buffer, "END") == 0) {
            break;
        }
        else if (strcmp(buffer, "READY") == 0) {

            char file_name[256];

            bytes_received = recv(socket_file, file_name, sizeof(file_name), 0);

            if (bytes_received == -1){
                perror("Erreur lors de la réception du nom du fichier");
                break;
            }


            printf("Nom du fichier reçu : %s\n", file_name);

            // Ouverture du fichier pour écriture
            char full_file_path[256];
            sprintf(full_file_path, "%s/%s", PATCH_CLIENT_FILE, file_name);

            int file_fd = open(full_file_path, O_WRONLY | O_CREAT, 0644);

            if (file_fd == -1) {
                perror("Erreur lors de l'ouverture du fichier");
                break;
            }

            // Réception de la taille du fichier
            int file_size;

            bytes_received = recv(socket_file, &file_size, sizeof(file_size), 0);

            if (bytes_received == -1){
                perror("Erreur lors de la réception de la taille du fichier");
                break;
            }

            printf("Taille du fichier reçue : %d\n", file_size);

            // Réception du fichier
            char buffer[BUFFER_SIZE];
            int remaining_bytes = file_size; // Nombre d'octets restant à lire

            while (remaining_bytes > 0) {
                bytes_received = recv(socket_file, buffer, min(BUFFER_SIZE, remaining_bytes), 0);
                if (bytes_received == -1){
                    perror("Erreur lors de la réception du fichier");
                    break;
                }
                write(file_fd, buffer, bytes_received);
                remaining_bytes -= bytes_received;
            }

            // Fermeture du fichier
            close(file_fd);

            char response[] = "READY";
            send(socket_file, response, sizeof(response), 0);
        }
    }
    printf("Fin de la réception des fichiers\n");
    printf("----------------------------------\n");
    printf("Entrez votre message: ");
    fflush(stdout);

    close(socket_file);
    pthread_exit(NULL);
}


void handle_get_files_command(int client_socket,int pipefd[2]) {

    pthread_t thread_receive_files;

    int file_get_selected = 1;
    int file_can_be_receive = 1;

    char selected_indices[BUFFER_SIZE];
    memset(selected_indices, 0, BUFFER_SIZE);

    // Efface la ligne actuelle
    printf("\033[2K\r");

    printf("----------------------------------\n");
    printf("Attente de la sélection de fichier...\n");

    while (file_get_selected) {
        fgets(selected_indices, BUFFER_SIZE, stdin);


        selected_indices[strcspn(selected_indices, "\n")] = 0;

        if (strcmp(selected_indices, "/quit") == 0) {
            printf("Sortie de la sélection de fichier.\n");
            file_get_selected = 1;
        }else {

            if (send(client_socket, selected_indices, BUFFER_SIZE, 0) < 0) {
                perror("Erreur lors de l'envoi de la sélection de fichier");
                break;
            }


            // Lecture des données du pipe
            char buffer[1024];
            ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
            if (bytes_read >= 0) {
                buffer[bytes_read] = '\0'; // Ajouter le caractère nul à la fin de la chaîne
            } else {
                perror("Erreur lors de la lecture du pipe");
            }

            if (strncmp(buffer, "OK", strlen("OK"))==0) {
                printf("Le serveur a accepté la sélection de fichier\n");
                file_can_be_receive = 0;
                file_get_selected = 0;
            } else if (strncmp(buffer, "NOK", strlen("NOK"))==0){
                printf("Le serveur a refusé la sélection de fichier\n");
                file_can_be_receive = 1;
                file_get_selected = 1;
            }
            else {
                printf("Error\n");
                file_can_be_receive = 1;
                file_get_selected = 1;
            }

        }
    }

    if (file_can_be_receive==0) {
        printf("Vous avez sélectionné les fichiers suivants:\n");
        printf("%s\n", selected_indices);

        //send(client_socket, selected_indices, strlen(selected_indices), 0);

        if (pthread_create(&thread_receive_files, NULL, receive_files, NULL) != 0) {
            perror("Erreur lors de la création du thread d'envoi de fichiers");
        }

        return;


        // return send_file_thread;
    }

}



