#include "../../../include/client/display_message/display_all_message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



void display_all_message(const char *message_received) {
    time_t rawtime;
    struct tm *timeinfo;
    char time_buffer[9]; // Format HH:MM:SS\0

    // Obtient l'heure actuelle
    time(&rawtime);
    // Convertit l'heure en une structure tm
    timeinfo = localtime(&rawtime);
    // Formate l'heure dans le buffer time_buffer
    strftime(time_buffer, 9, "%H:%M:%S", timeinfo);

    // Recherche la position du caractère ':' qui sépare le pseudo du message
    char *colon_position = strchr(message_received, ':');
    if (colon_position == NULL) {
        printf("Erreur: format de message invalide\n");
        return;
    }

    // Extrayez le pseudo et le message à partir de la chaîne d'origine
    int username_length = colon_position - message_received;

    char username[username_length + 1];
    strncpy(username, message_received, username_length);
    username[username_length] = '\0';

    const char *message = colon_position + 2;

    // Efface la ligne actuelle
    printf("\033[2K\r");
    // Affiche le message formaté avec le pseudo, l'heure et le message
    printf("[%s] [all] %s: %s", time_buffer, username, message);
    printf("Entrez votre message: ");
    fflush(stdout);
}
