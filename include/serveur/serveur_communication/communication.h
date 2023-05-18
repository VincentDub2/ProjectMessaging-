//
// Created by vincent DUBUC on 16/05/2023.
//

#ifndef COMMUNICATION_H
#define COMMUNICATION_H
/**
 * @brief Envoie un message à un client spécifique.
 *
 * @param protocol Le protocole du message.
 * @param message Le message à envoyer.
 * @param socket_client Le socket du client auquel envoyer le message.
 * @return 0 si l'envoi s'est bien passé, -1 en cas d'erreur.
 */
int send_to_one_client(const char* protocol, const char* message, int socket_client);

/**
 * @brief Envoie un message à tous les clients connectés, à l'exception de l'expéditeur.
 *
 * @param protocol Le protocole du message.
 * @param message Le message à envoyer.
 * @param sender_index L'indice de l'expéditeur.
 * @return 0 si l'envoi s'est bien passé, -1 en cas d'erreur.
 */
int send_to_all_client(const char* protocol, const char* message, int sender_index);

#endif //COMMUNICATION_H
