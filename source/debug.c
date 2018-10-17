#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "debug.h"

struct timespec time_struct;
long elapsed;

void error_handler(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void usage(char **argv)
{
    fprintf(stderr, "Usage: %s <no arguments>\n", argv[0]);
    exit(EXIT_FAILURE);
}

long get_time(void)
{
    if (clock_gettime(CLOCK_REALTIME, &time_struct) != 0)
        error_handler("start_time | clock_gettime");

    return time_struct.tv_nsec;
}

void start_time(void)
{
    elapsed = get_time();
}

void end_time(void)
{
    elapsed = get_time() - elapsed;
    INFO("Elapsed: %li us", elapsed/US);
}
