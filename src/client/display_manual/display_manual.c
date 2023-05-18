#include "../../../include/client/display_manual/display_manual.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void display_manuel(const char *message_received) {

    // Efface la ligne actuelle
    printf("\033[2K\r");
    // Affiche le message formaté avec le pseudo (en jaune), l'heure et le message
    printf("%s\n",message_received);

    printf("Entrez votre message: ");
    fflush(stdout);
}
