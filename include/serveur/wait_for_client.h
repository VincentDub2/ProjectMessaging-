//
// Created by vincent DUBUC on 17/05/2023.
//

#ifndef WAIT_FOR_CLIENT_H
#define WAIT_FOR_CLIENT_H

/**
 * @brief Attend les connexions des clients et crée des threads pour gérer chaque client.
 *
 * @param server_socket_message Le socket du serveur pour les messages.
 * @param server_socket_file Le socket du serveur pour les fichiers.
 * @return NULL.
 */
void wait_for_clients(int server_socket_message, int server_socket_file);

#endif //WAIT_FOR_CLIENT_H
