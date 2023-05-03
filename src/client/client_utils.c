#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../../include/client/client_utils.h"
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
