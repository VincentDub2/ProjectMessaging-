// Date: 05/04/22
#ifndef SERVEUR_UTILS_H
#define SERVEUR_UTILS_H

void force_logout_clients();
//pre : -
//post : force la déconnexion de tous les clients
// et la fin des threads associés

void clean_logout_clients();
//pre : -
//post : déconnecte proprement tous les clients
// et attend que tous les threads soit terminés

void clean_shutdown_server(int serveur_socket_message,int serveur_socket_file);
// pre : serveur_socket valide
// post : appelle clean_logout_clients
// libère le sémaphore client_slot
// détruit la liste des clients
// ferme le serveur

void destroy_semaphore(void);
// pre : -
// post : détruit le sémaphore client_slot

void initialize_semaphore(void);
// pre : -
// post : initialise le sémaphore client_slot

struct rk_sema* get_client_slot(void);
// pre : -
// post : retourne un pointeur vers le sémaphore client_slot

#endif //SERVEUR_UTILS_H
