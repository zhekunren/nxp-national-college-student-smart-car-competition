#include "6050_i2c.h"
#include "fsl_i2c.h"
#include "math.h"
#include "user_i2c.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "data_transfer.h"

void BOARD_I2C_ConfigurePins(void) {
  //CLOCK_EnableClock(kCLOCK_PortB);                           /* Port D Clock Gate Control: Clock enabled */

  const port_pin_config_t port_SCL_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainEnable,                                   /* Open drain is enabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt2,                                           /* Pin is configured as I2C0_SCL */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTB, 2, &port_SCL_config);  /* PORTB8 (pin B2) is configured as I2C0_SCL */
  const port_pin_config_t port_SDA_config = {
    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
    kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
    kPORT_OpenDrainEnable,                                   /* Open drain is enabled */
    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
    kPORT_MuxAlt2,                                           /* Pin is configured as I2C0_SDA */
    kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
  };
  PORT_SetPinConfig(PORTB, 3, &port_SDA_config);  /* PORTB8 (pin B3)  is configured as I2C0_SDA */
}

void I2C_init(void)
{
    i2c_master_config_t masterConfig;
	I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = 400000;
    I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(I2C0_CLK_SRC)); 
}



uint8_t Mpu6050_Init(void)
{
	uint8_t temp;
	I2C_init();
    temp = I2CReadReg(I2C0, MPU6050_DEV_ADDR, WHO_AM_I);
    if(temp == 0x68)
    {
        I2CWriteReg(I2C0, MPU6050_DEV_ADDR, PWR_MGMT_1, 0x00);
        I2CWriteReg(I2C0, MPU6050_DEV_ADDR, SMPLRT_DIV, 0x04);      //125HZ采样率
        I2CWriteReg(I2C0, MPU6050_DEV_ADDR, CONFIG, 0x06);
        I2CWriteReg(I2C0, MPU6050_DEV_ADDR, GYRO_CONFIG, 0x08);	//陀螺仪量程+-500°/s
        I2CWriteReg(I2C0, MPU6050_DEV_ADDR, ACCEL_CONFIG, 0x18);    
        I2CWriteReg(I2C0, MPU6050_DEV_ADDR, User_Control, 0x00);
        I2CWriteReg(I2C0, MPU6050_DEV_ADDR, INT_PIN_CFG, 0x02);
        
        return 0;
    }
    return 1;
}

int16_t GetData1(uint8_t reg_address)
{
    uint8_t H,L;
    H=I2CReadReg(I2C0, MPU6050_DEV_ADDR, reg_address);
    L=I2CReadReg(I2C0, MPU6050_DEV_ADDR, reg_address+1);
    return (H<<8)+L;   //合成数据
}


int32_t MPU_GYRO_X,MPU_GYRO_Y,MPU_GYRO_Z;
int32_t MPU_ACC_X,MPU_ACC_Y,MPU_ACC_Z;
float GRYO_X,GRYO_Y,GRYO_Z;
float ACC_X,ACC_Y,ACC_Z;
int16_t GRYO_X_OFFSET,GRYO_Y_OFFSET,GRYO_Z_OFFSET;
int16_t ACC_X_OFFET,ACC_Y_OFFET,ACC_Z_OFFET;

float GRYO_X_NOW;
float ACC_X_NOW;

void sensor_calibrate1(void)
{
    uint16_t time=0;
    for(time=0;time<500;time++)
    {
        MPU_ACC_X += GetData1(ACCEL_XOUT_H);
        MPU_ACC_Y += GetData1(ACCEL_YOUT_H);
        MPU_ACC_Z += GetData1(ACCEL_ZOUT_H) + 2000;
        MPU_GYRO_X += GetData1(GYRO_XOUT_H);
        MPU_GYRO_Y += GetData1(GYRO_YOUT_H);
        MPU_GYRO_Z += GetData1(GYRO_ZOUT_H);
    }
    ACC_X_OFFET = MPU_ACC_X/500;
    ACC_Y_OFFET = MPU_ACC_Y/500;
    ACC_Z_OFFET = MPU_ACC_Z/500;  //不用
    GRYO_X_OFFSET = MPU_GYRO_X/500;
    GRYO_Y_OFFSET = MPU_GYRO_Y/500;
    GRYO_Z_OFFSET = MPU_GYRO_Z/500;
    
}

