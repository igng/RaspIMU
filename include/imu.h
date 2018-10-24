#ifndef TV_H_
#define TV_H_

#include <stdint.h>

/* read the datasheet for more informations */
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

int open_file(const char *);
int MPU_setup(void);
int M_setup(void);
int AG_setup(void);
void read_byte(uint8_t, int8_t *, int);
void read_word(uint8_t, int16_t *, int);
void read_accels(int);

#endif
