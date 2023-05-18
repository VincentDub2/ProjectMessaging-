
#ifndef HANDLE_TAG_H
#define HANDLE_TAG_H
/**
 * @brief Gère le traitement d'un message de tag.
 *
 * @param buffer Le message reçu contenant le tag.
 * @param client_index L'index du client qui a envoyé le message.
 * @param client_socket Le socket du client qui a envoyé le message.
 * @param pseudo Le pseudo du client qui a envoyé le message.
 * @return 0 en cas de succès, -1 en cas d'erreur.
 */

int handle_tag_message(const char *buffer, int client_index, int client_socket, const char *pseudo);
#endif //HANDLE_TAG_H
