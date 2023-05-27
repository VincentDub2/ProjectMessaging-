#ifndef SEND_MSG_CHANNEL_H
#define SEND_MSG_CHANNEL_H
/**
 * @brief Envoie un message à tous les utilisateurs d'un channel.
 *
 * @param channelName Le nom du channel.
 * @param msg Le message à envoyer.
 * @param pseudo_sender Le pseudo de l'expéditeur du message.
 * @param socket_client Le socket du client expéditeur.
 * @return 0 en cas de succès.
 */
int send_msg_to_channel(char *channelName, char *msg, char *pseudo_sender, int socket_client);


#endif //SEND_MSG_CHANNEL_H
