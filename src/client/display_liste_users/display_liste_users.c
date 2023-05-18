#include "../../../include/client/display_liste_users/display_liste_users.h"
#include <stdio.h>
#include <string.h>
#include "../../../include/common.h"

/**
 * @brief Affiche la liste des clients connectés.
 *
 * @param rest La liste des pseudos des clients connectés.
 * @return N/A.
 */

void display_client_list(const char *rest) {
    // Supprimer la ligne actuelle du terminal
    //printf("\033[A\033[2K");

    printf("\rListe des clients connectés :\n");
    printf("-----------------------------\n");

    // Copier le contenu de 'rest' dans un tampon temporaire
    char buffer[BUFFER_SIZE];
    strncpy(buffer, rest, BUFFER_SIZE - 1);
    buffer[BUFFER_SIZE - 1] = '\0';

    // Extraire les pseudos
    char *pseudo = strtok(buffer, " ");
    while (pseudo != NULL) {
        printf("- %s\n", pseudo);
        pseudo = strtok(NULL, " ");
    }
    printf("-----------------------------\n");
    // Réaffiche la saisie de l'utilisateur
    printf("Entrez votre message: ");
    fflush(stdout);
}
