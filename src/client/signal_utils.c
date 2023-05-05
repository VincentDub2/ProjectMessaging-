//
// Created by vincent DUBUC on 03/05/2023.
//
//Description : Fonctions pour gérer les signaux
#include <signal.h>
#include <string.h>

#include "../../include/client/signal_utils.h"

volatile sig_atomic_t client_running= 1; // Variable pour indiquer si le serveur est en cours d'exécution

int client_socket;

int get_client_running(void) {
    return client_running;
}

void set_client_running(int value) {
    client_running = value;
}

void sigint_handler(int signum) {
    // Indiquer que le serveur doit s'arrêter
    client_running = 0;

}

void setup_sigint_handler(int socket) {
    client_socket = socket;
    struct sigaction sigint_action;
    memset(&sigint_action, 0, sizeof(sigint_action));
    sigint_action.sa_handler = sigint_handler;
    sigaction(SIGINT, &sigint_action, NULL);
}