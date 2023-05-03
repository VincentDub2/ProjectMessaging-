#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../../include/serveur/socket_utils.h"

// Fonction qui permet de fermer le serveur proprement
//Ferme le socket serveur.
//Appelle shutdown_client_list() pour détruire le mutex et libérer la liste des clients
void shutdown_server(int server_socket);

int create_server_socket(struct sockaddr_in *server_addr, int port);


int create_server_socket(struct sockaddr_in *server_addr, int port) {
    // Créer un socket TCP
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    // Gestion d'erreur
    if (server_socket == -1) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Configurer l'adresse du serveur
    memset(server_addr, 0, sizeof(*server_addr));
    server_addr->sin_family = AF_INET; // IPv4
    server_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr->sin_port = htons(port);

    // Associer le socket avec l'adresse du serveur
    if (bind(server_socket, (struct sockaddr *)server_addr, sizeof(*server_addr)) == -1) {
        perror("Erreur lors de l'association du socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    return server_socket;
}

void shutdown_server(int server_socket) {
    close(server_socket);
    printf("Serveur correctement arrêté\n");
}

