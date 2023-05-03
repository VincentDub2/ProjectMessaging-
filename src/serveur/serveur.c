#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../../include/serveur/socket_utils.h"

#include "../../include/serveur/signal_utilis.h"

#include "../../include/serveur/client_handler.h"

#include "../../include/serveur/serveur_utils.h"

#define PORT 8080          // Port d'écoute du serveur
#define BACKLOG 5 // Longueur de la file d'attente pour les connexions entrantes

// printf("\033[A\023[2K"); // Effacer ligne précédente




int main() {


    struct sockaddr_in server_addr;

    // Créer un socket TCP et le configurer
    int server_socket = create_server_socket(&server_addr, PORT);

    // Configurer le gestionnaire de signal pour SIGINT (Ctrl+C)
    setup_sigint_handler(server_socket);

    printf("Serveur en écoute sur le port %d\n", PORT);

    // Écoute les connexions entrantes
    if (listen(server_socket, BACKLOG) == -1) {
        perror("Erreur lors de l'écoute des connexions entrantes");
        close(server_socket);
        exit(EXIT_FAILURE);
    }


    // Attendre les connexions des clients
    wait_for_clients(server_socket);

    // Fermer le socket d'écoute du serveur
    clean_shutdown_server(server_socket);


    return 0;
}


