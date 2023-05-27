// include/serveur/common.h
#ifndef COMMON_H
#define COMMON_H
// Contient les constantes utilisées dans le programme


#define MAX_CLIENTS 10

#define MAX_CHANNELS 10

#define MAX_CLIENT_CHANNEL 10

#define BUFFER_SIZE 2048

#define MAX_PSEUDO_LENGTH 20

#define PORT 8080          // Port d'écoute du serveur

#define SERVER_PORT_FILE 8081 // Port d'écoute du serveur pour les transferts de fichiers

#define BACKLOG 5 // Longueur de la file d'attente pour les connexions entrantes

#define SERVER_IP "127.0.0.1"  // Adresse IP du serveur

#define MAX_MESSAGE_LENGTH 1000 // Longueur maximale d'un message (doit être légèrement inférieure à BUFFER_SIZE)

#define FILE_BUFFER_SIZE 1024 // Taille du buffer utilisé pour envoyer les fichiers

#define PATCH_CLIENT_FILE "client_files" // Dossier contenant les fichiers du client qui peuvent etre envoyes au serveur

#define PATCH_SERVEUR_FILE "serveur_files" // Dossier contenant les fichiers du serveur qui peuvent etre envoyes au client

#define CHANNEL_FILE "channel.txt" // Dossier contenant les fichiers des channels

#define MAX_FILES_LIST 100 // Nombre maximum de fichiers pouvant etre liste

#define MAX_FILES_NAME 256 // Nombre maximum de caractères dans le nom d'un fichier

#define MAX_FILES_SEND 10 // Nombre maximum de fichiers pouvant etre envoyes au serveur en meme temps

#define min(a,b) ((a) < (b) ? (a) : (b)) // Macro pour retourner le minimum entre deux valeurs


#endif // COMMON_H
