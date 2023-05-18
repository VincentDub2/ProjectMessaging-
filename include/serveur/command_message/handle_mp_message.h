
#ifndef HANDLE_MP_MESSAGE_H
#define HANDLE_MP_MESSAGE_H
/**
 * @brief Envoie un message privé à un client spécifique.
 *
 * @param socket_sender Le socket du client qui envoie le message.
 * @param pseudo_sender Le pseudo du client qui envoie le message.
 * @param pseudo_target Le pseudo du client destinataire.
 * @param message Le message à envoyer.
 * @return 0 si l'envoi s'est bien passé, -1 en cas d'erreur.
 */
int mp_client(int socket_sender, const char* pseudo_sender, const char* pseudo_target, const char* message);

#endif //HANDLE_MP_MESSAGE_H
