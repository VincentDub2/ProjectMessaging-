#include "../../../include/serveur/command_channel/listChannel.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../../include/serveur/serveur_utils/channel.h"
#include "../../../include/common.h"
#include <string.h>
#include <pthread.h>

#include "../../../include/serveur/serveur_communication/communication.h"



//Fonction permet d'envoyer la liste des channels au client
//Avec le nombre de personnes dans chaque channel
int sendChannels(int socket_client){
    Channel * channels = get_tab_channels();
    pthread_mutex_t *channels_mutex = get_channels_mutex();
    char buffer[BUFFER_SIZE];

    pthread_mutex_lock(channels_mutex);
    for (int i = 0; i < MAX_CHANNELS; i++) {
        if (channels[i].name[0] != '\0') {
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "%s:%s:%d", channels[i].name, channels[i].description, channels[i].num_users);
            send_to_one_client("channelList", buffer, socket_client);
        }
    }
    send_to_one_client("channelList", "end", socket_client);
    pthread_mutex_unlock(channels_mutex);

    // Renvoie une valeur pour indiquer que la fonction a réussi.
    return 0;
}