#ifndef SIGNAL_UTILS_H
#define SIGNAL_UTILS_H

#include <signal.h>
// Contient la variable server_running qui permet de savoir si le serveur est en cours d'exécution
// pour les threads notamment
// la variable est volatile, car elle peut être modifiée par un thread et lue par un autre
// volatile permet de dire au compilateur que la variable peut être modifiée à tout moment meme quand elle est lu,
// c'est une variable commune

int get_server_running(void);
// pre : -
// post : retourne la valeur de server_running

void set_server_running(int value);
// pre : value = 0 ou 1
// post : modifie la valeur de server_running

void setup_sigint_handler(int server_socket_message, int server_socket_file);
//pre : server_socket valide
// post : met en place le handler pour SIGINT
// et ferme le serveur en cas de SIGINT
// et ferme le programme en cas d'erreur

#endif // SIGNAL_UTILS_H
