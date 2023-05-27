#include "../../../include/client/display_channels/display_channels.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @brief Affiche les détails d'un seul channel sous forme de table.
 *
 * @param channel_received Le channel à afficher.
 * @return N/A.
 */
static int isFirst = 1;

void display_single_channel(char *channel_received) {
    // Vérifie si le serveur a fini d'envoyer les détails du channel.
    if (strcmp(channel_received, "fin") == 0) {
        // Réinitialise isFirst à 1 et retourne.
        printf("Entrez votre message: ");
        fflush(stdout);
        isFirst = 1;
        return;
    }

    // Découpe la chaîne du channel reçu à chaque ":".
    char *name = strtok(channel_received, ":");
    char *description = strtok(NULL, ":");
    char *num_users_str = strtok(NULL, ":");

    // Affiche l'en-tête de la table une seule fois.
    if (isFirst) {
        isFirst = 0;
        printf("\033[2K\r");
        printf("%15s | %50s | %15s\n", "Channel Name", "Description", "Number of Users");
        printf("%15s-%50s-%15s\n", "---------------", "--------------------------------------------------", "---------------");
    }

    // Affiche les détails du channel.
    printf("%15s | %50s | %15s\n", name ? name : "", description ? description : "", num_users_str ? num_users_str : "");

    fflush(stdout);
}