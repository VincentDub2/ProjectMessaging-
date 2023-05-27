#include "../../../include/client/display_channels/display_channels_message.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @brief Affiche un message reçu d'un channel.
 *
 * @param message_received Le message à afficher.
 * @return N/A.
 */
void display_channel_message(char *message_received) {
    time_t rawtime;
    struct tm *timeinfo;
    char time_buffer[9]; // Format HH:MM:SS\0

    // Obtient l'heure actuelle
    time(&rawtime);
    // Convertit l'heure en une structure tm
    timeinfo = localtime(&rawtime);
    // Formate l'heure dans le buffer time_buffer
    strftime(time_buffer, 9, "%H:%M:%S", timeinfo);

    // Découpe la chaîne du message reçu à chaque ":".
    char *channel_name = strtok(message_received, ":");
    char *pseudo_sender = strtok(NULL, ":");
    char *message = strtok(NULL, ":");

    // Efface la ligne actuelle
    printf("\033[2K\r");

    // Affiche le message formaté avec le nom du channel, le pseudo de l'expéditeur et le message
    if (channel_name && pseudo_sender && message) {
        printf("[%s] [%s] %s: %s",time_buffer,channel_name, pseudo_sender, message);
    }
    printf("Entrez votre message: ");
    fflush(stdout);
}