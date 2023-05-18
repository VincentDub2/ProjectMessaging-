#ifndef HANDLE_HELP_H
#define HANDLE_HELP_H

/**
 * @brief Envoie le contenu du fichier de manuel à un client spécifique.
 *
 * @param socket Le socket du client auquel envoyer le manuel.
 * @return 0 si l'envoi s'est bien passé, -1 en cas d'erreur.
 */
int send_manual(int socket);

#endif //HANDLE_HELP_H
