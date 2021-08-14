#ifndef _USER_I2C_H
#define _USER_I2C_H

#include <stdint.h>



void I2CStartTrans(I2C_Type *base, uint8_t addr, uint8_t mode);
void I2CStart(I2C_Type *base);
void I2CWriteByte(I2C_Type *base, uint8_t data);
void I2C_WaitAck(I2C_Type *base, uint8_t is_wait);
void I2C_ReStart(I2C_Type *base);
void I2C_SetMasterWR(I2C_Type *base, uint8_t mode);
uint8_t I2CReadByte(I2C_Type *base);
void I2C_Stop(I2C_Type *base);
uint8_t I2CReadReg(I2C_Type *base,uint8_t DeviceAdd,uint8_t Reg);
void I2CWriteReg(I2C_Type *base,uint8_t DeviceAdd,uint8_t Reg,uint8_t data);
void Delay(uint8_t time);
#endif
