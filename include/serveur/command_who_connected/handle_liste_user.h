//
// Created by vincent DUBUC on 16/05/2023.
//

#ifndef HANDLE_LISTE_USER_H
#define HANDLE_LISTE_USER_H

/**
 * @brief Envoie la liste des clients connectés à un client spécifique.
 *
 * @param client_socket Le socket du client auquel envoyer la liste.
 * @return 0 si l'envoi s'est bien passé, -1 en cas d'erreur.
 */

int send_all_client_connected(int client_socket);
#endif //HANDLE_LISTE_USER_H
