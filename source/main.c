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
    
    int m_fd = i2c_setup(M_I2C_ADDR);
    int ag_fd = i2c_setup(AG_I2C_ADDR);

    INFO("Opened m_fd at %d", m_fd);
    INFO("Opened ag_fd at %d", ag_fd);

    read_whoami(m_fd);
    read_whoami(ag_fd);

//    int fd = MPU_setup();
//    read_accels(fd);

//    start_time();
//    control_init();
//    ekf();
//    control_destroy();
//    end_time();

    return EXIT_SUCCESS;
}
