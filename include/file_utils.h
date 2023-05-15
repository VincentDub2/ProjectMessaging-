//
// Created by vincent DUBUC on 13/05/2023.
//

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

void list_files_in_directory(char *file_list[],const char *patch_file );
int check_valid_input(char *selected_indices, char *file_list[]);
void free_file_list(char *file_list[]);
void send_file(const char *patch_file_name, int sockfd,const char *file_name);



#endif //FILE_UTILS_H
