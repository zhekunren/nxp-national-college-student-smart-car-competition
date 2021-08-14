#include "fsl_i2c.h"
#include "user_i2c.h"

#define I2C_ACK_OFF                     (0)
#define I2C_ACK_ON                      (1)
//定义主机读写模式 
#define I2C_MWSR                        (0x00)  
#define I2C_MRSW                        (0x01)  

void I2CStartTrans(I2C_Type *base, uint8_t addr, uint8_t mode)
{
	//I2C产生start信号
	I2CStart(I2C0);
	//将从机地址和主机读写位合成一个字节写入
	I2CWriteByte(base, (addr<<1)|mode );
}

void I2CStart(I2C_Type *base)
{
	base->C1 |= I2C_C1_TX_MASK ;
	base->C1 |= I2C_C1_MST_MASK ;
}
void I2CWriteByte(I2C_Type *base, uint8_t data)
{
	base->D = data;
}

void I2C_WaitAck(I2C_Type *base, uint8_t is_wait)
{
	uint16_t timeOut;
	if(is_wait == I2C_ACK_ON)
	{
		while(!(base->S & I2C_S_IICIF_MASK))
		{
		if(timeOut > 50000) //如果等待超时，强行退出
			break;
		else timeOut++;
		}
		base->S |= I2C_S_IICIF_MASK;
	}
	else
	{
		base->C1 |= I2C_C1_TXAK_MASK; 
	}
}

void I2C_ReStart(I2C_Type *base)
{
	base->C1 |= I2C_C1_RSTA_MASK;
}

void I2C_SetMasterWR(I2C_Type *base, uint8_t mode)
{
  if(mode == 0x01) 
    base->C1 &= (~I2C_C1_TX_MASK);
  else
    base->C1 |= ( I2C_C1_TX_MASK);
}
uint8_t I2CReadByte(I2C_Type *base)
{
	return (uint8_t)(base->D); 
}

void I2C_Stop(I2C_Type *base)
{
	base->C1 &=(~I2C_C1_MST_MASK);
	base->C1 &=(~I2C_C1_TX_MASK); 
}


uint8_t I2CReadReg(I2C_Type *base,uint8_t DeviceAdd,uint8_t Reg)
{
	uint8_t result;
	uint16_t timeOut = 0;
	//I2CStartTrans(base, DeviceAdd,I2C_MWSR);
	base->C1 |= I2C_C1_TX_MASK ;
	base->C1 |= I2C_C1_MST_MASK;
	base->D = (DeviceAdd << 1) | I2C_MWSR;
	
	
	//I2C_WaitAck(base, I2C_ACK_ON);
	
	while(!(base->S & I2C_S_IICIF_MASK))
	{
	if(timeOut > 50000) //如果等待超时，强行退出
		break;
	else timeOut++;
	}
	base->S |= I2C_S_IICIF_MASK;
	
	

	//I2CWriteByte(base, Reg);
	base->D = Reg;
	
	
	//I2C_WaitAck(base, I2C_ACK_ON);
	timeOut = 0;
    while(!(base->S & I2C_S_IICIF_MASK))
	{
	if(timeOut > 50000) //如果等待超时，强行退出
		break;
	else timeOut++;
	}
	base->S |= I2C_S_IICIF_MASK;
	
	
	//I2C_ReStart(base);
	base->C1 |= I2C_C1_RSTA_MASK;
	
	
	//I2CWriteByte(base, (DeviceAdd << 1) | I2C_MRSW);
	base->D = (DeviceAdd << 1) | I2C_MRSW;
	
	
	//I2C_WaitAck(base, I2C_ACK_ON);
	timeOut = 0;
    while(!(base->S & I2C_S_IICIF_MASK))
	{
	if(timeOut > 50000) //如果等待超时，强行退出
		break;
	else timeOut++;
	}
	base->S |= I2C_S_IICIF_MASK;
	
	
	//I2C_SetMasterWR(base, I2C_MRSW);
	base->C1 &= (~I2C_C1_TX_MASK);
	
	
	//I2C_WaitAck(base, I2C_ACK_OFF);
	base->C1 |= I2C_C1_TXAK_MASK; 
	
	
	//result = I2CReadByte(base);
	result = base->D;
	
	
	//I2C_WaitAck(base, I2C_ACK_ON);
	timeOut = 0;
    while(!(base->S & I2C_S_IICIF_MASK))
	{
	if(timeOut > 50000) //如果等待超时，强行退出
		break;
	else timeOut++;
	}
	base->S |= I2C_S_IICIF_MASK;
	
	
	//I2C_Stop(base);
	base->C1 &=(~I2C_C1_MST_MASK);
	base->C1 &=(~I2C_C1_TX_MASK);
	
	
	//result = I2CReadByte(base);
	result = base->D;
	//读IIC数据
	while(((I2C0 ->S) & 0x20) == 0x20);
	return result;
	

}

void I2CWriteReg(I2C_Type *base,uint8_t DeviceAdd,uint8_t Reg,uint8_t data)
{
	uint16_t timeOut = 0;
	//发送从机地址
	//I2CStartTrans(base, DeviceAdd, I2C_MWSR);
	base->C1 |= I2C_C1_TX_MASK ;
	base->C1 |= I2C_C1_MST_MASK ;
	base->D = (DeviceAdd << 1) | I2C_MWSR;
	
	
	//I2C_WaitAck(base, I2C_ACK_ON);
	while(!(base->S & I2C_S_IICIF_MASK))
	{
	if(timeOut > 50000) //如果等待超时，强行退出
		break;
	else timeOut++;
	}
	base->S |= I2C_S_IICIF_MASK;


	//I2CWriteByte(base, Reg);
	base->D = Reg;
	
	
	//I2C_WaitAck(base, I2C_ACK_ON);
	timeOut = 0;
    while(!(base->S & I2C_S_IICIF_MASK))
	{
	if(timeOut > 50000) //如果等待超时，强行退出
		break;
	else timeOut++;
	}
	base->S |= I2C_S_IICIF_MASK;
	//向寄存器中写具体数据
	
	//I2CWriteByte(base, data);
	base->D = data;
	
	//I2C_WaitAck(base, I2C_ACK_ON);
	timeOut = 0;
	 while(!(base->S & I2C_S_IICIF_MASK))
	{
	if(timeOut > 50000) //如果等待超时，强行退出
		break;
	else timeOut++;
	}
	base->S |= I2C_S_IICIF_MASK;
	
	
	//I2C_Stop(base);
	base->C1 &=(~I2C_C1_MST_MASK);
	base->C1 &=(~I2C_C1_TX_MASK);
	

	//Delay(1);
	while(((I2C0 ->S) & 0x20) == 0x20);
	
}

void Delay(uint8_t time)
{
	int16_t i = 0,j = 0;
	for(i = 0;i <= 200; i++)
		for(j = 0;j <= time; j++)
	{
		__ASM ("nop");
	}
}
