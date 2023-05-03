#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <pthread.h>
#include "client_list.h"

void wait_for_clients(int server_socket);
void *handle_client(void *arg);
void disconnect_client(client_info *info);

#endif // CLIENT_HANDLER_H
