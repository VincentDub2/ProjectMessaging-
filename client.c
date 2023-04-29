#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define SERVER_IP "127.0.0.1"  // Adresse IP du serveur
#define PORT 8080             // Port du serveur
#define BUFFER_SIZE 1024      // Taille du tampon pour les messages
#define MAX_MESSAGE_LENGTH 1000 // Longueur maximale d'un message (doit être légèrement inférieure à BUFFER_SIZE)

// Fonction pour créer un socket
int create_socket();

// Fonction pour se connecter au serveur
void connect_to_server(int socket_fd);

// Fonction pour envoyer des messages
void *send_messages(void *arg);

// Fonction pour recevoir des messages
void *receive_messages(void *arg);

int client_actif = 1;

int main() {
    // Crée un nouveau socket et se connecte au serveur
    int socket_fd = create_socket();
    connect_to_server(socket_fd);
    printf("Connecté au serveur\n");

    pthread_t send_thread, receive_thread;

    // Crée un thread pour envoyer des messages
    pthread_create(&send_thread, NULL, send_messages, (void *)&socket_fd);

    // Crée un thread pour recevoir des messages
    pthread_create(&receive_thread, NULL, receive_messages, (void *)&socket_fd);

    //Permet d'attendre que les threads se terminent avant de continuer l'exécution du programme principal.
    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);

    // Ferme le socket
    close(socket_fd);
    return 0;
}

// Crée un socket et vérifie si la création a réussi
int create_socket() {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }
    return socket_fd;
}

// Se connecte au serveur en utilisant l'adresse IP et le port définis
void connect_to_server(int socket_fd) {
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur de connexion au serveur");
        exit(EXIT_FAILURE);
    }
}

// Envoie des messages au serveur et gère les messages trop longs
void *send_messages(void *arg) {
    int socket_fd = *(int *)arg;
    char buffer[BUFFER_SIZE];

    while (client_actif) {
        printf("Entrez votre message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        // Vérifie si la longueur du message est inférieure à la longueur maximale autorisée
        if (strlen(buffer) > MAX_MESSAGE_LENGTH) {
            printf("Message trop long. Veuillez entrer un message de %d caractères maximum.\n", MAX_MESSAGE_LENGTH);
        } else {
            send(socket_fd, buffer, strlen(buffer), 0);

            // Si le message est "fin", sortir de la boucle
            if (strcmp(buffer, "fin") == 0) {
                client_actif = 0;
            }
        }
    }
    pthread_exit(NULL);
}

// Reçoit des messages du serveur et les affiches
void *receive_messages(void *arg) {
    int socket_fd = *(int *)arg;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while (client_actif) {
        // Réinitialise le tampon et reçoit un message du serveur
        memset(buffer, 0, BUFFER_SIZE);
        bytes_received = recv(socket_fd, buffer, BUFFER_SIZE, 0);

        // Si la réception échoue, sortir de la boucle
        if (bytes_received <= 0) {
            break;
        }

        // Affiche le message reçu
        printf("Message reçu de %s\n", buffer);

        // Si le message est "fin", sortir de la boucle
        // Pas implémenté dans le serveur mais permet de finir la connexion si le serveur envoie "fin"
        if (strcmp(buffer, "fin") == 0) {
            client_actif = 0;
        }
    }
    pthread_exit(NULL);
}
