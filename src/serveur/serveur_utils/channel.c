#include   <stdio.h>
#include   <stdlib.h>
#include   <string.h>
#include  "../../../include/common.h"
#include "../../../include/serveur/serveur_utils/channel.h"


Channel channels[MAX_CHANNELS];
int num_channels = 0;
pthread_mutex_t channels_mutex = PTHREAD_MUTEX_INITIALIZER;


//Fonction qui permet de créer un channel
int create_channel(char* name, char* description,char *creator){
    //On vérifie si l'on a atteint le nombre maximum de channels
    if (num_channels == MAX_CHANNELS) {
        printf("Nombre maximum de channels atteint\n");
        return 0;
    }
    pthread_mutex_lock(&channels_mutex);

    //On enleve le \n du description du channel
    //name[strlen(description)-1] = '\0';

    Channel channel;
    strncpy(channel.name, name, sizeof(channel.name));
    strncpy(channel.description, description, sizeof(channel.description));
    channel.num_users = 0;
    strncpy(channel.creator, creator, sizeof(channel.creator));
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (strcmp(channels[i].name,name) == 0) {
            printf("Le channel %s existe déjà\n", name);
            pthread_mutex_unlock(&channels_mutex);
            return 0;
        }
    }

    for (int i=0; i<MAX_CHANNELS; i++){
        if (channels[i].name[0] == '\0'){
            channels[i] = channel;
            num_channels++;
            printf("Channel %s créé\n", channel.name);
            pthread_mutex_unlock(&channels_mutex);
            return 1;
        }
    }
    pthread_mutex_unlock(&channels_mutex);
    //On ajoute le channel au tableau de channels
    printf("Erreur lors de la création du channel\n");
    return -1;
}


//Fonction qui permet de sauvegarder les channels dans un fichier.txt
int save_channels() {
    FILE *file = fopen(CHANNEL_FILE, "w");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier %s\n", CHANNEL_FILE);
        return -1;
    }

    for (int i = 0; i < num_channels; i++) {
        fprintf(file, "%s:%s:%s\n", channels[i].name,channels[i].creator, channels[i].description);
    }

    fclose(file);
    return 0;
}


//Fonction permettant de charger les channels depuis un fichier.txt
int load_channels() {
    FILE *file = fopen(CHANNEL_FILE, "r");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier %s\n", CHANNEL_FILE);
        return -1;
    }

    for (int i = 0; i < MAX_CHANNELS; i++){
        if (fscanf(file, "%49[^:]:%49[^:]:%199[^\n]\n", channels[i].name,channels[i].creator, channels[i].description) != 3) {
            break;
        }
        num_channels++;
    }

    fclose(file);
    return num_channels;
}

// Fonction permettant de supprimer un channel
int delete_channel(char *name) {
    pthread_mutex_lock(&channels_mutex);

    //Enlever "\n" du nom du channel
    name[strlen(name) - 1] = '\0';

    for (int i = 0; i < num_channels; i++) {
        if (strcmp(channels[i].name, name) == 0) {
            for (int j = i; j < num_channels - 1; j++) {
                channels[j] = channels[j + 1];
            }
            memset(&channels[num_channels - 1], 0, sizeof(Channel)); // Clear the last channel
            num_channels--;
            pthread_mutex_unlock(&channels_mutex);
            return 1;
        }
    }

    pthread_mutex_unlock(&channels_mutex);
    printf("Channel %s n'a pas été trouvé\n", name);
    return 0;
}

//Fonction permettant a un utilisateur de rejoindre un channel
int join_channel(client_info *client,char *channel_name) {
    pthread_mutex_lock(&channels_mutex);
    //Enlever "\n" du nom du channel
    channel_name[strlen(channel_name) - 1] = '\0';

    for (int i = 0; i < num_channels; i++) {
        if (strcmp(channels[i].name, channel_name) == 0) {
            if (channels[i].num_users == MAX_CLIENTS) {
                printf("Le channel %s est plein\n", channel_name);
                pthread_mutex_unlock(&channels_mutex);
                return -1;
            }
            for (int j = 0; j < channels[i].num_users; j++) {
                if (strcmp(channels[i].clients[j].pseudo, client->pseudo) == 0) {
                    printf("L'utilisateur %s est déjà dans le channel %s\n", client->pseudo, channel_name);
                    pthread_mutex_unlock(&channels_mutex);
                    return -1;
                }
            }
            channels[i].clients[channels[i].num_users] = *client;
            channels[i].num_users++;
            printf("L'utilisateur %s a rejoint le channel %s\n", client->pseudo, channel_name);
            pthread_mutex_unlock(&channels_mutex);
            return 0;
        }
    }

    pthread_mutex_unlock(&channels_mutex);
    printf("Le channel %s n'a pas été trouvé\n", channel_name);
    return -1;
}


//Fonction permettant a un utilisateur de quitter un channel
int quit_channel(client_info *client,char *channel_name) {

    pthread_mutex_lock(&channels_mutex);

    //Enlever "\n" du nom du channel
    channel_name[strlen(channel_name) - 1] = '\0';

    for (int i = 0; i < num_channels; i++) {
        if (strcmp(channels[i].name, channel_name) == 0) {
            for (int j = 0; j < channels[i].num_users; j++) {
                // vérifier si le client existe avant de comparer les pseudos
                if (strcmp(channels[i].clients[j].pseudo, client->pseudo) == 0) {
                    for (int k = j; k < channels[i].num_users - 1; k++) {
                        channels[i].clients[k] = channels[i].clients[k + 1];
                    }
                    memset(&channels[i].clients[channels[i].num_users - 1], 0, sizeof(client_info)); // Clear the last user
                    channels[i].num_users--;
                    printf("L'utilisateur %s a quitté le channel %s\n", client->pseudo, channel_name);
                    pthread_mutex_unlock(&channels_mutex);
                    return 0;
                }
            }
            printf("L'utilisateur %s n'est pas dans le channel %s\n", client->pseudo, channel_name);
            pthread_mutex_unlock(&channels_mutex);
            return -1;
        }
    }

    pthread_mutex_unlock(&channels_mutex);
    printf("Le channel %s n'a pas été trouvé\n", channel_name);
    return -1;
}

Channel* get_tab_channels(){
    return channels;
}

pthread_mutex_t* get_channels_mutex(){
    return &channels_mutex;
}