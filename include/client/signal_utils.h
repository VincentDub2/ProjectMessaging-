#ifndef SIGNAL_UTILS_H
#define SIGNAL_UTILS_H

void setup_sigint_handler(int server_socket);
// pre : server_socket valide
// post : met en place le handler pour SIGINT
// et ferme le serveur en cas de SIGINT
// et ferme le programme en cas d'erreur

#endif //SIGNAL_UTILS_H
