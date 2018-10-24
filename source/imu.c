#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <sys/ioctl.h>

#include "imu.h"
#include "smbus.h"
#include "debug.h"

const char *i2c_bus = "/dev/i2c-1";
int addr = 0x68;

int open_file(const char *filename)
{
    int flags = O_CREAT | O_RDWR;
    int permissions = 0666;
	int fd = open(filename, flags, permissions); 

	if (fd == -1)
		error_handler("open_file | open");

    MISC("open_file | opened \"%s\" at fd %d", filename, fd);
	return fd;
}

int M_setup(void)


int MPU_setup(void)
{
    int fd;

    if ((fd = open_file(i2c_bus)) == -1)
        error_handler("MPU_setup | open");
    if (ioctl(fd, I2C_SLAVE, addr) == -1)
        error_handler("MPU_setup | ioctl");

    i2c_smbus_write_byte_data(fd, PWR_MGT, 0x00);
    MISC("MPU_setup | Opened file \"%s\" (address 0x%2x) at fd %d", i2c_bus, addr, fd);

    return fd;
}

void read_word(uint8_t reg, int16_t *dest, int fd)
{
    int8_t high, low;

    read_byte(reg, &high, fd);
    read_byte(reg + 1, &low, fd);
    *dest = (high << 8) + low;
    MISC("read_word | Read %d from registers 0x%2x and 0x%2x", *dest, reg, reg+1);
}

void read_byte(uint8_t reg, int8_t *dest, int fd)
{
    *dest = i2c_smbus_read_byte_data(fd, reg);
    MISC("read_byte | Read %d from register 0x%2x", *dest, reg);
}

void write_to_file(int file_fd, char *buff)
{
    if (write(file_fd, buff, strlen(buff)) == -1)
        error_handler("write_to_file | write");
}

void read_accels(int fd)
{
    int16_t x_acc, y_acc, z_acc;
    char line[32] = {0};
    int out_fd = open_file("accels.txt");

    for (;;)
    {
        read_word(ACCEL_XOUT_H, &x_acc, fd);
        read_word(ACCEL_YOUT_H, &y_acc, fd);
        read_word(ACCEL_ZOUT_H, &z_acc, fd);

        sprintf(line, "%d %d %d\n", x_acc, y_acc, z_acc);
        write_to_file(out_fd, line);

    }
}
