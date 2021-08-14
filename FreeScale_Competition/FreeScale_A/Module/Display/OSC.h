#ifndef _OSC_H_
#define _OSC_H_

//
//volatile int  OutData[4];  //示波器
//--------串口示波器1数据封装发送------//

//void SciFrameTx(long  Data) ;
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);
//--------串口示波器2数据封装发送------//


#define SET_Uartx UART4
void OutPut_Data(void);
extern int16_t  OutData[4];//修改点

void vcan_sendimg(void *imgaddr, uint32_t imgsize);
void img_extract(uint8_t *dst, uint8_t *src, uint32_t srclen);            
void img_extract1(uint8_t *dst, uint8_t *src, uint32_t srclen);   
#endif
