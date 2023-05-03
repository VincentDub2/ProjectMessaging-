#ifndef SIGNAL_UTILS_H
#define SIGNAL_UTILS_H

#include <signal.h>

// Prototypes des fonctions

int get_server_running(void);
void set_server_running(int value);
void setup_sigint_handler(int server_socket);

#endif // SIGNAL_UTILS_H
