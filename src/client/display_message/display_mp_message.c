#include "../../../include/client/display_message/display_all_message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>

/**
 * @brief Affiche un message privé avec le pseudo de l'expéditeur.
 *
 * @param message_received Le message privé au format "pseudo_envoyeur: message".
 * @return N/A.
 */
void display_mp_message(const char *message_received) {
    time_t rawtime;
    struct tm *timeinfo;
    char time_buffer[9]; // Format HH:MM:SS\0

    // Obtient l'heure actuelle
    time(&rawtime);
    // Convertit l'heure en une structure tm
    timeinfo = localtime(&rawtime);
    // Formate l'heure dans le buffer time_buffer
    strftime(time_buffer, 9, "%H:%M:%S", timeinfo);

    // Recherche la position du caractère ':' qui sépare le pseudo de l'expéditeur du message
    char *colon_position = strchr(message_received, ':');
    if (colon_position == NULL) {
        printf("Erreur: format de message invalide\n");
        return;
    }

    // Extrayez le pseudo de l'expéditeur et le message à partir de la chaîne d'origine
    int username_length = colon_position - message_received;

    char username[username_length + 1];
    strncpy(username, message_received, username_length);
    username[username_length] = '\0';

    const char *message = colon_position + 2;

    // Efface la ligne actuelle
    printf("\033[2K\r");
    // Affiche l'heure en couleur par défaut, et le pseudo de l'expéditeur et le message en couleur par défaut
    printf("[%s]\033[1;35m %s whispered to you\033[0m: %s", time_buffer, username, message);

    printf("Entrez votre message: ");
    fflush(stdout);
}
