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

int i2c_setup(int device_address)
{
    int fd = open_file(i2c_bus);

    if (ioctl(fd, I2C_SLAVE, device_address) == -1)
        error_handler("i2c_setup | ioctl");

    return fd;
}

int MPU_setup(void)
{
    int fd;

    if ((fd = open_file(i2c_bus)) == -1)
        error_handler("MPU_setup | open");
    if (ioctl(fd, I2C_SLAVE, addr) == -1)
        error_handler("MPU_setup | ioctl");

    i2c_smbus_write_byte_data(fd, PWR_MGT, 0x00);
    MISC("MPU_setup | Opened file \"%s\" (address 0x%2X) at fd %d", i2c_bus, addr, fd);

    return fd;
}

void read_byte(uint8_t reg, int8_t *dest, int fd)
{
    *dest = i2c_smbus_read_byte_data(fd, reg);
    MISC("read_byte | Read %d from register 0x%2X", *dest, reg);
}

void read_word(uint8_t reg, int16_t *dest, int fd)
{
    *dest = i2c_smbus_read_word_data(fd, reg);
    MISC("read_word | Read %d from registers 0x%2X and 0x%2X", *dest, reg, reg+1);
}

void write_byte(uint8_t reg, int8_t data, int fd)
{
    i2c_smbus_write_word_data(fd, reg, data);
    MISC("write_byte | Wrote %d into register 0x%2X", data, reg);
}

void write_word(uint8_t reg, int16_t data, int fd)
{
    i2c_smbus_write_word_data(fd, reg, data);
    MISC("write_word | Wrote %d into register 0x%2X and %d into register 0x%2X", data & 0x0F, reg, (data >> 8) & 0x0F, reg+1);
}

void write_to_file(int file_fd, char *buff)
{
    if (write(file_fd, buff, strlen(buff)) == -1)
        error_handler("write_to_file | write");
}

void read_accels(int fd)
{
    int8_t new_data;
    int16_t x_accel, y_accel, z_accel;

    read_byte(STATUS_REG_AG, &new_data, fd);

    if (!(new_data & ACC_READY))
        return;

    read_word(OUT_X_L_A, &x_accel, fd);
    read_word(OUT_Y_L_A, &y_accel, fd);
    read_word(OUT_Z_L_A, &z_accel, fd);

    INFO("x_accel: %d | y_accel: %d | z_accel: %d", x_accel, y_accel, z_accel);
}

void read_gyro(int fd)
{
    int8_t new_data;
    int16_t x_gyro, y_gyro, z_gyro;

    read_byte(STATUS_REG_AG, &new_data, fd);

    if (!(new_data & GYRO_READY))
        return;

    read_word(OUT_X_L_G, &x_gyro, fd);
    read_word(OUT_Y_L_G, &y_gyro, fd);
    read_word(OUT_Z_L_G, &z_gyro, fd);

    INFO("x_gyro: %d | y_gyro: %d | z_gyro: %d", x_gyro, y_gyro, z_gyro);
}

void read_magnet(int fd)
{
    int16_t x_mag, y_mag, z_mag;

    read_word(OUT_X_L_M, &x_mag, fd);
    read_word(OUT_Y_L_M, &y_mag, fd);
    read_word(OUT_Z_L_M, &z_mag, fd);

    INFO("x_mag: %d | y_mag: %d | z_mag: %d", x_mag, y_mag, z_mag);
}

void check_whoami(int ag_fd, int m_fd)
{
    int8_t ag_w, m_w;
    int16_t who_am_i;

    read_byte(WHO_AM_I_AG, &ag_w, ag_fd);
    read_byte(WHO_AM_I_M, &m_w, m_fd);

    who_am_i = (ag_w << 8) | m_w;

    if (who_am_i != 0x683D)
        ERROR("who_am_i [0x%4X] != 0x683D", who_am_i);
    else
        INFO("who_am_i: 0x%4X", who_am_i);
}

void accel_init(int fd)
{
    uint8_t data = 0;

    data |= 0x38;       // 00111000; enables axis output
    write_byte(CTRL_REG5_XL, data, fd);
    data = 0;

    data |= 0xC0;       // 11000000; maximum sample rate
    write_byte(CTRL_REG6_XL, data, fd);
}

void gyro_init(int fd)
{
    uint8_t data = 0;

    data |= 0xC0;
    write_byte(CTRL_REG1_G, data, fd);
    data = 0;

    data |= 0x38;
    write_byte(CTRL_REG4, data, fd);
}

void magnet_init(int fd)
{
    fd = fd;
}
