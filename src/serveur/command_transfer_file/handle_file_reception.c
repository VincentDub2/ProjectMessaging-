#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "../../../include/serveur/command_transfer_file/handle_file_reception.h"
#include "../../../include/common.h"
#include "../../../include/serveur/signal_utilis.h"
#include "../../../include/serveur/serveur_utils/socket_utils.h"



void* handle_file_reception(void* arg) {
    int client_socket = *((int*) arg);

    printf("create thread\n");

    // Réception du nom du fichier
    while (1) { //get(server_running)

        char buffer[BUFFER_SIZE];
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("buffer : %s\n", buffer);
        if (strcmp(buffer, "END") == 0) {
            break;
        }
        else if (strcmp(buffer, "READY") == 0) {

            char file_name[256];
            int bytes_received;
            do {
                bytes_received = recv(client_socket, file_name, sizeof(file_name), 0);
                if (bytes_received == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                    usleep(200000);
                    continue;
                } else if (bytes_received <= 0) {
                    perror("Erreur lors de la réception du nom du fichier");
                    break;
                }
            } while (bytes_received == -1);

            printf("file name received : %s \n", file_name);
            // Ouverture du fichier pour écriture
            char full_file_path[256];
            sprintf(full_file_path, "%s/%s",PATCH_SERVEUR_FILE, file_name);

            int file_fd = open(full_file_path, O_WRONLY | O_CREAT, 0644);
            if (file_fd == -1) {
                perror("Erreur lors de l'ouverture du fichier");
                // ou traiter l'erreur comme vous le souhaitez
            }

            // ...

            // Réception de la taille du fichier
            int file_size;
            do {
                bytes_received = recv(client_socket, &file_size, sizeof(file_size), 0);
                if (bytes_received == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                    usleep(200000);
                    continue;
                } else if (bytes_received <= 0) {
                    perror("Erreur lors de la réception de la taille du fichier");
                    break;
                }
            } while (bytes_received == -1);

            printf("file size received : %d \n", file_size);
            // ...

            // Reception du fichier
            char buffer[BUFFER_SIZE];
            int remaining_bytes = file_size; // Nombre d'octets restant à lire


            // Le min permet de recevoir le fichier par morceaux
            // et d'éviter de recevoir plus que la taille du fichier quand on arrive à la fin
            while (remaining_bytes > 0) {
                bytes_received = recv(client_socket, buffer, min(BUFFER_SIZE, remaining_bytes), 0);
                if (bytes_received == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                    usleep(200000);
                    continue;
                } else if (bytes_received <= 0) {
                    perror("Erreur lors de la réception du fichier");
                    break;
                }
                write(file_fd, buffer, bytes_received);
                remaining_bytes -= bytes_received;
            }

            // Fermeture du fichier
            close(file_fd);

            printf("file received\n");
            char response[] = "READY";
            send(client_socket, response, sizeof(response), 0);
        }
    }


    // Votre code pour la réception de fichiers ici

    close(client_socket);
    pthread_exit(NULL);
}


void handle_file_reception_command(int serveur_socket_file) {
    printf("Envoie de fichier\n");


    struct sockaddr_in client_addr; //adresse du client
    socklen_t client_addr_size = sizeof(client_addr); //taille de l'adresse du client

    // Accepter la connexion entrante
    int client_socket_file = accept(serveur_socket_file, (struct sockaddr *)&client_addr, &client_addr_size);

    if (client_socket_file == -1) {
        perror("Erreur lors de l'acceptation de la connexion entrante");
        //On signale l'erreur et on continue l'écoute
    } else {

        // Création d'un nouveau thread pour gérer la réception de fichiers
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_file_reception, &client_socket_file) != 0) {
            perror("Erreur lors de la création du thread");
        }
    }
}
