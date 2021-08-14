#ifndef _6050_i2c_h_
#define _6050_i2c_h_
#include "stdint.h"

#define MPU6050_ADDRESS         0x68

#define	MPU6050_DEV_ADDR	    0xD0>>1	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ

#define	SMPLRT_DIV				0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG					0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG				0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	        0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define INT_PIN_CFG	  			0x37    //����6050����I2CΪֱͨģʽ�Ĵ���
#define	ACCEL_XOUT_H	        0x3B
#define	ACCEL_XOUT_L	        0x3C
#define	ACCEL_YOUT_H	        0x3D
#define	ACCEL_YOUT_L	        0x3E
#define	ACCEL_ZOUT_H	        0x3F
#define	ACCEL_ZOUT_L	        0x40
#define	GYRO_XOUT_H				0x43
#define	GYRO_XOUT_L				0x44	
#define	GYRO_YOUT_H				0x45
#define	GYRO_YOUT_L				0x46
#define	GYRO_ZOUT_H				0x47
#define	GYRO_ZOUT_L				0x48
#define User_Control 			0x6A    //�ر�6050�Ը���I2C�豸�Ŀ���
#define	PWR_MGMT_1				0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I				0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)



extern int16_t Gyr_Z;
extern float ANGLE_X,ANGLE_Y,ANGLE_Z;


void iic_write(uint8_t addr, int16_t data);
int8_t iic_read(uint8_t addr, uint8_t reg);
void iic_write_reg(uint8_t addr, uint8_t *reg_data);
int16_t GetData1(uint8_t reg_address);
void sensor_calibrate1(void);
void Get_Angle(void);
uint8_t Mpu6050_Init(void);
void I2C_init(void);
void BOARD_I2C_ConfigurePins(void);
void BOARD_I2C_ReleaseBus(void);
void BOARD_I2C_ReleaseBus(void);


#endif
