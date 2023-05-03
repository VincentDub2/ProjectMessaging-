#include "../../include/serveur/client_list.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void add_client_to_list(client_info* info);
void remove_client_from_list(int client_index);
void shutdown_client_list(void);

client_info* get_head_list(void);
pthread_mutex_t* get_mutex_list(void);
client_info* get_client_by_pseudo(char* pseudo);
client_info* get_client_by_index(int client_index);


// Pointeur vers la tête de la liste chaînée des clients
static client_info* client_list_head = NULL;

// Mutex pour protéger l'accès à la liste des clients
static pthread_mutex_t client_list_mutex = PTHREAD_MUTEX_INITIALIZER;



void add_client_to_list(client_info* info) {
    // Verrouiller l'accès à la liste des clients
    // pour éviter les conflits d'accès entre les différents threads
    pthread_mutex_lock(&client_list_mutex);
    info->next = client_list_head;

    // On ajoute le client au niveau de la tete de liste,
    // car c'est plus rapide que de parcourir toute la liste
    // à voir si plus tard, il ne faut pas l'ajouter à la fin plus tot
    client_list_head = info;

    // On peut libérer le thread, car on a fini de modifier la liste
    pthread_mutex_unlock(&client_list_mutex);
}

void remove_client_from_list(int client_index) {
    //On verrouille l'accès à la liste des clients


    pthread_mutex_lock(&client_list_mutex);



    client_info* prev = NULL;
    client_info* curr = client_list_head;

    //Tant que le client n'est pas trouvé, on parcourt la liste
    while (curr != NULL) {
        if (curr->index == client_index) {

            // On vérifie le cas ou le client cherché est en tete de liste
            if (prev == NULL) {
                client_list_head = curr->next;
            } else {
                prev->next = curr->next;
            }
            // Permet de sortir de la liste une fois le client trouvé
            // Peu-entre réfléchir à un autre moyen de quitter cette boucle,
            // car pas tres "propre"
            free(curr->pseudo);
            free(curr);
            break;
        }

        prev = curr;
        curr = curr->next;
    }

    //On peut libérer le thread, car on a fini de modifier la liste
    pthread_mutex_unlock(&client_list_mutex);
    return ;
}


void shutdown_client_list() {
    pthread_mutex_destroy(&client_list_mutex);

    while (client_list_head != NULL) {
        remove_client_from_list(client_list_head->index);
    }

    printf("Liste des clients libérée.\n");
}


client_info* get_client_by_index(int client_index) {
    pthread_mutex_lock(&client_list_mutex);

    client_info* curr = client_list_head;

    client_info *client = NULL;

    while (curr != NULL) {
        if (curr->index == client_index) {
            client = curr;
        }
        curr = curr->next;
    }

    pthread_mutex_unlock(&client_list_mutex);
    return client;
}

client_info* get_client_by_pseudo(char* pseudo){

    pthread_mutex_lock(&client_list_mutex);

    client_info* curr = client_list_head;
    client_info *client = NULL;

    while (curr != NULL) {
        if (strcmp(curr->pseudo, pseudo) == 0) {
            client = curr;
        }
        curr = curr->next;
    }

    pthread_mutex_unlock(&client_list_mutex);

    return client;
}


client_info* get_head_list(){
    return client_list_head;
}

pthread_mutex_t* get_mutex_list(){
    return &client_list_mutex;
}