/*
 * MPU_6050.h
 *
 *  Created on: Jan 17, 2013
 *      Author: steven
 */


#ifndef MPU_6050_H_
#define MPU_6050_H_

#define MPU6050_ADDRESS_AD0_LOW     (0xD0) // address pin low (GND)
#define MPU6050_ADDRESS_AD0_HIGH    (0xD1) // address pin high (VCC)

/*Register Addresses*/


#define MPU6050_RA_SMPLRT_DIV       	(0x19)
/*Sample rate = Gyroscope output rate/(1+SMPLRT_DIV)
 * Gyroscope output rate = 8kHz
 * We want 1kHz
 */
#define	MPU6050_SMPLRT_DIV					(0x7)


/*GYRO CONFIG*/
#define MPU6050_RA_GYRO_CONFIG      	(0x1B)
/*Gyro range*/
#define MPU6050_FS_SEL0						(0x0<<3)		/* +/- 250 degrees/s*/
#define MPU6050_FS_SEL1						(0x1<<3)	/* +/- 500 degrees/s*/
#define MPU6050_FS_SEL2						(0x2<<3)	/* +/- 1000 degrees/s*/
#define MPU6050_FS_SEL3						(0x3<<3)	/* +/- 2000 degrees/s*/
/*Self test*/
#define MPU6050_XG_ST						(0x1<<7)
#define MPU6050_YG_ST						(0x1<<6)
#define MPU6050_ZG_ST						(0x1<<5)



/*ACCEL CONFIG*/
#define MPU6050_RA_ACCEL_CONFIG     	(0x1C)
/*Accel range*/
#define MPU6050_AFS_SEL0					(0x0<<3)		/* +/- 2g */
#define MPU6050_AFS_SEL1					(0x1<<3)	/* +/- 4g */
#define MPU6050_AFS_SEL2					(0x2<<3)	/* +/- 8g */
#define MPU6050_AFS_SEL3					(0x3<<3)	/* +/- 16g */
/*Self test*/
#define MPU6050_XA_ST						(0x1<<7)
#define MPU6050_YA_ST						(0x1<<6)
#define MPU6050_ZA_ST						(0x1<<5)


/*INT PIN*/
#define MPU6050_RA_INT_PIN_CFG			(0x37)
#define MPU6050_INT_LEVEL_LOW				(0x1<<7)
#define MPU6050_INT_LEVEL_HIGH				(0x0<<7)
#define MPU6050_LATCH_INT_ENABLE			(0x1<<5)
#define MPU6050_LATCH_INT_DISABLE			(0x0<<5)
#define MPU6050_INT_RD_SET					(0x1<<4)
#define MPU6050_INT_RD_CLEAR				(0x0<<4)

/*INT ENABLE*/
#define MPU6050_RA_INT_EN				(0x38)
#define MPU6050_DATA_RDY_EN					(0x1<<0)

/*ACCEL DATA*/
#define MPU6050_RA_ACCEL_XOUT_H     	(0x3B)
#define MPU6050_RA_ACCEL_XOUT_L     	(0x3C)
#define MPU6050_RA_ACCEL_YOUT_H     	(0x3D)
#define MPU6050_RA_ACCEL_YOUT_L     	(0x3E)
#define MPU6050_RA_ACCEL_ZOUT_H     	(0x3F)
#define MPU6050_RA_ACCEL_ZOUT_L     	(0x40)

/*TEMP DATA*/
#define MPU6050_RA_TEMP_OUT_H       	(0x41)
#define MPU6050_RA_TEMP_OUT_L       	(0x42)

/*GRYO DATA*/
#define MPU6050_RA_GYRO_XOUT_H      	(0x43)
#define MPU6050_RA_GYRO_XOUT_L      	(0x44)
#define MPU6050_RA_GYRO_YOUT_H      	(0x45)
#define MPU6050_RA_GYRO_YOUT_L      	(0x46)
#define MPU6050_RA_GYRO_ZOUT_H      	(0x47)
#define MPU6050_RA_GYRO_ZOUT_L      	(0x48)

/*POWER MANAGEMENT*/
#define MPU6050_RA_PWR_MGMT_1			(0x6B)
#define MPU6050_WAKE						(0x0)
#define MPU6050_SLEEP						(0x1<<6)

/*WHO AM I*/
#define MPU6050_RA_WHO_AM_I         	(0x75)


void MEMSInit(void);
uint16_t startMEMS(void);
uint16_t stopMEMS(void);
uint16_t getRawAccelX(void);
uint16_t getRawAccelY(void);
uint16_t getRawAccelZ(void);
uint16_t getRawGyroX(void);
uint16_t getRawGyroY(void);
uint16_t getRawGyroZ(void);
uint32_t getTemp(void);

#endif /* MPU_6050_H_ */
