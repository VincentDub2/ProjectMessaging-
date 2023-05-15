#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../../include/serveur/socket_utils.h"

#include "../../include/serveur/signal_utilis.h"

#include "../../include/serveur/client_handler.h"

#include "../../include/serveur/serveur_utils.h"

#include "../../include/common.h"



int main() {


    struct sockaddr_in server_addr_message;

    struct sockaddr_in server_addr_file;

    // Créer un socket TCP et le configurer
    int server_socket_message = create_server_socket(&server_addr_message, PORT);

    int serveur_socket_file = create_server_socket(&server_addr_file,SERVER_PORT_FILE);

    // Configurer le gestionnaire de signal pour SIGINT (Ctrl+C)
    setup_sigint_handler(server_socket_message,serveur_socket_file);

    printf("Serveur en écoute sur le port %d\n", PORT);

    // Écoute les connexions entrantes
    if ((listen(server_socket_message, BACKLOG) == -1) || (listen(serveur_socket_file, BACKLOG) == -1)){
        perror("Erreur lors de l'écoute sur les sockets");
        close(server_socket_message);
        close(serveur_socket_file);
        exit(EXIT_FAILURE);
    }


    // Attendre les connexions des clients
    wait_for_clients(server_socket_message,serveur_socket_file);

    // Fermer le socket d'écoute du serveur
    clean_shutdown_server(server_socket_message,serveur_socket_file);


    return 0;
}


