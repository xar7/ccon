#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "include/container.h"
#include "include/debug.h"

#define SETENV_OVERWRITE 1

int set_container_env(struct container_arg *arg) {
    CALL(sethostname(arg->opt->hostname, strlen(arg->opt->hostname)));

    CALL(chroot(arg->opt->rootfs));
    CALL(chdir("/"));

    CALL(setenv("PS1", "cconsh:\\w$ ", SETENV_OVERWRITE));
    CALL(setenv("PATH", "/bin", SETENV_OVERWRITE));
    return 0;
}

int set_container_fs(void) {
    CALL(mount("/proc", "/proc", "proc", 0, ""));
    return 0;
}

int container_function(void *raw_arg) {
    LOG("Entering container function.");

    struct container_arg *arg = (struct container_arg *) raw_arg;
    if (!arg->argv[0]) {
        puts("Usage: ./ccon <cmd>");
        return 1;
    }

    if (set_container_env(arg) != 0) {
        LOGERR("an error occured in the container environment setup");
        return 1;
    }
    if (set_container_fs() != 0) {
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        LOGERR("fork failed!");
        return 1;
    }

    if (pid == 0) {                      /* Child */
        char **exec_argv = arg->argv;
        CALL(execvp(exec_argv[0], exec_argv));
    }
    else {                               /* Parent */
        int wstatus;
        CALL(waitpid(pid, &wstatus, 0));
        CALL(umount("/proc"));
    }

    return 0;
}
