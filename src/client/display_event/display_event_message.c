#include "../../../include/client/display_event/display_event_message.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void display_event_message(const char *message_received) {
    time_t rawtime;
    struct tm *timeinfo;
    char time_buffer[9]; // Format HH:MM:SS\0

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_buffer, 9, "%H:%M:%S", timeinfo);


    // Efface la ligne actuelle
    printf("\033[2K\r");
    // Affiche le message formaté avec le pseudo (en jaune), l'heure et le message
    printf("[%s] \033[1;33m%s\033[0m: %s\n", time_buffer,"Serveur", message_received);

    printf("Entrez votre message: ");
    fflush(stdout);
}
