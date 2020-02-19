#define _GNU_SOURCE
#include <unistd.h>
#include <string.h>

#include "include/container.h"
#include "include/debug.h"

int container_function(void *raw_arg) {
    setvbuf(stdout, NULL, _IONBF, 0);
    struct container_arg *arg = (struct container_arg *) raw_arg;
    if (!arg->argv[0]) {
        puts("Usage: ./ccon <cmd>");
        return 1;
    }

    LOG("Entering child function.");
    //LOG("Setting the container hostname to : %s.", arg->opt->hostname);
    if (sethostname(arg->opt->hostname, strlen(arg->opt->hostname)) < 0) {
        LOGERR("sethostname failed");
        return 1;
    }

    if (chroot(arg->opt->rootfs) == -1) {
        LOGERR("chroot failed");
        return 1;
    }

    char **exec_argv = arg->argv;
    //LOG("Calling execvp for : %s", exec_argv[0]);
    if (execvp(exec_argv[0], exec_argv) == -1) {
        LOGERR("execvp failed");
        return 1;
    }

    return 0;
}
