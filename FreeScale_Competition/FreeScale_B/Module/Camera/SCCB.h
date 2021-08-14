#ifndef _SCCB_H_
#define _SCCB_H_

//SCCBπ‹Ω≈≈‰÷√
#define SCCB_SCL  PTC16
#define SCCB_SDA  PTC17

#define SCL_H      PTC16_O=1  //GPIO_SetPinsOutput(PTC, 1<<16)
#define SCL_L      PTC16_O=0    //GPIO_SetPinsOutput(PTC, 0<<16)
#define SCL_DDR_O  DDRC16=1
#define SCL_DDR_I  DDRC16=0

#define SDA_H      PTC17_O=1  //GPIO_SetPinsOutput(PTC, 1<<17)
#define SDA_L      PTC17_O=0    //GPIO_SetPinsOutput(PTC, 0<<17)
#define SDA_DDR_O  DDRC17=1
#define SDA_DDR_I  DDRC17=0
#define SDA_IN     PTC17_I  //GPIO_ReadPinInput(PTC, 17)

#define ADDR_OV7725  0x42

#define DEV_ADR    ADDR_OV7725

#define SCCB_DELAY()   SCCB_delay(400)

extern void SCCB_GPIO_Init(void);
extern int SCCB_WriteByte(uint16_t WriteAddress , uint8_t SendByte);
extern int SCCB_ReadByte(uint8_t *pBuffer , uint16_t length , uint8_t ReadAddress);

#endif
