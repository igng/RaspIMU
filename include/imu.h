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

/* xinabox SI01 registers */
#define M_I2C_ADDR      0x1C
#define AG_I2C_ADDR     0x6A
#define WHO_AM_I        0x0F    // ask to the ag_chip
#define WHO_AM_I_M      0x0F    // ask to the m_chip

int open_file(const char *);
int MPU_setup(void);
int M_setup(void);
int AG_setup(void);
void read_byte(uint8_t, int8_t *, int);
void read_word(uint8_t, int16_t *, int);
void read_accels(int);

#endif
