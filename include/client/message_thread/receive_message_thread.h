
#ifndef RECEIVE_MESSAGE_THREAD_H
#define RECEIVE_MESSAGE_THREAD_H

typedef struct {
    int socket_fd;
    const char* pseudo;
    int pipe_fd[2];
} ReceiveThreadArgs;

/**
 * @brief Fonction exécutée par le thread pour recevoir les messages du serveur.
 *
 * @param arg Un pointeur vers le descripteur de fichier du socket de connexion au serveur.
 * @return N/A.
 */
void *receive_messages_thread(void *arg);

#endif //RECEIVE_MESSAGE_THREAD_H
