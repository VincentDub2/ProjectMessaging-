//
// Created by vincent DUBUC on 13/05/2023.
//

#ifndef HANDLE_SEND_FILES_H
#define HANDLE_SEND_FILES_H

#include "../../include/common.h"
// Structure pour représenter les fichiers
typedef struct {
    char filenames[MAX_FILES_SEND][MAX_FILES_NAME];
    int count;
} FileList;

void handle_send_files_command();


#endif //HANDLE_SEND_FILES_H
