#include "../../../include/serveur/command_help/handle_help.h"

#include <string.h> // pour memset et sprintf

#include <stdio.h> // pour perror

#include "../../../include/common.h" // pour BUFFER_SIZE
#include "../../../include/serveur/serveur_communication/communication.h"



int send_manual(int socket) {
    FILE *file = fopen("manual.txt", "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier manual.txt");
        return -1;
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        if (send_to_one_client("help", buffer, socket) == -1) {
            perror("Erreur lors de l'envoi du manuel");
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 0;

}
