//Description : Fonctions pour gérer les signaux
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../../include/serveur/signal_utilis.h"

#include "../../include/serveur/serveur_utils.h"

void sigint_handler(int signum);
void setup_sigint_handler(int server_socket);
int get_server_running(void);
void set_server_running(int value);




volatile sig_atomic_t server_running = 1; // Variable pour indiquer si le serveur est en cours d'exécution
int server_socket;

int get_server_running(void) {
    return server_running;
}

void set_server_running(int value) {
    server_running = value;
}

void sigint_handler(int signum) {
    // Indiquer que le serveur doit s'arrêter
    server_running = 0;

    clean_shutdown_server(server_socket);

    // Terminer le programme
    exit(0);
}

void setup_sigint_handler(int socket) {
    server_socket = socket;
    struct sigaction sigint_action;
    memset(&sigint_action, 0, sizeof(sigint_action));
    sigint_action.sa_handler = sigint_handler;
    sigaction(SIGINT, &sigint_action, NULL);
}