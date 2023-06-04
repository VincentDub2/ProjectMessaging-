//
// Created by vincent DUBUC on 13/05/2023.
//

#ifndef FILE_UTILS_H
#define FILE_UTILS_H


void list_files_in_directory(char *file_list[],const char *patch_file );

/**
 * Fonction de vérification de l'entrée utilisateur pour la sélection de fichiers.
 *
 * @param selected_indices - La chaîne de caractères contenant les indices sélectionnés.
 * @param file_list - Le tableau de pointeurs de chaînes de caractères contenant les noms de fichiers.
 * @return int - 1 si l'entrée est valide, 0 sinon.
 */

int check_valid_input(char *selected_indices, char *file_list[]);

/**
 * Fonction de listage des fichiers dans un répertoire.
 *
 * @param file_list - Un tableau de pointeurs de chaînes de caractères pour stocker les noms des fichiers.
 * @param patch_file - Le chemin du répertoire à lister.
 * @return void.
 */
void free_file_list(char *file_list[]);

/**
 * Fonction d'envoi de fichier.
 *
 * @param patch_file_name - Le chemin complet du fichier à envoyer.
 * @param sockfd - Le descripteur de socket à utiliser pour l'envoi du fichier.
 * @param file_name - Le nom du fichier à envoyer.
 * @return void.
 */
void send_file(const char *patch_file_name, int sockfd,const char *file_name);



#endif //FILE_UTILS_H
