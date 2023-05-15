
#ifndef GET_FILES_H
#define GET_FILES_H

typedef struct {
    char* selected_indices;
    char** file_list;
    int serveur_socket_file;
} FileSendThreadArgs;

void handle_get_files_commande(int socket_client,const char *pseudo,int server_socket_file) ;

#endif //GET_FILES_H
