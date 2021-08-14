#include "mpu6050.h"
#include "math.h"
#include <stdint.h>
#include "fsl_gpio.h"
#include "data_transfer.h"

#define ack 1      //主应答
#define no_ack 0   //从应答	

#define SCL PTB2_O
#define SDA PTB3_O
#define SDA_I PTB3_I 


#define SCL_H_A SCL=1
#define SCL_L_A SCL=0
#define SDA_H_A SDA=1
#define SDA_L_A SDA=0


uint8_t temp=0;

void IIC_Init(void)
{
    gpio_pin_config_t gpio_i2c_1;
    gpio_i2c_1 .pinDirection = kGPIO_DigitalOutput;
    gpio_i2c_1 .outputLogic = 1;
    GPIO_PinInit(GPIOB, 2, &gpio_i2c_1);
    GPIO_PinInit(GPIOB, 3, &gpio_i2c_1);
}

void iic_delay(void)
{
    uint16_t k=120;
    while(k--);
}

void IIC_start(void)
{
    PTD->PDDR |= 0x00000002;
    SDA_H_A;
    SCL_H_A;
    iic_delay();
    SDA_L_A;
    iic_delay();
    SCL_L_A;
}

void IIC_stop(void)
{
    PTD->PDDR |= 0x00000002;
	SDA_L_A;
	SCL_L_A;
	iic_delay();
	SCL_H_A;
	iic_delay();
	SDA_H_A;
	iic_delay();
}

void IIC_SendACK(unsigned char ack_dat)
{
    PTD->PDDR |= 0x00000002;
    SCL_L_A;
    iic_delay();
    if(ack_dat) SDA_L_A;
    else SDA_H_A;
    SCL_H_A;
    iic_delay();
    SCL_L_A;
    iic_delay();
}

static uint8_t IIC_WaitAck(void)
{
    PTD->PDDR &= ~(0x00000002); //设置方向
    SCL_L_A;
    iic_delay();
    SCL_H_A;
    iic_delay();
    
    if(SDA_I)
    {
        PTD->PDDR |= 0x00000002;
        SCL_L_A;
        return 0;
    }
    PTD->PDDR |= 0x00000002;
    SCL_L_A;
    iic_delay();
    return 1;
}

void send_ch(uint8_t c)
{
    uint8_t i=8;
    PTD->PDDR |= 0x00000002;
    while(i--)
    {
        if(c&0x80) SDA_H_A;
        else SDA_L_A;
        
        c<<=1;
        iic_delay();
        SCL_H_A;
        iic_delay();
        SCL_L_A;
    }
    IIC_WaitAck();
}

uint8_t read_ch(void)
{
    uint8_t i;
    uint8_t c;
    c=0;
    PTD->PDDR |= 0x00000002;
    SCL_L_A;
    iic_delay();
    SDA_H_A;             //置数据线为输入方式
    PTD->PDDR &= ~(0x00000002);
    for(i=0;i<8;i++)
    {
        iic_delay();
        SCL_L_A;         //置时钟线为低，准备接收数据位
        iic_delay();
        SCL_H_A;         //置时钟线为高，使数据线上数据有效
        iic_delay();
        c<<=1;
        if(SDA_I) c+=1;   //读数据位，将接收的数据存c
    }
    PTD->PDDR |= 0x00000002;
	SCL_L_A;
	iic_delay();
	IIC_SendACK(no_ack);
	
    return c;
}

void MPU6050_WriteReg(uint8_t dev_add, uint8_t reg, uint8_t dat)
{
    IIC_start();
    send_ch( (dev_add<<1) | 0x00);   //发送器件地址加写位
	send_ch( reg );   				 //发送从机寄存器地址
	send_ch( dat );   				 //发送需要写入的数据
	IIC_stop();
}

uint8_t MPU6050_ReadReg(uint8_t dev_add, uint8_t reg)
{
	uint8_t dat;
	IIC_start();
    send_ch( (dev_add<<1) | 0x00);  //发送器件地址加写位
	send_ch( reg );   				//发送从机寄存器地址
	
	IIC_start();
	send_ch( (dev_add<<1) | 0x01);  //发送器件地址加读位
	dat = read_ch();   				//发送需要写入的数据
	IIC_stop();
	
	return dat;
}

