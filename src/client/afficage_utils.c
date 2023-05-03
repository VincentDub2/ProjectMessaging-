#include "../../include/client/affichage_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void display_message(const char *message_received) {
    time_t rawtime;
    struct tm *timeinfo;
    char time_buffer[9]; // Format HH:MM:SS\0

    time(&rawtime);
    timeinfo = localtime(&rawtime);
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
    printf("[%s] %s: %s\n", time_buffer, username, message);

    // Réaffiche la saisie de l'utilisateur
    printf("Entrez votre message: ");
    fflush(stdout);
}
