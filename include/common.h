// include/serveur/common.h
#ifndef COMMON_H
#define COMMON_H
// Contient les constantes utilisées dans le programme


#define MAX_CLIENTS 10

#define BUFFER_SIZE 2048

#define MAX_PSEUDO_LENGTH 20

#define PORT 8080          // Port d'écoute du serveur

#define BACKLOG 5 // Longueur de la file d'attente pour les connexions entrantes

#define SERVER_IP "127.0.0.1"  // Adresse IP du serveur

#define MAX_MESSAGE_LENGTH 1000 // Longueur maximale d'un message (doit être légèrement inférieure à BUFFER_SIZE)

#endif // COMMON_H
