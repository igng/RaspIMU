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
float gyro_sens = GYRO_245DPS;
float accel_sens = ACCEL_2G;
float mag_sens = MAG_4GAUSS;
float x_acc, y_acc, z_acc;
float x_gyro, y_gyro, z_gyro;
float x_mag, y_mag, z_mag;
int acc_fd, gyro_fd, mag_fd;

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
    int16_t accel_data;
    char line[16];

    read_byte(STATUS_REG_AG, &new_data, fd);

    if (!(new_data & ACC_READY))
        return;

    read_word(OUT_X_L_A, &accel_data, fd);
    x_acc = accel_data*accel_sens;

    read_word(OUT_Y_L_A, &accel_data, fd);
    y_acc = accel_data*accel_sens;

    read_word(OUT_Z_L_A, &accel_data, fd);
    z_acc = accel_data*accel_sens;

    //INFO("x_acc: %.2f | y_acc: %.2f | z_acc: %.2f", x_acc, y_acc, z_acc);
    sprintf(line, "%.2f %.2f %.2f\n", x_acc, y_acc, z_acc);
    write_to_file(acc_fd, line);
}

void read_gyro(int fd)
{
    int8_t new_data;
    int16_t gyro_data;
    char line[16];

    read_byte(STATUS_REG_AG, &new_data, fd);

    if (!(new_data & GYRO_READY))
        return;

    read_word(OUT_X_L_G, &gyro_data, fd);
    x_gyro = gyro_data*gyro_sens;

    read_word(OUT_Y_L_G, &gyro_data, fd);
    y_gyro = gyro_data*gyro_sens;

    read_word(OUT_Z_L_G, &gyro_data, fd);
    z_gyro = gyro_data*gyro_sens;

    //INFO("x_gyro: %.2f | y_gyro: %.2f | z_gyro: %.2f", x_gyro, y_gyro, z_gyro);
    sprintf(line, "%.2f %.2f %.2f\n", x_gyro, y_gyro, z_gyro);
    write_to_file(gyro_fd, line);
}

void read_magnet(int fd)
{
    int8_t new_data;
    int16_t mag_data;
    char line[16];

    read_byte(STATUS_REG_M, &new_data, fd);

    if (!(new_data & MAG_READY))
        return;

    read_word(OUT_X_L_M, &mag_data, fd);
    x_mag = mag_data*mag_sens;

    read_word(OUT_Y_L_M, &mag_data, fd);
    y_mag = mag_data*mag_sens;

    read_word(OUT_Z_L_M, &mag_data, fd);
    z_mag = mag_data*mag_sens;

    //INFO("x_mag: %.2f | y_mag: %.2f | z_mag: %.2f", x_mag, y_mag, z_mag);
    sprintf(line, "%.2f %.2f %.2f", a_mag, y_mag, z_mag);
    write_to_file(mag_fd, line);
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
    uint8_t data = 0x00;

    data |= 0x38;       // 00111000; enables axis output
    write_byte(CTRL_REG5_XL, data, fd);

    acc_fd = open_file("acc.txt");
}

void gyro_init(int fd)
{
    uint8_t data = 0x00;

    data |= 0xC0;       // 11000000; maximum sample rate
    write_byte(CTRL_REG1_G, data, fd);

    data = 0x00;
    write_byte(CTRL_REG2_G, data, fd);

    data = 0x00;
    write_byte(CTRL_REG3_G, data, fd);

    data = 0x00;
    data |= 0x38;       // 00111000; enable axis output
    data |= 0x02;       // 00000010; latcedIRQ
    write_byte(CTRL_REG4, data, fd);

    data = 0x00;
    write_byte(ORIENT_CFG_G, data, fd);

    gyro_fd = open_file("gyro.txt");
}

void magnet_init(int fd)
{
    uint8_t data = 0x00;

    data |= 0x60;       // 01100000; XY ultra-high performance mode
    data |= 0x1C;       // 00011100; maximum sample rate
    write_byte(CTRL_REG1_M, data, fd);

    data = 0x00;
    write_byte(CTRL_REG2_M, data, fd);

    data = 0x00;
    write_byte(CTRL_REG3_M, data, fd);

    data = 0x00;
    data |= 0x0C;       // 00001100; Z ultra-high performance mode
    write_byte(CTRL_REG4_M, data, fd);

    data = 0x00;
    write_byte(CTRL_REG5_M, data, fd);

    mag_fd = open_file("mag.txt");
}
