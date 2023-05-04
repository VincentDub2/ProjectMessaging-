#ifndef SIGNAL_UTILS_H
#define SIGNAL_UTILS_H

int get_client_running(void);
// pre : -
// post : retourne la valeur de la variable client_running

void set_client_running(int value);
// pre : value = 0 ou 1
// post : modifie la valeur de la variable client_running

void setup_sigint_handler(int server_socket);
// pre : server_socket valide
// post : met en place le handler pour SIGINT
// et ferme le serveur en cas de SIGINT
// et ferme le programme en cas d'erreur

#endif //SIGNAL_UTILS_H
