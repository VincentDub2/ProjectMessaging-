#include "../../../include/serveur/serveur_utils/handle_error_thread.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include  <unistd.h>



void* thread_waiter(void* args) {
    thread_waiter_args* tw_args = (thread_waiter_args*)args;
    void* ret;

    if (pthread_join(tw_args->thread_to_wait, &ret) != 0) {
        perror("pthread_join() error");
    }

    printf("thread exited with '%s'\n", ret);
    free(tw_args);

    return NULL;
}