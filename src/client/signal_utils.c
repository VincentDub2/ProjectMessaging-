#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>

#include "../../include/client/signal_utils.h"



int client_socket;

void sigint_handler(int signum) {

    // Indiquer que le serveur doit s'arrêter
    send(client_socket, "STOP", sizeof("STOP"), 0);

    exit(0);
}


void setup_sigint_handler(int socket) {
    client_socket = socket;
    struct sigaction sigint_action;
    memset(&sigint_action, 0, sizeof(sigint_action));
    sigint_action.sa_handler = sigint_handler;
    sigaction(SIGINT, &sigint_action, NULL);
}