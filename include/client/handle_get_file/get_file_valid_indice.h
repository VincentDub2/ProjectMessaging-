
#ifndef GET_FILE_VALID_INDICE_H
#define GET_FILE_VALID_INDICE_H

/**
 * @brief Obtient l'indice de validation du fichier.
 *
 * @return L'indice de validation du fichier : 0 pour OK, -1 pour NOK, 1 pour fin.
 *         En cas d'erreur de connexion avec le serveur, -1 est renvoyé.
 */
int get_file_valid_indice(int pipefd[2],int client_socket);

#endif //GET_FILE_VALID_INDICE_H
