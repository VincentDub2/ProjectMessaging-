#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "client_list.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8080          // Port d'écoute du serveur
#define BUFFER_SIZE 1024    // Taille du tampon pour les messages
#define BACKLOG 5 // Longueur de la file d'attente pour les connexions entrantes

// printf("\033[A\023[2K"); // Effacer ligne précédente

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

// Fonction qui permet de fermer le serveur proprement
//Ferme le socket serveur.
//Appelle shutdown_client_list() pour détruire le mutex et libérer la liste des clients
void shutdown_server(int server_socket);

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// Variables globales
volatile sig_atomic_t server_running = 1; // Variable pour indiquer si le serveur est en cours d'exécution
//Variable pour plus tard ajouter gestion de signal
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

int main() {

    int server_socket;
    struct sockaddr_in server_addr;

    // Créer un socket TCP
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    //Gestion d'erreur
    if (server_socket == -1) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Configurer l'adresse du serveur
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // Associer le socket avec l'adresse du serveur
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erreur lors de l'association du socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Serveur en écoute sur le port %d\n", PORT);

    // Écoute les connexions entrantes
    if (listen(server_socket, BACKLOG) == -1) {
        perror("Erreur lors de l'écoute des connexions entrantes");
        close(server_socket);
        exit(EXIT_FAILURE);
    }


    // Attendre les connexions des clients
    wait_for_clients(server_socket);

    // Fermer le serveur proprement
    shutdown_server(server_socket);

    return 0;
}



void wait_for_clients(int server_socket) {
    int client_count = 0;

    pthread_t thread_id; //Identifiant du thread

    while (server_running) {

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

    // Récupére les informations du client
    client_info *info = (client_info *) arg;

    int client_socket = info->socket_fd;
    int client_index = info->index;

    char buffer[BUFFER_SIZE];
    int bytes_received;

    printf("Client %d connecté\n", client_index + 1);

    while (server_running) {
        // Réinitialiser le tampon
        memset(buffer, 0, BUFFER_SIZE);

        // Recevoir un message du client
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received == -1) {
            perror("Erreur lors de la réception du message");
            break;
        }

        // Si la réception échoue ou si le message est "fin", sortir de la boucle
        if (bytes_received == 0 || strcmp(buffer, "fin") == 0) {
            break;
        }

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

            mp_client(client_index,pseudo, msg, BUFFER_SIZE);

        }
        else {
            // Renvoyer le message à tous les autres clients
            send_message_to_all_clients(client_index, buffer, BUFFER_SIZE);
        }
    }

    //deconnexion du client
    // Fermer le socket et retirer le client de la liste chaînée
    close(client_socket);
    remove_client_from_list(client_index);

    free(info);
    printf("Client %d déconnecté\n", client_index + 1);
    pthread_exit(NULL);
}


void shutdown_server(int server_socket) {
    close(server_socket);
    shutdown_client_list();
    printf("Serveur correctement arrêté\n");
}


