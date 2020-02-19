#include <err.h>
#include <stdio.h>

/* Not so clean debugging MACRO */
#ifdef DEBUG
#define LOG(...) fprintf(stderr, "[DEBUG] ");     \
    fprintf(stderr, __VA_ARGS__);                 \
    putchar('\n');

#else
#define LOG(...)
#endif /* DEBUG */

#ifdef DEBUG
#define LOGERR(...) fprintf(stderr, "[DEBUG][ERROR] "); \
    warn(__VA_ARGS__);
#else
#define LOGERR(...)
#endif /* DEBUG */
