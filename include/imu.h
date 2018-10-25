#ifndef TV_H_
#define TV_H_

#include <stdint.h>

/* MPU6050 registers */
#define PWR_MGT         0x6B
#define ACCEL_XOUT_H    0x3B
#define ACCEL_XOUT_L    0x3C
#define ACCEL_YOUT_H    0x3D
#define ACCEL_YOUT_L    0x3E
#define ACCEL_ZOUT_H    0x3F
#define ACCEL_ZOUT_L    0x40
#define GIRO_XOUT_H     0x43
#define GIRO_XOUT_L     0x44
#define GIRO_YOUT_H     0x45
#define GIRO_YOUT_L     0x46
#define GIRO_ZOUT_H     0x47
#define GIRO_ZOUT_L     0x48

/* xinabox SI01 registers [accelerometer + gyroscope] */
#define AG_I2C_ADDR     0x6A
#define WHO_AM_I_AG     0x0F
#define STATUS_REG_AG   0x17
#define ACC_READY       0x01
#define GYRO_READY      0x02
// gyrosocope
#define CTRL_REG1_G     0x10
#define CTRL_REG4       0x1E
#define OUT_X_L_G       0x18
#define OUT_X_H_G       0x19
#define OUT_Y_L_G       0x1A
#define OUT_Y_H_G       0x1B
#define OUT_Z_L_G       0x1C
#define OUT_Z_H_G       0x1D
#define GYRO_245DPS     8.75
#define GYRO_500DPS     17.5
#define GYRO_2000DPS    50
// acceleromoeter
#define CTRL_REG5_XL    0x1F
#define CTRL_REG6_XL    0x20
#define CTRL_REG7_XL    0x21
#define OUT_X_L_A       0x28
#define OUT_X_H_A       0x29
#define OUT_Y_L_A       0x2A
#define OUT_Y_H_A       0x2B
#define OUT_Z_L_A       0x2C
#define OUT_Z_H_A       0x2D
#define ACCEL_2G        0.061
#define ACCEL_4G        0.122
#define ACCEL_8G        0.244
#define ACCEL_16G       0.732       // ?? *2, *4, *8, *12

/* xinabox SI01 registers [magnetometer] */
#define M_I2C_ADDR      0x1C
#define WHO_AM_I_M      0x0F
#define MAG_READY       0x08
#define OUT_X_L_M       0x28
#define OUT_X_H_M       0x29
#define OUT_Y_L_M       0x2A
#define OUT_Y_H_M       0x2B
#define OUT_Z_L_M       0x2C
#define OUT_Z_H_M       0x2D
#define MAG_4GAUSS      0.14
#define MAG_8GAUSS      0.29
#define MAG_12GAUSS     0.43
#define MAG_16GAUSS     0.58

int open_file(const char *);
int MPU_setup(void);
int i2c_setup(int);
void read_byte(uint8_t, int8_t *, int);
void read_word(uint8_t, int16_t *, int);
void write_byte(uint8_t, int8_t, int);
void write_word(uint8_t, int16_t, int);
void accel_init(int);
void gyro_init(int);
void magnet_init(int);
void read_accels(int);
void read_gyro(int);
void read_magnet(int);
void check_whoami(int, int);

#endif
