//
// Created by vincent DUBUC on 02/05/2023.
//

#ifndef SERVEUR_UTILS_H
#define SERVEUR_UTILS_H

void force_logout_clients();
void clean_logout_clients();
void clean_shutdown_server(int serveur_socket);
void destroy_semaphore(void);
void initialize_semaphore(void);
struct rk_sema* get_client_slot(void);
#endif //SERVEUR_UTILS_H
