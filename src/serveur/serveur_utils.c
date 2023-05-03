#include "../../include/serveur/serveur_utils.h"
#include "../../include/serveur/client_list.h"
#include "../../include/serveur/signal_utilis.h"
#include "../../include/serveur/socket_utils.h"
#include "../../include/serveur/communication_utils.h"
#include "../include/serveur/common.h"
#include <sys/sem.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../include/serveur/rk_sema.h"

struct rk_sema client_slot;


//Pre : None
//Post : Change la valeur de server_running à 0
// Puis attend la fermeture de tous les threads et les sockets des clients
void clean_logout_server(int serveur_socket);

//pre : None
//post : force la fermeture de tous les threads et les sockets des clients
void force_logout_clients();



//Pre :
//Post : déconnecte tous les clients et ferme le serveur
// Efface la liste des clients
void clean_shutdown_server(int serveur_socket);

void initialize_semaphore(void);

void destroy_semaphore(void);

// Retourne un pointeur vers le sémaphore client_slot
struct rk_sema* get_client_slot(void);

void force_logout_clients() {

    pthread_mutex_t *mutex = get_mutex_list();
    pthread_mutex_lock(mutex);

    client_info *curr = get_head_list();

    client_info *next;
    while (curr != NULL) {
        next = curr->next;
        close(curr->socket_fd);
        pthread_cancel(curr->thread_id);
        curr = next;
    }

    pthread_mutex_unlock(mutex);
}

void clean_logout_clients(){
    send_fin();


    set_server_running(0);

    printf("En attente de la déconnexion des clients...\n");


    client_info *curr = get_head_list();

    while (curr != NULL) {
        pthread_join(curr->thread_id, NULL);
        close(curr->socket_fd);
        curr = curr->next;
    }


    printf("Tous les clients sont déconnectés.\n");

}


// Fonction pour arrêter le serveur
// On ferme toutes les connexions et on libère la mémoire
void clean_shutdown_server(int serveur_socket) {
    clean_logout_clients();

    shutdown_client_list();

    shutdown_server(serveur_socket);

    destroy_semaphore();

    exit(EXIT_SUCCESS);
}

void initialize_semaphore() {
    rk_sema_init(&client_slot, MAX_CLIENTS);
}

void destroy_semaphore() {
    rk_sema_destroy(&client_slot);
}

struct rk_sema* get_client_slot() {
    return &client_slot;
}