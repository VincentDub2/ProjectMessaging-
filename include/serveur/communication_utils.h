#ifndef COMMUNICATION_UTILS_H
#define COMMUNICATION_UTILS_H

void mp_client(int sender_index,char* pseudo, const char* message);
void send_message_to_all_clients(int sender_index, const char* message);
void send_fin();
#endif // COMMUNICATION_UTILS_H
