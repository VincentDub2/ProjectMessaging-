#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>

#include "../../include/serveur/rk_sema.h"

#include "../../include/serveur/client_handler.h"

#include "../../include/serveur/signal_utilis.h"

#include "../../include/serveur/communication_utils.h"


#include "../../include/serveur/serveur_utils.h"

#include "../../include/common.h" // Permet de définir des constantes pour plusieurs fichiers

//Ici on va utiliser BUFFER_SIZE

// Fonctions pour gérer les clients et la liste des clients
//Gère la communication avec le client.
//Lit les messages du client et les retransmet à tous les autres clients.
//Supprime le client de la liste et ferme le socket lors de la déconnexion.
void *handle_client(void *arg);

// Fonction pour attendre les connexions des clients
//Accepte les connexions des clients.
//Crée un nouvel élément client_info et l'ajoute à la liste des clients.
//Crée un thread pour gérer chaque client.
void wait_for_clients(int server_socket);

void disconnect_client(client_info *info);

// Implémentez vos fonctions liées à la gestion des clients et de leurs threads ici

void wait_for_clients(int server_socket) {
    int client_count = 0;


    // Initialiser le sémaphore
    initialize_semaphore();

    pthread_t thread_id; //Identifiant du thread

    while (get_server_running()) {

        // Avant d'accepter un nouveau client, attendre qu'un slot soit disponible
        rk_sema_wait(get_client_slot());

        struct sockaddr_in client_addr; //adresse du client
        socklen_t client_addr_size = sizeof(client_addr); //taille de l'adresse du client

        // Accepter la connexion entrante
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
        //Gestion d'erreur
        if (client_socket == -1) {
            perror("Erreur lors de l'acceptation de la connexion entrante");
            continue; //On signale l'erreur et on continue l'écoute
        }

        // Créer un nouveau client_info et l'ajouter à la liste chaînée
        client_info *info = malloc(sizeof(client_info));

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

        // Créer un thread pour gérer le client
        if (pthread_create(&thread_id, NULL, handle_client, (void *)info)!=0) {
            perror("Erreur lors de la création du thread");
            close(client_socket);
            remove_client_from_list(info->index);
            continue;
        }
    }
}


void *handle_client(void *arg) {
    // Récupéré les informations du client
    client_info *info = (client_info *) arg;

    int client_socket = info->socket_fd;
    int client_index = info->index;

    int pseudo_set = 0; // Indique si le pseudo du client a été défini

    char pseudo_buffer[MAX_PSEUDO_LENGTH + 1];

    while (!pseudo_set){

        memset(pseudo_buffer, 0, MAX_PSEUDO_LENGTH + 1);

        recv(client_socket, pseudo_buffer, MAX_PSEUDO_LENGTH + 1, 0);

        printf("Pseudo reçu : %s\n", pseudo_buffer);

        // Vérifier si le pseudo est disponible
        if (is_pseudo_available(pseudo_buffer)) {
            send(client_socket, "PSEUDO_OK", 9, 0); // Envoie "PSEUDO_OK" si le pseudo est disponible
            pseudo_set = 1;
        } else {
            send(client_socket, "PSEUDO_TAKEN", 12, 0); // Envoie "PSEUDO_TAKEN" si le pseudo est déjà pris
            continue; // Passe à la prochaine itération pour attendre un nouveau client
        }
    }

    info->pseudo = strdup(pseudo_buffer);


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

    printf("Client %d connecté\n", client_index + 1);

    while (get_server_running()) {

        // Réinitialiser le tampon
        memset(buffer, 0, BUFFER_SIZE);

        // Recevoir un message du client
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        // Si recv() retourne -1 et que errno est EAGAIN ou EWOULDBLOCK, continuez la boucle
        // Cela signifie qu'il n'y a pas de données à lire
        if (bytes_received == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            usleep(200000); // Attendre 200 ms pour éviter une utilisation élevée du processeur
            continue;
        }


        // Si la réception échoue ou si le message est "fin", sortir de la boucle
        if (bytes_received == 0) {
            printf("Erreur de connection");
            break;
        }

        // Si le client envoie la commande "/quit", déconnectez-le
        if (strcmp(buffer, "/quit") == 0) {
            break;
        }

        // Gérez les autres commandes et messages ici
        // Créer des fonctions pour chaque commande



        if (strncmp(buffer, "/mp ", 4) == 0) {
            // Divise la chaîne en tokens séparés par un espace
            char *token = strtok(buffer + 4, " ");

            // Le premier token correspond au pseudo
            char *pseudo = token;

            // Le deuxième token correspond au message
            char *msg = strtok(NULL, "");

            // Affiche le pseudo et le message
            printf("Message privé pour %s : %s\n", pseudo, msg);

            // Envoie le message privé au client

            mp_client(client_index,pseudo, msg);

        }
        else {
            // Renvoyer le message à tous les autres clients
            send_message_to_all_clients(client_index, buffer);
        }
    }

    //déconnexion du client
    // Fermer le socket et retirer le client de la liste chaînée

    disconnect_client(info);
}



void disconnect_client(client_info *info) {
    int client_socket = info->socket_fd;
    int client_index = info->index;

    printf("En cours de déconnexion du client %d\n", client_index + 1);

    // Fermer le socket et retirer le client de la liste chaînée
    close(client_socket);


    remove_client_from_list(client_index);


    printf("Client %d déconnecté\n", client_index + 1);

    // Lorsque le client se déconnecte, libérer le slot
    rk_sema_post(get_client_slot());

    // Terminer le thread associé au client
    pthread_exit(NULL);
}
