/**
 * @file channel.h
 * @brief Définitions et fonctions relatives aux channels du serveur.
 */

#ifndef CHANNEL_H
#define CHANNEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./client_list.h"
#include "../../../include/common.h"

/**
 * @struct Channel
 * @brief Structure représentant un channel.
 */
typedef struct {
    char name[50]; /**< Nom du channel. */
    char description[200]; /**< Description du channel. */
    char creator[50]; /**< Créateur du channel. */
    client_info clients[MAX_CLIENTS]; /**< Tableau des clients connectés au channel. */
    int num_users; /**< Nombre d'utilisateurs dans le channel. */
} Channel;


extern Channel channels[MAX_CHANNELS]; /**< Tableau des channels disponibles. */
extern int num_channels; /**< Nombre actuel de channels. */
extern pthread_mutex_t channels_mutex; /**< Mutex pour la gestion des channels. */

/**
 * @brief Crée un nouveau channel.
 *
 * @param name Le nom du channel.
 * @param description La description du channel.
 * @param creator Le créateur du channel.
 * @return 1 si le channel a été créé avec succès, 0 si le nombre maximal de channels a été atteint, -1 en cas d'erreur.
 */
int create_channel(char* name, char* description, char* creator);

/**
 * @brief Sauvegarde les channels dans un fichier.
 *
 * @return 0 si la sauvegarde a réussi, -1 en cas d'erreur.
 */
int save_channels();

/**
 * @brief Charge les channels à partir d'un fichier.
 *
 * @return Le nombre de channels chargés avec succès, -1 en cas d'erreur.
 */
int load_channels();

/**
 * @brief Supprime un channel.
 *
 * @param name Le nom du channel à supprimer.
 * @return 0 si le channel a été supprimé avec succès, -1 si le channel n'a pas été trouvé.
 */
int delete_channel(char* name);

/**
 * @brief Permet à un utilisateur de rejoindre un channel.
 *
 * @param client Les informations sur le client.
 * @param channel_name Le nom du channel.
 * @return 0 si l'utilisateur a rejoint le channel avec succès, -1 en cas d'erreur.
 */
int join_channel(client_info *client, char *channel_name);

/**
 * @brief Permet à un utilisateur de quitter un channel.
 *
 * @param client Les informations sur le client.
 * @param channel_name Le nom du channel.
 * @return 0 si l'utilisateur a quitté le channel avec succès, -1 en cas d'erreur.
 */
int quit_channel(client_info *client, char *channel_name);


/**
 * @brief Obtient un pointeur vers le tableau des channels.
 *
 * @return Un pointeur vers le tableau des channels.
 */
Channel* get_tab_channels();

/**
 * @brief Obtient un pointeur vers le mutex des channels.
 *
 * @return Un pointeur vers le mutex des channels.
 */
pthread_mutex_t* get_channels_mutex();

#endif /* CHANNEL_H */
