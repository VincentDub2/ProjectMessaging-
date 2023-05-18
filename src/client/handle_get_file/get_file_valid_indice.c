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

#include "../../../include/common.h"

int get_file_valid_indice(int pipefd[2], int client_socket) {

    while (1) {

        char buffer[BUFFER_SIZE];
        // Réinitialise le tampon et reçoit un message du serveur
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        // Si la réception échoue, sortir de la boucle
        if (bytes_received == 0) {
            printf("Erreur de connection avec le serveur \n");
            char valid[] = "error";
            write(pipefd[1], valid, sizeof(valid));
            return -1;
        }

        // Recherche la position du caractère ':' qui sépare le protocole du message
        char *colon_position = strchr(buffer, ':');
        if (colon_position == NULL) {
            printf("Erreur: format de message invalide\n");
            char valid[] = "error";
            write(pipefd[1], valid, sizeof(valid));
            return -1;
        }

        // Extrayez le protocole et le message à partir de la chaîne d'origine
        int protocol_length = colon_position - buffer;

        char protocol[protocol_length + 1];

        strncpy(protocol, buffer, protocol_length);
        protocol[protocol_length] = '\0';

        const char *message = colon_position + 2;

        if(strcmp(protocol, "indice") == 0){
            if (strcmp(message, "OK") == 0) {
                char valid[] = "OK";
                write(pipefd[1], valid, sizeof(valid));
                return 0;

            } else {
                // Efface la ligne actuelle
                printf("\033[2K\r");
                printf("Veuillez entrer des indices valides\n");
                printf("Entrez votre message: ");
                char valid[] = "NOK";
                write(pipefd[1], valid, sizeof(valid));
            }
        }else if(strcmp(protocol, "error") == 0) {
            printf("Erreur: %s\n", message);
            return -1;
        }else if(strcmp(protocol, "fin") == 0) {
            printf("Fin du serveur\n");
            return 1;
        }
        else {
            char valid[] = "error";
            write(pipefd[1], valid, sizeof(valid));
            printf("Erreur: format de message invalide\n");
            return -1;
        }
    }
    return 0;
}