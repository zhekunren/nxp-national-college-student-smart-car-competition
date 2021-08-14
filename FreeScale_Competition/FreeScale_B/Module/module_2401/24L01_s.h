#ifndef _24L01_s_h
#define _24L01_s_h

/*******2401shou**********/
int16_t SPI_RW_s(int16_t byteg); 
int16_t SPI_RW_Reg_s(int16_t reg, int16_t value); 
int16_t SPI_Read_s(int16_t reg); 
int16_t SPI_Read_Buf_s(int16_t reg, int16_t *pBuf, int16_t bytes); 
int16_t SPI_Write_Buf_s(int16_t reg, int16_t *pBuf, int16_t bytes); 

extern int16_t RX_TAB_s[8];
void init_io_s(void);
void RX_Mode_s(void); 
int16_t NRF24L01_RxPacket_s(int16_t *rxbuf);
int16_t NRF24L01_Check_s(void);

#endif

