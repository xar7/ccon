#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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

/* Create a new cgroup and attach this process to it. */
/* This function is using cgroup v1. */
int set_cgroup(void) {
    if (mkdir("/sys/fs/cgroup/pids/ccon", 0755) == -1) {
        if (errno != EEXIST) {
            perror(NULL);
            return 1;
        }
    }

    CALL(echo("10", "/sys/fs/cgroup/pids/ccon/pids.max"));

    pid_t pid = getpid();
    char str_pid[256];
    sprintf(str_pid, "%d", pid);
    CALL(echo(str_pid, "/sys/fs/cgroup/pids/ccon/cgroup.procs"));
    return 0;
}
