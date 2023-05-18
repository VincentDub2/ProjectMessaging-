#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>


#include "../../../include/serveur/serveur_utils/handle_pseudo.h"

#include "../../../include/common.h"
#include "../../../include/serveur/serveur_utils/client_list.h"

void receive_pseudo_from_client(char* pseudo_buffer,int client_socket){
    int pseudo_set = 0; // Indique si le pseudo du client a été défini

    printf("Buffer pseudo : %s\n", pseudo_buffer);

    while (!pseudo_set){

        memset(pseudo_buffer, 0, MAX_PSEUDO_LENGTH + 1);

        recv(client_socket, pseudo_buffer, MAX_PSEUDO_LENGTH + 1, 0);

        printf("Pseudo reçu : %s\n", pseudo_buffer);

        // Vérifier si le pseudo est disponible
        if (is_pseudo_available(pseudo_buffer)) {
            send(client_socket, "PSEUDO_OK", BUFFER_SIZE, 0); // Envoie "PSEUDO_OK" si le pseudo est disponible
            printf("envoie de pseudo_ok\n");
            pseudo_set = 1;
        } else {
            send(client_socket, "PSEUDO_TAKEN", BUFFER_SIZE, 0); // Envoie "PSEUDO_TAKEN" si le pseudo est déjà pris
            printf("envoie de pseudo_taken\n");
            continue; // Passe à la prochaine itération pour attendre un nouveau client
        }
    }

}