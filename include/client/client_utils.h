
#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

int create_socket(void);
// pre : /
// post : crée un socket et le retourne,
// elle vérifie les erreurs lors de la creation du socket

void connect_to_server(int socket_fd, char *ip, int port);
// pre : socket_fd valide / ip valide / port valide
// post : connecte le socket au serveur
// et affiche un message de connexion
// et ferme le programme en cas d'erreur

char* ask_for_pseudo(int socket_fd);
// pre : socket_fd valide
// post : demande un pseudo au client
// et le retourne si il est valide
//

#endif //CLIENT_UTILS_H
