#include "../../../include/client/display_file/display_liste_file.h"

#include <stdio.h>

int display_liste_file(const char *message_received) {
    // Efface la ligne actuelle
    printf("\033[2K\r");
    // Affiche le message formaté avec le pseudo (en jaune), l'heure et le message
    printf("%s\n",message_received);

    printf("Entrez votre message: ");
    fflush(stdout);
}