
#ifndef HANDLE_FILE_RECEPTION_H
#define HANDLE_FILE_RECEPTION_H

/**
 * Fonction de gestion de la réception de fichiers.
 *
 * @param arg - Un pointeur vers le descripteur de socket du client.
 * @return void* - Pointeur nul (NULL).
 */
void* handle_file_reception(void* arg);

/**
 * Fonction de gestion de la commande de réception de fichiers.
 *
 * @param serveur_socket_file - Le descripteur de socket du serveur dédié à la réception de fichiers.
 * @return void.
 */
void handle_file_reception_command(int serveur_socket_file);

#endif //HANDLE_FILE_RECEPTION_H
