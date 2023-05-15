#ifndef COMMUNICATION_UTILS_H
#define COMMUNICATION_UTILS_H

void mp_client(int sender_index,const char* pseudo, const char* message);
//pre : pseudo valide / index valide / message valide
//post : envoie un message privé au client avec ce pseudo si il existe

void send_message_to_all_clients(int sender_index, const char* message);
//pre : index valide / message valide
//post : envoie un message à tous les clients sauf à celui qui l'envoie

void send_fin();
//pre : /
//post : envoie un message de fin à tous les clients pour qu'ils puissent se déconnecter

void send_welcome_message_to_clients(const char *new_client_pseudo);
//pre : pseudo valide
//post : envoie un message indiquant que q'un nouveau client s'est connecté au channel

void send_disconnect_message_to_clients(const char *new_client_pseudo);
//pre : pseudo valide
//post : envoie un message indiquant que q'un client s'est déconnecté du channel

void send_client_list(const char * pseudo);
//pre : pseudo valide
//post : envoie la liste des clients connectés au client avec ce pseudo

void send_manual(const char *pseudo);
//pre : pseudo valide / le fichier manuel.txt existe et se situe dans ressources/
//post : envoie le manuel d'utilisation au client avec ce pseudo


void send_file_list(char *file_list[],const char *pseudo);
//pre : pseudo valide / file_list valide
//post : envoie la liste des fichiers au client qui ont ce pseudo

#endif // COMMUNICATION_UTILS_H
