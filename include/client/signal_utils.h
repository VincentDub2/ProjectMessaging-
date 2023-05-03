//
// Created by vincent DUBUC on 03/05/2023.
//

#ifndef SIGNAL_UTILS_H
#define SIGNAL_UTILS_H
int get_client_running(void);
void set_client_running(int value);
void setup_sigint_handler(int server_socket);

#endif //SIGNAL_UTILS_H
