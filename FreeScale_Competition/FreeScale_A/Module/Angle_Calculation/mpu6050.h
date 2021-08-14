#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "fsl_gpio.h"
#include "gpio_port.h"
#include "stdint.h"

//���AD0��(9��)�ӵ�,IIC��ַΪ0X68(���������λ).
//�����V3.3,��IIC��ַΪ0X69(���������λ).
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

#define IIC_Port PTB
#define SCL_Pin GPIO_Pin2
#define SDA_Pin GPIO_Pin3




extern int16_t acc_x_offset, gyro_y_offset,acc_y_offset, gyro_x_offset;
extern float angle_x,angle_y,angle_z;
extern float acc_x,acc_y,acc_z;
extern float gyro_x,gyro_y,gyro_z;
void IIC_Init(void);
void MPU6050_WriteReg(uint8_t dev_add,uint8_t reg_address,uint8_t reg_data);
uint8_t MPU6050_ReadReg(uint8_t dev_add,uint8_t reg_address);
uint8_t mpu6050_init(void);
int16_t GetData(uint8_t reg_address);
void sensor_calibrate(void);
void get_angle(void);


#endif
