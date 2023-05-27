#include "../../../include/client/display_success/display_success.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/**
 * @brief Affiche un message de success en rouge avec la date et l'heure.
 *
 * @param message_received Le message de success à afficher.
 * @return N/A.
 */
void display_success_message(const char *message_received) {
    time_t rawtime;
    struct tm *timeinfo;
    char time_buffer[9]; // Format HH:MM:SS\0

    // Obtient l'heure actuelle
    time(&rawtime);
    // Convertit l'heure en une structure tm
    timeinfo = localtime(&rawtime);
    // Formate l'heure dans le buffer time_buffer
    strftime(time_buffer, 9, "%H:%M:%S", timeinfo);

    // Efface la ligne actuelle
    printf("\033[2K\r");
    // Affiche le message formaté avec le pseudo "Success" (en vert), l'heure et le message d'erreur
    printf("[%s] \033[1;32m%s\033[0m: %s\n", time_buffer, "Success", message_received);

    printf("Entrez votre message: ");
    fflush(stdout);
}