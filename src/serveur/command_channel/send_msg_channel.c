#include "../../../include/serveur/command_channel/send_msg_channel.h"
#include <stdio.h>

#include <string.h>
#include <pthread.h>


#include "../../../include/serveur/serveur_utils/channel.h"
#include "../../../include/common.h"
#include "../../../include/serveur/serveur_communication/communication.h"

int send_msg_to_channel(char * channelName,char *msg,char * pseudo_sender,int socket_client){
    Channel * channels = get_tab_channels();
    pthread_mutex_t *channels_mutex = get_channels_mutex();
    int client_find=0;

    char buffer[BUFFER_SIZE];
    int channel_found = 0;
    pthread_mutex_lock(channels_mutex);
    for (int i = 0; i < MAX_CHANNELS; i++) {
        if (strcmp(channels[i].name, channelName) == 0) {
            channel_found = 1;
            if (channels[i].num_users >= 0) {
                for (int k = 0; k < MAX_CLIENT_CHANNEL; k++) {
                    if (channels[i].clients[k].socket_fd == socket_client) {
                        client_find = 1;
                    }
                }
                if (client_find == 0) {

                    send_to_one_client("error", "You are not in this channel", socket_client);
                    pthread_mutex_unlock(channels_mutex);
                    return 0;
                }
                for (int j = 0; j < MAX_CLIENT_CHANNEL; j++) {
                    if (channels[i].clients[j].socket_fd != socket_client && channels[i].clients[j].pseudo != NULL ) {
                        memset(buffer, 0, BUFFER_SIZE);
                        sprintf(buffer, "%s:%s:%s", channels[i].name,pseudo_sender, msg);
                        send_to_one_client("channel", buffer, channels[i].clients[j].socket_fd);
                    }
                }
            }
        }
    }
    pthread_mutex_unlock(channels_mutex);
    if (channel_found == 0) {
        send_to_one_client("error", "Channel not found", socket_client);
        return 1;
    }
    // Renvoie une valeur pour indiquer que la fonction a réussi.
    return 0;
}