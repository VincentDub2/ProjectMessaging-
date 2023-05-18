
#ifndef GET_FILES_H
#define GET_FILES_H

typedef struct {
    char* selected_indices;
    char** file_list;
    int serveur_socket_file;
} FileSendThreadArgs;

/**
 * @brief Fonction exécutée par un thread pour envoyer les fichiers sélectionnés à un client via un socket de transfert de fichiers.
 *
 * @param args Les arguments du thread.
 * @return NULL.
 */
void *send_file_thread_func(void *args);


/**
 * @brief Gère la commande "getFiles" en envoyant la liste des fichiers disponibles au client et en attendant les indices des fichiers à recevoir.
 *
 * @param socket_client Le socket du client.
 * @param pseudo Le pseudo du client.
 * @param serveur_socket_file Le socket du serveur pour les fichiers.
 * @return 1 si le client a saisi "exit", -1 en cas d'erreur.
 */
int handle_get_files_commande(int socket_client, const char *pseudo, int serveur_socket_file);

/**
 * @brief Envoie la liste des fichiers disponibles à un client spécifique.
 *
 * @param file_list Le tableau contenant la liste des fichiers.
 * @param socket_client Le socket du client auquel envoyer la liste des fichiers.
 * @return 0 si l'envoi s'est bien passé, -1 en cas d'erreur.
 */
int send_file_list(char *file_list[], int socket_client);

#endif //GET_FILES_H
