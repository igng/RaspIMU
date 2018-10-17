#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <error.h>
#include <stdint.h>
#include <inttypes.h>

#include "imu.h"
#include "debug.h"
#include "linalg.h"
#include "control.h"

int main(int argc, char *argv[])
{
    if (argc != 1)
        usage(argv);
    
//    int fd = MPU_setup();
//    read_accels(fd);

    control_init();
    start_time();
    ekf();
    end_time();
    control_destroy();

    return EXIT_SUCCESS;
}