float ANGLE_X=0,ANGLE_Y=0,ANGLE_Z=0;
int16_t Gyr_Z = 0;

//int16_t gyr_z,max_gyr_z,min_gyr_z;
void Get_Angle(void)
{
    //int16_t acc_x;
	int16_t acc_y, acc_z;
	int16_t gyr_x;//,gyr_zgyr_y;
	//float angle_acc_x;
	float angle_acc_y;
	float angle_delta_y;//angle_delta_x
	
	//读取传感器数据
//	acc_x =(GetData1(ACCEL_XOUT_H) - ACC_X_OFFET);
	acc_y =(GetData1(ACCEL_YOUT_H) - ACC_Y_OFFET);
	acc_z =(GetData1(ACCEL_ZOUT_H) - ACC_Z_OFFET);
	gyr_x =(GetData1(GYRO_XOUT_H) - GRYO_X_OFFSET);
	//gyr_y =(GetData1(GYRO_YOUT_H) - GRYO_Y_OFFSET);
	//Gyr_Z =GetData1(GYRO_ZOUT_H) - GRYO_Z_OFFSET;

	//计算x轴角度
//	angle_acc_x = atan2f(acc_x ,-acc_z) * 57.29578f;	//通过加速度计算得的角度
//	angle_delta_x = gyr_y / 65.53f * 0.01f;
//	ANGLE_X = 0.05f * angle_acc_x + 0.95f * (ANGLE_X + angle_delta_x);
	
	//计算y轴角度
	angle_acc_y = -atan2f(acc_y ,-acc_z) * 57.29578f;	//通过加速度计算得的角度
	angle_delta_y = gyr_x / 65.53f * 0.005f;
	ANGLE_Y = 0.05f * angle_acc_y + 0.95f * (ANGLE_Y + angle_delta_y);
}

static void i2c_release_bus_delay(void)
{
    uint32_t i = 0;
    for (i = 0; i < 100; i++)
    {
        __NOP();
    }
}

void BOARD_I2C_ReleaseBus(void)
{
    uint8_t i = 0;
    gpio_pin_config_t pin_config;
    port_pin_config_t i2c_pin_config = {0};

    /* Config pin mux as gpio */
    i2c_pin_config.pullSelect = kPORT_PullUp;
    i2c_pin_config.mux = kPORT_MuxAsGpio;

    pin_config.pindirection = kGPIO_DigitalOutput;
    pin_config.outputLogic = 1U;
    CLOCK_EnableClock(kCLOCK_PortB);
    PORT_SetPinConfig(PORTB, 2, &i2c_pin_config);
    PORT_SetPinConfig(PORTB, 3, &i2c_pin_config);

    GPIO_PinInit(GPIOB, 2, &pin_config);
    GPIO_PinInit(GPIOB, 3, &pin_config);

    /* Drive SDA low first to simulate a start */
    GPIO_WritePinOutput(GPIOB, 3, 0U);
    i2c_release_bus_delay();

    /* Send 9 pulses on SCL and keep SDA low */
    for (i = 0; i < 9; i++)
    {
        GPIO_WritePinOutput(GPIOB, 2, 0U);
        i2c_release_bus_delay();

        GPIO_WritePinOutput(GPIOB, 3, 1U);
        i2c_release_bus_delay();

        GPIO_WritePinOutput(GPIOB, 2, 1U);
        i2c_release_bus_delay();
        i2c_release_bus_delay();
    }

    /* Send stop */
    GPIO_WritePinOutput(GPIOB, 2, 0U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(GPIOB, 3, 0U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(GPIOB, 2, 1U);
    i2c_release_bus_delay();

    GPIO_WritePinOutput(GPIOB, 3, 1U);
    i2c_release_bus_delay();
}

