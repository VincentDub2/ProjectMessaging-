#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

int create_server_socket(struct sockaddr_in *server_addr, int port);
// pre : server_addr valide / port valide
// post : crée un socket serveur et le retourne,
// elle vérifie les erreurs lors de la creation du socket

void shutdown_server(int server_socket_message, int server_socket_file);
// pre : server_socket valide
// post : ferme le socket serveur
// et affiche un message de fermeture
// et ferme le programme

#endif // SOCKET_UTILS_H
