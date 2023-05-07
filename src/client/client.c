#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdlib.h>
#include "../../include/client/affichage_utils.h"
#include "../../include/client/signal_utils.h"
#include "../../include/client/client_utils.h"
#include "../../include/client/client.h"
#include "../../include/common.h"



int main() {
    // Crée un nouveau socket et se connecte au serveur

    int socket_fd = create_socket();

    // Configuration du signal SIGINT (Ctrl+C)
    setup_sigint_handler(socket_fd);

    // Connection au serveur
    connect_to_server(socket_fd, SERVER_IP, PORT);
    printf("Connecté au serveur\n");

    printf("Bienvenue sur le chat ! Pas de spam | Pas d'insulte.\n");

    // Demande le pseudo du client
    char* pseudo = ask_for_pseudo(socket_fd);

    if (pseudo == NULL) {
        printf("Erreur lors de la demande du pseudo\n");
        close(socket_fd);
        free(pseudo);
        exit(EXIT_FAILURE);
    }

    if (get_client_running() == 0) {
        printf("Client fermé\n");
        close(socket_fd);
        free(pseudo);
        exit(EXIT_FAILURE);
    }

    printf("Bienvenue %s ! Vous avez rejoint la conversation.\n", pseudo);
    printf("Pour quitter le chat, tapez Ctrl+C ou /quit\n");
    printf("-------------------------------------------\n");

    printf("Entrez votre message: ");
    fflush(stdout);

    pthread_t send_thread, receive_thread;

    // Crée un thread pour envoyer des messages
    pthread_create(&send_thread, NULL, send_messages, (void *)&socket_fd);

    // Crée un thread pour recevoir des messages
    pthread_create(&receive_thread, NULL, receive_messages, (void *)&socket_fd);

    //Permet d'attendre que les threads se terminent avant de continuer l'exécution du programme principal.
    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);

    // Ferme le socket
    close(socket_fd);
    free(pseudo);

    printf("\r Fin de connection\n");
    return 0;
}


// Envoie des messages au serveur et gère les messages trop longs
void *send_messages(void *arg) {
    int socket_fd = *(int *)arg;
    char buffer[BUFFER_SIZE];



    while (get_client_running()){
        // Note :
        // On va utiliser select() pour vérifier si l'utilisateur a entré un message comme pour recevoir des messages.
        // L'intérêt de ne pas bloquer le thread avec fgets() si l'utilisateur n'a rien entré.

        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        // On vérifie l'entre standard (stdin) pour la lecture

        struct timeval timeout;
        timeout.tv_sec = 0; // 0 seconde
        timeout.tv_usec = 500000; // 500000 microsecondes (0.5 seconde)

        // Si 0 > select_result on a on un résultat d'un descripteur
        int select_result = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);


        if (select_result > 0 && FD_ISSET(STDIN_FILENO, &read_fds)) {

            fgets(buffer, BUFFER_SIZE, stdin);

            buffer[strcspn(buffer, "\n")] = '\0';

            // Vérifie si la longueur du message est inférieure à la longueur maximale autorisée
            if (strlen(buffer) > MAX_MESSAGE_LENGTH) {
                printf("Message trop long. Veuillez entrer un message de %d caractères maximum.\n", MAX_MESSAGE_LENGTH);
            }
            else
            {
                send(socket_fd, buffer, strlen(buffer), 0);
                if (strcmp(buffer, "/quit") == 0) {
                    set_client_running(0);
                }
            }
            display_sent_message("me", buffer);
        } else if (select_result == -1) {
            perror("Erreur lors de l'appel à select()");
            set_client_running(0);
        }
    }
    pthread_exit(NULL);
}


// Reçoit des messages du serveur et les affiche
void *receive_messages(void *arg) {

    //On récupère le socket
    int socket_fd = *(int *)arg;

    //On crée un buffer pour stocker les messages
    char buffer[BUFFER_SIZE];

    //On crée une variable pour stocker le nombre d'octets reçus
    int bytes_received;


    while (get_client_running()) {
        //Note :
        // On a decider d'utiliser la fonction select pour gérer les messages reçus
        // Car elle est non bloquante, on aurait pu aussi rendre le socket non bloquant comme pour
        // Le serveur


        //Initialise les descripteurs de fichiers à surveiller
        fd_set read_fds;
        FD_ZERO(&read_fds); //On vide le descripteur de fichiers
        FD_SET(socket_fd, &read_fds); // On spécifie le socket à surveiller pour la lecture

        //On spécifie le timeout
        struct timeval timeout;
        timeout.tv_sec = 1; // 1 seconde
        timeout.tv_usec = 0; // 0 microseconde

        //On utilise la fonction select pour surveiller le socket
        //Socket_fd + 1, car le premier paramètre est le nombre de descripteurs de fichiers à surveiller
        //&read_fds puisque c'est le descripteur de fichiers à surveiller pour la lecture
        //NULL parce qu'on ne surveille pas les descripteurs de fichiers pour l'écriture
        //NULL comme on ne surveille pas les descripteurs de fichiers pour les exceptions
        //&timeout : Si aucun descripteur n'est prêt pour l'opération spécifiée avant l'expiration du délai,
        //select() retournera &timeout.

        int select_result = select(socket_fd + 1, &read_fds, NULL, NULL, &timeout);

        //Si select_result est négatif, il y a eu une erreur
        //Si select_result est égal à 0, le délai d'attente est expiré
        //Si select_result est positif, signifie quand certains nombres de descripteurs de fichiers sont prêts


        // Ici, on vérifie que select_result est positif et que le socket est prêt pour la lecture
        if (select_result > 0 && FD_ISSET(socket_fd, &read_fds)) {

            // Réinitialise le tampon et reçoit un message du serveur
            memset(buffer, 0, BUFFER_SIZE);
            bytes_received = recv(socket_fd, buffer, BUFFER_SIZE, 0);


            // Si la réception échoue, sortir de la boucle
            if (bytes_received <= 0) {
                printf("Erreur lors de la réception des données du serveur\n");
                set_client_running(0);
            }

            // Gestion des commandes envoyées par le serveur
            // Un utilisateur ne peut pas envoyer des /commandes

            if (is_message_from_server(buffer) == 1) {

                // Vérifier si un slash est présent au début
                if (buffer[9] == '/') {
                      // Récupérer la commande
                    char buffer_copy[BUFFER_SIZE];
                    strncpy(buffer_copy, buffer, BUFFER_SIZE - 1);
                    buffer_copy[BUFFER_SIZE - 1] = '\0';

                    char *first_element = strtok(buffer_copy, " ");
                    char *second_element = strtok(NULL, " ");
                    char *rest = strtok(NULL, "");

                    printf("Commande reçue du serveur : %s\n", second_element);
                    second_element++;


                    // Traiter la commande
                    if (strcmp(second_element, "list") == 0) {
                        display_client_list(rest);
                    } else if (strcmp(second_element, "fin") == 0) {
                        printf("Le serveur a été arrêté\n");
                        set_client_running(0);
                    } else {
                        printf("Commande inconnue du serveur\n");
                    }
                } else {
                    // Afficher le message reçu
                    display_welcome_message(buffer);
                }
            }
            // Affiche le message reçu
            else {
                 display_message(buffer);
            }

        }
    }
    pthread_exit(NULL);
}


