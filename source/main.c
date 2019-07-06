#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <error.h>
#include <stdint.h>
#include <inttypes.h>

#include "imu.h"
#include "debug.h"

int main(int argc, char *argv[])
{
    if (argc != 1)
        usage(argv);
    
    int fd = MPU_setup();
    read_accels(fd);

    return EXIT_SUCCESS;
}
