#include "../../../include/serveur/serveur_utils/serveur_utils.h"
#include "../../../include/serveur/serveur_utils/client_list.h"
#include "../../../include/serveur/serveur_utils/socket_utils.h"


#include "../../../include/common.h"

#include <sys/sem.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../../include/serveur/serveur_utils/rk_sema.h"

struct rk_sema client_slot;


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


// Fonction pour arrêter le serveur
// On ferme toutes les connexions et on libère la mémoire
void clean_shutdown_server(int serveur_socket_message,int serveur_socket_file) {

    shutdown_client_list();

    shutdown_server(serveur_socket_message, serveur_socket_file);

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