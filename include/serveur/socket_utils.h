#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

int create_server_socket(struct sockaddr_in *server_addr, int port);
void shutdown_server(int server_socket);

#endif // SOCKET_UTILS_H
