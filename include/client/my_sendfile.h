// my_sendfile.h
#include <errno.h>

#ifdef __linux__
#include <sys/sendfile.h>
#elif __APPLE__
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#endif

#define MY_SENDFILE_UNSUPPORTED -1

static inline ssize_t my_sendfile(int out_fd, int in_fd, off_t offset, size_t count) {
#ifdef __linux__
    // Utilisation de sendfile sur Linux
    ssize_t result = sendfile(out_fd, in_fd, &offset, count);
    if (result == -1) {
        perror("sendfile failed");
    }
    return result;
#elif __APPLE__
    // Utilisation de sendfile sur MacOS
    off_t len = count;
    int result = sendfile(in_fd, out_fd, offset, &len, NULL, 0);
    if (result == -1) {
        perror("sendfile failed");
    }
    return (ssize_t)result;
#else
    // Si le système d'exploitation n'est ni Linux ni MacOS,
    // cette fonction retourne une erreur.
    errno = MY_SENDFILE_UNSUPPORTED;
    perror("sendfile is not supported on this operating system");
    return -1;
#endif
}
