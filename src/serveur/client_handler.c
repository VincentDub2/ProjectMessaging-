#include "../include/serveur/client_handler.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "../../include/serveur/communication_utils.h"

#include "../../include/serveur/rk_sema.h"

#include "../include/common.h"

#include "../include/serveur/handle_pseudo.h"

#include "../../include/serveur/handle_file_reception.h"

#include "../../include/serveur/serveur_utils.h"

#include "../../include/serveur/handle_file_send.h"

#include "../../include/serveur/signal_utilis.h"

#include "../../include/serveur/socket_utils.h"

#include "../../include/serveur/client_list.h"

void wait_for_clients(int server_socket_message, int server_socket_file) {

    // Ajouter le compte serveur à la liste des clients
    add_serveur_account();

    int client_count = 1;

    // Initialiser le sémaphore
    initialize_semaphore();

    pthread_t thread_id; //Identifiant du thread

    while (get_server_running()) {

        // Avant d'accepter un nouveau client, attendre qu'un slot soit disponible
        rk_sema_wait(get_client_slot());

        struct sockaddr_in client_addr; //adresse du client
        socklen_t client_addr_size = sizeof(client_addr); //taille de l'adresse du client

        // Accepter la connexion entrante
        int client_socket = accept(server_socket_message, (struct sockaddr *)&client_addr, &client_addr_size);
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
            free(args); // N'oubliez pas de libérer la mémoire si la création du thread échoue
            continue;
        }
    }
}


void *handle_client(void *arg) {

    thread_args *args = (thread_args *) arg;

    int server_socket_message = args->server_socket_message; // PAS UTILISER POUR L'INSTANT
    int server_socket_file = args->server_socket_file;
    client_info *info = args->info;


    int client_socket = info->socket_fd;
    int client_index = info->index;

    printf("Client %d choisi un pseudo \n", client_index);

    char pseudo_buffer[MAX_PSEUDO_LENGTH + 1]; // +1 pour le caractère de fin de chaîne

    receive_pseudo_from_client(pseudo_buffer, client_socket);

    info->pseudo = strdup(pseudo_buffer); // Copier le pseudo dans la structure client_info

    // Envoie du message "x a rejoint le chat" à tous les clients
    send_welcome_message_to_clients(info->pseudo);

    // Configurer le socket en mode non bloquant
    int flags = fcntl(client_socket, F_GETFL, 0);

    if (flags == -1) {
        perror("Erreur lors de l'obtention des flags du socket");
        pthread_exit(NULL);
    }

    if (fcntl(client_socket, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("Erreur lors de la configuration du socket en mode non bloquant");
        pthread_exit(NULL);
    }


    char buffer[BUFFER_SIZE];
    int bytes_received;

    printf("Client %d est connecté\n", client_index);

    while (get_server_running()) {

        // Réinitialiser le tampon
        memset(buffer, 0, BUFFER_SIZE);

        // Recevoir un message du client
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        // Si recv() retourne -1 et que errno est EAGAIN ou EWOULDBLOCK, continuez la boucle
        // Cela signifie qu'il n'y a pas de données à lire
        if (bytes_received == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            usleep(200000); // Attendre 200 ms pour éviter une utilisation élevée du processeur

            continue; // Permet de passer directement à la prochaine itération de la boucle
        }


        // Si la réception échoue sortir de la boucle
        if (bytes_received == 0) {
            printf("Erreur de connection avec le client %d \n", client_index);
            break;
        }

        // Gestion des commandes
        if (buffer[0] == '/'){
            // Extraire la commande
            char *commande = strtok(buffer, " ");
            commande++; // Ignorer le caractère '/' dans la commande
            // Traiter la commande
            if (strcmp(commande, "list") == 0) {
                send_client_list(pseudo_buffer);

            }
            else if(strcmp(commande,"sendFiles")==0) {
                handle_file_reception_command(server_socket_file);
            }
            else if (strcmp(commande, "getFiles")==0){
                printf("getFiles");
                handle_get_files_commande(client_socket,pseudo_buffer,server_socket_file);
            }

            else if (strcmp(commande, "quit") == 0) {
                // Déconnecter le client
                break;
            }
            else if (strcmp(commande, "help") == 0){
                send_manual(pseudo_buffer);
            }
            else if (strcmp(commande, "mp") == 0){
                //Message privé
                //Extraire le pseudo et le message
                char *pseudo = strtok(NULL, " ");
                char *message = strtok(NULL, "");

                // Vérifier que le pseudo et le message sont valides
                if (pseudo != NULL && message != NULL) {
                    // Envoyer le message privé
                    mp_client(info->index, pseudo, message);
                } else {
                    // Envoyer un message d'erreur au client
                    mp_client(0, info->pseudo, "Format de commande invalide pour MP");
                }
            }
            // Gestion des tags
            else {
                // Envoyer un message d'erreur au client
                mp_client(0, info->pseudo, "Commande inconnue");
            }
        }
        else if (strstr(buffer, "@")) {
            // Trouver la position de l'arobase
            char *at_position = strchr(buffer, '@');

            // Extraire le pseudo qui suit l'arobase
            char pseudo[MAX_PSEUDO_LENGTH + 1]; // +1 pour le caractère de fin de chaîne
            int extracted_length = 0;
            sscanf(at_position + 1, "%s%n", pseudo, &extracted_length);

            // Vérifier si le pseudo extrait est valide
            if (extracted_length == 0) {
                // Envoyer un message d'erreur au client
                mp_client(0, info->pseudo, "Format de commande invalide pour tag");
                continue;
            }

            //Verifier si le pseudo existe
            if (is_pseudo_available(pseudo)==1) {
                // Envoyer un message d'erreur au client
                mp_client(0, info->pseudo, "Aucun client avec ce pseudo");
                continue;
            }

            // Vérifie la longueur du pseudo extrait
            if (extracted_length <= MAX_PSEUDO_LENGTH) {
                // Traite le message avec le pseudo extrait
                char message[BUFFER_SIZE];
                sprintf(message, "%s%s", info->pseudo, " vous a tagué ");
                send_message_to_all_clients(client_index, buffer);
                mp_client(0,pseudo, message);
            }
            else {
                printf("Pseudo trop long\n");
                // Envoyer un message d'erreur au client
                mp_client(0, info->pseudo, "Pseudo trop long");
            }

        }
        else {
            // Renvoyer le message à tous les autres clients
            send_message_to_all_clients(client_index, buffer);
        }
    }

    //déconnexion du client

    // Envoyer le message de déconnexion à tous les clients
    send_disconnect_message_to_clients(info->pseudo);

    free(args);
    disconnect_client(info);
}



void disconnect_client(client_info *info) {
    int client_socket = info->socket_fd;
    int client_index = info->index;

    printf("En cours de déconnexion du client %d\n", client_index);

    // Fermer le socket et retirer le client de la liste chaînée
    close(client_socket);


    remove_client_from_list(client_index);


    printf("Client %d déconnecté\n", client_index);

    // Lorsque le client se déconnecte, libérer le slot
    rk_sema_post(get_client_slot());

    // Terminer le thread associé au client
    pthread_exit(NULL);
}
