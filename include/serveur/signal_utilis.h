#ifndef SIGNAL_UTILS_H
#define SIGNAL_UTILS_H

#include <signal.h>


void setup_sigint_handler(int server_socket_message, int server_socket_file);
//pre : server_socket valide
// post : met en place le handler pour SIGINT
// et ferme le serveur en cas de SIGINT
// et ferme le programme en cas d'erreur

#endif // SIGNAL_UTILS_H
