#ifndef __LSM303DLH_H__
#define __LSM303DLH_H__

#include "sys/process.h"

#define LSM303_I2CADDR_ACC  0x30
#define LSM303_I2CADDR_MAG  0x3c

#define LSM303_ACCDATA_REG 0x28

#define LSM303_MAGDATA_REG 0x03

extern int lsm303acc[3];
extern int lsm303mag[3];


PROCESS_NAME(lsm303_process);

#endif
