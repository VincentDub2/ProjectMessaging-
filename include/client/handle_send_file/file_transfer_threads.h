#ifndef FILE_TRANSFER_THREADS_H
#define FILE_TRANSFER_THREADS_H

#include "../../common.h"
void *send_file_thread_func(void *arg);

typedef struct {
    char *file_list[MAX_FILES_LIST];
    char selected_indices[MAX_FILES_LIST];
} FileSendThreadArgs;

#endif //FILE_TRANSFER_THREADS_H
