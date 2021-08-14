#include"include.h"
 int16_t  OutData[4];  //��λ��ʾ����
//int16  OutData[4];
//=====================================================================================
//������:      SciFrameTx1
//��  ��:      ʾ����1��װ   xikeda�������
//˵  ����
//�޸�ע�ͣ�
//=====================================================================================

/*
void SciFrameTx(long  Data)
{
  unsigned char ch_Temp = 0;
	unsigned char Temp_A_L;				//����ΪUnsignged
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
	ch_Temp ^= Temp_B_H;/////��λ���
	ch_Temp ^= Temp_B_L;/////��ͬΪ0��ͬλ1
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
//������:     1: CRC_CHECK   2: OutPut_Data
//��  ��:      ʾ����2��װ
//˵  ����
//�޸�ע�ͣ�
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
      ɽ����λ���ӿڳ���
*********************************************************/ 
void vcan_sendimg(void *imgaddr, uint32_t imgsize)
{
#define CMD_IMG     1
    int8_t cmdf[2] = {CMD_IMG, ~CMD_IMG};    //ɽ����λ�� ʹ�õ�����
    int8_t cmdr[2] = {~CMD_IMG, CMD_IMG};    //ɽ����λ�� ʹ�õ�����

    UART_WriteBlocking(SET_Uartx  , (const uint8_t*)(cmdf), sizeof(cmdf));    //�ȷ�������

    UART_WriteBlocking(SET_Uartx  , imgaddr, imgsize); //�ٷ���ͼ��

    UART_WriteBlocking(SET_Uartx  , (const uint8_t*)(cmdr), sizeof(cmdr));    //�ȷ�������
}

/*********************************************************
      ͼ���ѹ����  ��λ������
*********************************************************/ 
void img_extract(uint8_t *dst, uint8_t *src, uint32_t srclen)
{
    uint8_t colour[2] = {254, 0};            //255 ��ɫ �� 0 ��ɫ    Ϊ����Ӧ���ܳ���������
                                          //
    uint8_t tmpsrc;
    //src++;
    while(srclen --)
    {        
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];               //��ֵ����  0��ɫ 1��ɫ 
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
      ͼ���ѹ����  oled����
*********************************************************/ 
void img_extract1(uint8_t *dst, uint8_t *src, uint32_t srclen)
{
    uint8_t colour[2] = {1,0};            //255 ��ɫ �� 0 ��ɫ    Ϊ����Ӧ���ܳ���������
                                          //
    uint8_t tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];               //��ֵ����  0��ɫ 1��ɫ 
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
    
}
