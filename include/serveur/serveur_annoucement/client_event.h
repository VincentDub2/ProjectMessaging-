
#ifndef CLIENT_EVENT_H
#define CLIENT_EVENT_H

/**
 * @brief Envoie un message de rejoindre aux clients connectés.
 *
 * @param client_pseudo Le pseudo du client qui rejoint.
 * @return 0 si l'envoi s'est bien passé, -1 en cas d'erreur.
 */
int send_join_message_to_clients(const char *client_pseudo);

/**
 * @brief Envoie un message de déconnexion aux clients connectés.
 *
 * @param client_pseudo Le pseudo du client qui se déconnecte.
 * @return 0 si l'envoi s'est bien passé, -1 en cas d'erreur.
 */
int send_disconnect_message_to_clients(const char *client_pseudo);

/**
 * @brief Envoie un message d'arrêt aux clients connectés.
 *
 * @return 0 si l'envoi s'est bien passé, -1 en cas d'erreur.
 */
int send_shutdown_message_to_clients();

#endif //CLIENT_EVENT_H