uint8_t mpu6050_init(void)
{
    IIC_Init();
    temp = MPU6050_ReadReg(MPU6050_DEV_ADDR,WHO_AM_I);
    if(temp == 0x68)
    {
        MPU6050_WriteReg(MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);
        MPU6050_WriteReg(MPU6050_DEV_ADDR, SMPLRT_DIV, 0x04);      //125HZ采样率
        MPU6050_WriteReg(MPU6050_DEV_ADDR, CONFIG, 0x06);
        MPU6050_WriteReg(MPU6050_DEV_ADDR, GYRO_CONFIG, 0x08);	//陀螺仪量程+-500°/s
        MPU6050_WriteReg(MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x18);    
        MPU6050_WriteReg(MPU6050_DEV_ADDR, User_Control, 0x00);
        MPU6050_WriteReg(MPU6050_DEV_ADDR, INT_PIN_CFG, 0x02);
        
        return 0;
    }
    return 1;
}

int16_t GetData(uint8_t reg_address)
{
    uint8_t H,L;
    H=MPU6050_ReadReg(MPU6050_DEV_ADDR, reg_address);
    L=MPU6050_ReadReg(MPU6050_DEV_ADDR, reg_address+1);
    return (H<<8)+L;   //合成数据
}

int32_t mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;
int32_t mpu_acc_x,mpu_acc_y,mpu_acc_z;
float gyro_x,gyro_y,gyro_z;
float acc_x,acc_y,acc_z;
int16_t gyro_x_offset,gyro_y_offset,gyro_z_offset;
int16_t acc_x_offset,acc_y_offset,acc_z_offset;

float gyro_x_now;
float acc_x_now;

void sensor_calibrate(void)
{
    uint16_t time=0;
    for(time=0;time<500;time++)
    {
        mpu_acc_x += GetData(ACCEL_XOUT_H);
        mpu_acc_y += GetData(ACCEL_YOUT_H);
        mpu_acc_z += GetData(ACCEL_ZOUT_H) + 2000;
        mpu_gyro_x += GetData(GYRO_XOUT_H);
        mpu_gyro_y += GetData(GYRO_YOUT_H);
        mpu_gyro_z += GetData(GYRO_ZOUT_H);
    }
    acc_x_offset = mpu_acc_x/500;
    acc_y_offset = mpu_acc_y/500;
    acc_z_offset = mpu_acc_z/500;  //不用
    gyro_x_offset = mpu_gyro_x/500;
    gyro_y_offset = mpu_gyro_y/500;
    gyro_z_offset = mpu_gyro_z/500;
    
}

float angle_x=0,angle_y=0,angle_z=0;
void get_angle(void)
{
    int16_t acc_x, acc_y, acc_z;
	int16_t gyr_x, gyr_y;
	float angle_acc_x, angle_acc_y;
	float angle_delta_x, angle_delta_y;
	
	//读取传感器数据
	acc_x =(GetData(ACCEL_XOUT_H) - acc_x_offset);
	acc_y =(GetData(ACCEL_YOUT_H) - acc_y_offset);
	acc_z =(GetData(ACCEL_ZOUT_H) - acc_z_offset);
	gyr_x =(GetData(GYRO_XOUT_H) - gyro_x_offset);
	gyr_y =(GetData(GYRO_YOUT_H) - gyro_y_offset);
	//gyr_z =(GetData(GYRO_ZOUT_H) - gyro_z_offset);
	
	//计算x轴角度
	angle_acc_x = atan2f(acc_x ,-acc_z) * 57.29578f;	//通过加速度计算得的角度
	angle_delta_x = gyr_y / 65.53f * 0.01f;
	angle_x = 0.05f * angle_acc_x + 0.95f * (angle_x + angle_delta_x);
	
	//计算y轴角度
	angle_acc_y = -atan2f(acc_y ,-acc_z) * 57.29578f;	//通过加速度计算得的角度
	angle_delta_y = gyr_x / 65.53f * 0.01f;
	angle_y = 0.05f * angle_acc_y + 0.95f * (angle_y + angle_delta_y); 
	
}
