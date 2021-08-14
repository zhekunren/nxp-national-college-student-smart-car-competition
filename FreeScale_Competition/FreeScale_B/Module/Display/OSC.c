#include"include.h"
 int16_t  OutData[4];  //上位机示波器
//int16  OutData[4];
//=====================================================================================
//函数名:      SciFrameTx1
//功  能:      示波器1封装   xikeda开发软件
//说  明：
//修改注释：
//=====================================================================================

/*
void SciFrameTx(long  Data)
{
  unsigned char ch_Temp = 0;
	unsigned char Temp_A_L;				//必须为Unsignged
	signed char Temp_A_H;
	signed char Temp_B_L;
	signed char Temp_B_H;
	Temp_A_L = (Data >>0)&0x000000ff;
	Temp_A_H = (Data >>8)&0x000000ff;
	Temp_B_L = (Data >>16)&0x000000ff;
	Temp_B_H = (Data >>24)&0x000000ff;

        LPLD_UART_PutChar(UART0,0xaa);
        LPLD_UART_PutChar(UART0,0x09);
        LPLD_UART_PutChar(UART0,0x01);

	ch_Temp ^= 0x09;
	ch_Temp ^= 0x01;
	ch_Temp ^= Temp_B_H;/////按位异或
	ch_Temp ^= Temp_B_L;/////相同为0不同位1
	ch_Temp ^= Temp_A_H;
	ch_Temp ^= Temp_A_L;
LPLD_UART_PutChar(UART0,Temp_A_L);
LPLD_UART_PutChar(UART0,Temp_A_H);
LPLD_UART_PutChar(UART0,Temp_B_L);
LPLD_UART_PutChar(UART0,Temp_B_H);
LPLD_UART_PutChar(UART0,ch_Temp);
LPLD_UART_PutChar(UART0,0x55);
}*/

//=====================================================================================
//函数名:     1: CRC_CHECK   2: OutPut_Data
//功  能:      示波器2封装
//说  明：
//修改注释：
//=====================================================================================
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

unsigned char databuf[10];
/////////////////////////////////////////////////////////

void OutPut_Data(void)
{
  unsigned int temp;
  unsigned int i;
  unsigned short CRC16;
  for(i=0;i<4;i++)
   {
    temp =(uint32_t)OutData[i];
    databuf[i*2]   = (unsigned char)(temp%256);
    databuf[i*2+1] = (unsigned char)(temp/256);
   }
  CRC16 = CRC_CHECK(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;
   for(i=0;i<10;i++)
   UART_WriteBlocking(SET_Uartx, &databuf[i],1);
}
/*********************************************************
      山外上位机接口程序
*********************************************************/ 
void vcan_sendimg(void *imgaddr, uint32_t imgsize)
{
#define CMD_IMG     1
    int8_t cmdf[2] = {CMD_IMG, ~CMD_IMG};    //山外上位机 使用的命令
    int8_t cmdr[2] = {~CMD_IMG, CMD_IMG};    //山外上位机 使用的命令

    UART_WriteBlocking(SET_Uartx  , (const uint8_t*)(cmdf), sizeof(cmdf));    //先发送命令

    UART_WriteBlocking(SET_Uartx  , imgaddr, imgsize); //再发送图像

    UART_WriteBlocking(SET_Uartx  , (const uint8_t*)(cmdr), sizeof(cmdr));    //先发送命令
}

/*********************************************************
      图像解压函数  上位机发送
*********************************************************/ 
void img_extract(uint8_t *dst, uint8_t *src, uint32_t srclen)
{
    uint8_t colour[2] = {254, 0};            //255 白色 和 0 黑色    为了适应智能车调试助手
                                          //
    uint8_t tmpsrc;
    //src++;
    while(srclen --)
    {        
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];               //二值化后  0白色 1黑色 
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}

/*********************************************************
      图像解压函数  oled发送
*********************************************************/ 
void img_extract1(uint8_t *dst, uint8_t *src, uint32_t srclen)
{
    uint8_t colour[2] = {1,0};            //255 白色 和 0 黑色    为了适应智能车调试助手
                                          //
    uint8_t tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];               //二值化后  0白色 1黑色 
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
    
}
