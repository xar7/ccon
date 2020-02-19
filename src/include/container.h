#ifndef CCON_CONTAINER
#define CCON_CONTAINER

#include <stdint.h>

struct container_option {
    char *hostname;
    uint64_t stack_size;
};

struct container_arg {
    char **argv;
    struct container_option *opt;
};

int container_function(void *arg);

#endif /* CCON_CONTAINER */
