//
// Created by vincent DUBUC on 03/05/2023.
//

#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H
int create_socket(void);
void connect_to_server(int socket_fd, char *ip, int port);
char* ask_for_pseudo(int socket_fd);

#endif //CLIENT_UTILS_H
