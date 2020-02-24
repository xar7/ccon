#include "include/container.h"
#include "include/debug.h"


int echo(const char *value, const char *path) {
    int fd = open(path, O_WRONLY | O_CREAT);
    if (fd < 0) {
        perror(NULL);
        return -1;
    }

    CALL(write(fd, value, strlen(value)));
    CALL(close(fd));
    return 0;
}
