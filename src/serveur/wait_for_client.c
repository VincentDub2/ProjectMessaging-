#include "../../include/serveur/wait_for_client.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdlib.h>
#include "../../include/serveur/serveur_utils/rk_sema.h"
#include "../../include/serveur/serveur_utils/serveur_utils.h"

#include "../../include/serveur/client_handler.h"
#include "../../include/serveur/serveur_utils/handle_error_thread.h"

void wait_for_clients(int server_socket_message, int server_socket_file) {

    // Ajouter le compte serveur à la liste des clients
    add_serveur_account();

    int client_count = 1;

    // Initialiser le sémaphore
    initialize_semaphore();

    pthread_t thread_id; //Identifiant du thread

    while (1) {

        // Avant d'accepter un nouveau client, attendre qu'un slot soit disponible
        rk_sema_wait(get_client_slot());

        struct sockaddr_in client_addr; //adresse du client
        socklen_t client_addr_size = sizeof(client_addr); //taille de l'adresse du client

        // Accepter la connexion entrante
        int client_socket = accept(server_socket_message, (struct sockaddr *)&client_addr, &client_addr_size);

        printf("Connexion acceptée\n");
        //Gestion d'erreur
        if (client_socket == -1) {
            perror("Erreur lors de l'acceptation de la connexion entrante");
            continue; //On signale l'erreur et on continue l'écoute
        }

        client_info *info = malloc(sizeof(client_info));

        if (info == NULL) {
            perror("Erreur lors de l'allocation de mémoire pour info");
            exit(EXIT_FAILURE);
        }

        if (info== NULL) {
            perror("Erreur lors de l'allocation de mémoire");
            close(client_socket);
            continue; //On signale l'erreur et on continue l'écoute
        }

        //On remplit les champs de la structure Info pour le stocker dans la liste chaînée
        info->socket_fd = client_socket;
        info->index = client_count++;
        info->next = NULL;
        info->thread_id = thread_id;
        info->pseudo = NULL;

        add_client_to_list(info);

        thread_args *args = malloc(sizeof(thread_args));
        args->server_socket_message = server_socket_message;
        args->server_socket_file = server_socket_file;
        args->info = info;

        // Passer la structure args à pthread_create
        if (pthread_create(&thread_id, NULL, handle_client, args) != 0) {
            perror("Erreur lors de la création du thread");
            close(client_socket);
            remove_client_from_list(info->index);
            free(args);
            continue;
        }

        pthread_t waiter_thread_id;

        thread_waiter_args* tw_args = malloc(sizeof(thread_waiter_args));
        tw_args->thread_to_wait = thread_id;

        //Thread qui attendra la fin du thread client
        if (pthread_create(&waiter_thread_id, NULL, thread_waiter, tw_args) != 0) {
            perror("Erreur lors de la création du thread d'attente");
            free(tw_args);
        }
    }
}
