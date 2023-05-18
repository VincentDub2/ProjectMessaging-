#include "../../../include/serveur/command_message/handle_mp_message.h"

#include <string.h> // pour memset et sprintf
#include <sys/socket.h> // pour send
#include <unistd.h> // pour close
#include <stdio.h> // pour perror
#include <string.h>
#include "../../../include/common.h" // pour BUFFER_SIZE
#include "../../../include/serveur/serveur_communication/communication.h"
#include "../../../include/serveur/serveur_utils/client_list.h"


int mp_client(int socket_sender,const char* pseudo_sender,const char* pseudo_target, const char* message) {

    // On crée un nouveau tampon pour ajouter l'index du client
    char message_with_pseudo[BUFFER_SIZE];

    // On ajoute le pseudo du client qui envoie le message
    sprintf(message_with_pseudo, "%s: %s", pseudo_sender, message);

    // On parcourt la liste des clients et on envoie le message à tous les clients
    client_info *recipient = get_client_by_pseudo(pseudo_target);

    // Si le destinataire n'est pas trouvé, on envoie un message d'erreur au client qui a envoyé la commande
    if (recipient == NULL) {
        char error_msg[BUFFER_SIZE];
        sprintf(error_msg, "destinataire %s non trouvé.", pseudo_target);

        if (send_to_one_client("error", error_msg, socket_sender) == -1) {
            perror("Erreur lors de l'envoi du message d'erreur");
            return -1;
        }

        return 0;
    }

    else {
        if (send_to_one_client("mp",message_with_pseudo, recipient->socket_fd) == -1) {
            perror("Erreur lors de l'envoi du message du mp");
            return -1;
        }

        return 0;

    }
}