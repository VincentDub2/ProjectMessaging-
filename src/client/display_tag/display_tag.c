#include "../../../include/client/display_tag/display_tag.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../../include/common.h"

// Codes couleur ANSI
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/**
 * @brief Affiche un message de tag avec l'heure.
 *
 * @param message_received Le message à afficher au format "tag:pseudo:sender_pseudo:message".
 * @param pseudo Le pseudo de l'utilisateur actuel.
 * @return N/A.
 */
void display_tag_message(const char *message_received, const char *pseudo) {
    time_t rawtime;
    struct tm *timeinfo;
    char time_buffer[9]; // Format HH:MM:SS\0

    // Obtient l'heure actuelle
    time(&rawtime);
    // Convertit l'heure en une structure tm
    timeinfo = localtime(&rawtime);
    // Formate l'heure dans le buffer time_buffer
    strftime(time_buffer, 9, "%H:%M:%S", timeinfo);

    // Extraire le pseudo du tagué, pseudo de l'envoyeur et le message
    char extracted_pseudo[MAX_PSEUDO_LENGTH + 1]; // +1 pour le caractère de fin de chaîne
    char extracted_sender_pseudo[MAX_PSEUDO_LENGTH + 1]; // +1 pour le caractère de fin de chaîne
    char extracted_message[BUFFER_SIZE];
    sscanf(message_received, "%[^:]:%[^:]:%[^\n]", extracted_pseudo, extracted_sender_pseudo, extracted_message);

    // Recherche la position du pseudo tagué
    char *tagged_position = strstr(extracted_message, "@");

    // Vérifier si le message est destiné à l'utilisateur actuel
    if (strcmp(extracted_pseudo, pseudo) == 0) {
        // Efface la ligne actuelle
        printf("\033[2K\r");
        // Affiche le message formaté avec le pseudo de l'envoyeur en couleur, l'heure et le message
        printf("[%s] \033"ANSI_COLOR_RESET"%s: ", time_buffer, extracted_sender_pseudo);
        printf("%.*s" ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "%s\n", (int)(tagged_position - extracted_message), extracted_message, tagged_position, tagged_position + strlen(tagged_position));
    } else {
        // Efface la ligne actuelle
        printf("\033[2K\r");
        // Affiche le message formaté avec l'heure, le pseudo du tagué en couleur, le pseudo de l'envoyeur et le message
        printf("[%s] %s \033[1;36m%s\033[0m\n", time_buffer, extracted_pseudo, message_received);
    }
}



