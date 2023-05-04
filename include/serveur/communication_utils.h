#ifndef COMMUNICATION_UTILS_H
#define COMMUNICATION_UTILS_H

void mp_client(int sender_index,char* pseudo, const char* message);
//pre : pseudo valide / index valide / message valide
//post : envoie un message privé au client avec ce pseudo si il existe

void send_message_to_all_clients(int sender_index, const char* message);
//pre : index valide / message valide
//post : envoie un message à tous les clients sauf à celui qui l'envoie

void send_fin();
//pre : /
//post : envoie un message de fin à tous les clients pour qu'ils puissent se déconnecter
#endif // COMMUNICATION_UTILS_H
