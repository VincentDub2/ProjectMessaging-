//Description : Fonctions pour gérer les signaux
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../include/serveur/signal_utilis.h"

#include "../../../include/serveur/serveur_utils/serveur_utils.h"

#include "../../../include/serveur/serveur_annoucement/client_event.h"

int server_socket_msg;
int server_socket_file;



void sigint_handler(int signum) {
    // Arrêter le serveur

    send_shutdown_message_to_clients();

    sleep(1);

    clean_shutdown_server(server_socket_msg, server_socket_file);

}

void setup_sigint_handler(int socket_msg, int socket_file) {
    server_socket_msg = socket_msg;
    server_socket_file = socket_file;
    struct sigaction sigint_action;
    memset(&sigint_action, 0, sizeof(sigint_action));
    sigint_action.sa_handler = sigint_handler;
    sigaction(SIGINT, &sigint_action, NULL);
}