#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../../include/client/client_utils.h"

#include "../../include/common.h"

#include <stdio.h>


// Crée un socket et vérifie si la création a réussi
int create_socket() {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }
    return socket_fd;
}

// Se connecte au serveur en utilisant l'adresse IP et le port définis
void connect_to_server(int socket_fd, char *ip, int port) {
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur de connexion au serveur");
        exit(EXIT_FAILURE);
    }
}


int is_valid_pseudo(const char* pseudo) {
    size_t length = strlen(pseudo);

    if (length == 0 || length > MAX_PSEUDO_LENGTH) {
        return 0;
    }

    for (size_t i = 0; i < length; i++) {
        if (pseudo[i] == ' ') {
            return 1;
        }
    }

    return 1;
}

char* ask_for_pseudo(int socket_fd) {
    int pseudo_accepted = 0;
    char* buffer = (char*)malloc(BUFFER_SIZE * sizeof(char));

    while (!pseudo_accepted) {
        printf("Veuillez entrer un pseudo (maximum %d caractères, sans espaces): ",MAX_PSEUDO_LENGTH);
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

        if (is_valid_pseudo(buffer)) {
            send(socket_fd, buffer, strlen(buffer), 0);

            char server_response[BUFFER_SIZE];
            recv(socket_fd, server_response, BUFFER_SIZE, 0);

            if (strcmp(server_response, "PSEUDO_OK") == 0) {
                pseudo_accepted = 1;
            }
            else if (strcmp(server_response,"PSEUDO_TAKEN") == 0){
                printf("Le pseudo est déjà pris, veuillez en choisir un autre.\n");
            }
            else {
                printf("Erreur inattendue avec le serveur.\n");
                exit(EXIT_FAILURE);
            }
        } else {
            printf("Le pseudo est invalide. Veuillez entrer un pseudo sans espaces et de maximum %d caractères.\n", MAX_PSEUDO_LENGTH);
        }
    }

    return buffer;
}