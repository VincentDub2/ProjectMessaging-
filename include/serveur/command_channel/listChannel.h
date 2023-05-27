#ifndef LIST_CHANNEL_H
#define LIST_CHANNEL_H

/**
 * @brief Envoie la liste des channels au client avec le nombre de personnes dans chaque channel.
 *
 * @param socket_client Le socket du client.
 * @return 0 en cas de succès.
 */
int sendChannels(int socket_client);

#endif //LIST_CHANNEL_H
