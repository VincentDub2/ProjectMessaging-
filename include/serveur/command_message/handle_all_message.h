//
// Created by vincent DUBUC on 16/05/2023.
//

#ifndef HANDLE_ALL_MESSAGE_H
#define HANDLE_ALL_MESSAGE_H
/**
 * @brief Envoie un message à tous les clients connectés, à l'exception de l'expéditeur.
 *
 * @param sender_index L'indice de l'expéditeur.
 * @param pseudo Le pseudo du client qui envoie le message.
 * @param message Le message à envoyer.
 * @return 0 si l'envoi s'est bien passé, -1 en cas d'erreur.
 */
int send_message_to_all_clients(int sender_index, const char* pseudo, const char* message);

#endif //HANDLE_ALL_MESSAGE_H
