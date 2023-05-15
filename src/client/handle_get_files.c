#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>


#include "../../include/file_utils.h"
#include "../../include/client/signal_utils.h"
#include "../../include/common.h"
#include "../../include/client/client_utils.h"


void* receive_files(void* arg) {

    int socket_file = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_file < 0) {
        perror("Erreur lors de la création du socket");
        pthread_exit(NULL);
    }

    printf("Socket de réception de fichiers créé\n");

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

    // On suppose que le socket client est global et appelé client_socket

    printf("Thread de réception de fichiers créé\n");

    // Réception des fichiers
    while (get_client_running()) {
        char buffer[BUFFER_SIZE];
        recv(socket_file, buffer, sizeof(buffer), 0);
        printf("buffer : %s\n", buffer);

        if (strcmp(buffer, "END") == 0) {
            break;
        }
        else if (strcmp(buffer, "READY") == 0) {

            char file_name[256];
            int bytes_received;
            do {
                bytes_received = recv(socket_file, file_name, sizeof(file_name), 0);
                if (bytes_received == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                    usleep(200000);
                    continue;
                } else if (bytes_received <= 0) {
                    perror("Erreur lors de la réception du nom du fichier");
                    break;
                }
            } while (bytes_received == -1);

            printf("Nom du fichier reçu : %s\n", file_name);

            // Ouverture du fichier pour écriture
            char full_file_path[256];
            sprintf(full_file_path, "%s/%s", PATCH_CLIENT_FILE, file_name);

            int file_fd = open(full_file_path, O_WRONLY | O_CREAT, 0644);
            if (file_fd == -1) {
                perror("Erreur lors de l'ouverture du fichier");
                // ou traiter l'erreur comme vous le souhaitez
            }

            // Réception de la taille du fichier
            int file_size;
            do {
                bytes_received = recv(socket_file, &file_size, sizeof(file_size), 0);
                if (bytes_received == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                    usleep(200000);
                    continue;
                } else if (bytes_received <= 0) {
                    perror("Erreur lors de la réception de la taille du fichier");
                    break;
                }
            } while (bytes_received == -1);

            printf("Taille du fichier reçue : %d\n", file_size);

            // Réception du fichier
            char buffer[BUFFER_SIZE];
            int remaining_bytes = file_size; // Nombre d'octets restant à lire

            while (remaining_bytes > 0) {
                bytes_received = recv(socket_file, buffer, min(BUFFER_SIZE, remaining_bytes), 0);
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

            printf("Fichier reçu\n");

            printf("file received\n");
            char response[] = "READY";
            send(socket_file, response, sizeof(response), 0);
        }
    }
    close(socket_file);
    pthread_exit(NULL);
}


void handle_get_files_command(int client_socket) {
    pthread_t thread_receive_files;  // Renommé pour éviter la confusion

    int file_get_selected = 0;
    int file_can_be_receive = 0;

    char selected_indices[BUFFER_SIZE];
    memset(selected_indices, 0, BUFFER_SIZE);

    printf("Attente de la sélection de fichier...\n");

    while (!file_get_selected && get_client_running()) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int select_result = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);

        if (select_result > 0 && FD_ISSET(STDIN_FILENO, &read_fds)) {
            fgets(selected_indices, BUFFER_SIZE, stdin);

            selected_indices[strcspn(selected_indices, "\n")] = 0;

            if (strcmp(selected_indices, "/quit") == 0) {
                printf("Sortie de la sélection de fichier.\n");
                file_get_selected = 1;
            } else {
                printf("Boucle : Vous avez sélectionné les fichiers suivants: %s\n", selected_indices);

                if (send(client_socket, selected_indices,BUFFER_SIZE, 0) < 0) {
                    perror("Erreur lors de l'envoi de la sélection de fichier");
                    continue;
                }


                printf("Sélection de fichier envoyée.\n");

                char buffer[BUFFER_SIZE];
                memset(buffer, 0, BUFFER_SIZE);

                printf("Attente de la réponse du serveur...\n");

                fd_set read_fds;
                FD_ZERO(&read_fds);
                FD_SET(client_socket, &read_fds);

                struct timeval timeout;
                timeout.tv_sec = 2;  // Set timeout values as needed
                timeout.tv_usec = 0;

                int select_result = select(client_socket + 1, &read_fds, NULL, NULL, &timeout);

                printf("Select result: %d\n", select_result);

                if (select_result > 0 && FD_ISSET(client_socket, &read_fds)) {

                    int recv_result = recv(client_socket, buffer, BUFFER_SIZE, 0);

                    printf("Réponse du serveur reçue %d\n",recv_result);

                    printf("Réponse du serveur: %s\n", buffer);

                    if (recv_result < 0) {
                        perror("Erreur lors de la réception de la réponse du serveur");
                        continue;
                    }

                    printf("Réponse du serveur: %s\n", buffer);

                    if (strcmp(buffer, "OK") == 0) {
                        file_can_be_receive = 1;
                        file_get_selected = 1;
                    } else {
                        printf("Erreur lors de la sélection de fichier.\n");
                    }
                } else if (select_result == -1) {
                    perror("Erreur lors de l'appel à select()");
                    set_client_running(0);
                }
            }
        } else if (select_result == -1) {
            perror("Erreur lors de l'appel à select()");
            set_client_running(0);
        } else {
            selected_indices[0] = '\0';
        }
    }

    if (file_can_be_receive && get_client_running()) {
        printf("Vous avez sélectionné les fichiers suivants:\n");
        printf("%s\n", selected_indices);

        send(client_socket, selected_indices, strlen(selected_indices), 0);

        printf("Indices de fichiers envoyés au serveur: %s\n", selected_indices);

        if (pthread_create(&thread_receive_files, NULL, receive_files, NULL) != 0) {
            perror("Erreur lors de la création du thread d'envoi de fichiers");
        }

        // return send_file_thread;
    }

}