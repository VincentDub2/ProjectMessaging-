//
// Created by vincent DUBUC on 17/05/2023.
//

#ifndef SEND_MESSAGE_THREAD_H
#define SEND_MESSAGE_THREAD_H
typedef struct {
    int pipe_fd[2];
    int socket_fd;
} SendThreadArgs;
/**
 * @brief Fonction exécutée par le thread d'envoi des messages au serveur.
 *
 * @param arg Un pointeur vers le socket de connexion au serveur.
 * @return NULL.
 */
void *send_messages_thread(void *arg);

/**
 * @brief Envoie une commande au serveur.
 *
 * @param socket_fd Le descripteur de fichier du socket de connexion au serveur.
 * @param message Le message à envoyer.
 * @return 0 si l'envoi du message s'est déroulé avec succès, -1 en cas d'erreur.
 */
int send_message(int socket_fd, const char* message);



#endif //SEND_MESSAGE_THREAD_H
