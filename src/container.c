#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>

#include "include/container.h"
#include "include/debug.h"

#define SETENV_OVERWRITE 1

int set_container_env(struct container_arg *arg) {
    if (sethostname(arg->opt->hostname, strlen(arg->opt->hostname)) < 0) {
        LOGERR("sethostname failed");
        return 1;
    }

    if (chroot(arg->opt->rootfs) == -1) {
        LOGERR("chroot failed");
        return 1;
    }

    if (chdir("/") == -1) {
        LOGERR("chdir failed");
        return 1;
    }

    if (setenv("PS1", "cconsh$ ", SETENV_OVERWRITE) == -1) {
        LOGERR("setenv failed");
        return 1;
    }

    return 0;
}

int set_container_fs() {
    if (mount("/proc", "/proc", "proc", 0, "") == -1) {
        LOGERR("mounting proc failed");
        return 1;
    }

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

    char **exec_argv = arg->argv;

    if (execvp(exec_argv[0], exec_argv) == -1) {
        LOGERR("execvp failed");
        return 1;
    }

    return 0;
}
