#ifndef _24L01_f_h
#define _24L01_f_h

/*******2401fa******/
int16_t SPI_RW_f(int16_t byteg); 
int16_t SPI_RW_Reg_f(int16_t reg, int16_t value); 
int16_t SPI_Read_f(int16_t reg); 
int16_t SPI_Read_Buf_f(int16_t reg, int16_t *pBuf, int16_t bytes); 
int16_t SPI_Write_Buf(int16_t reg, int16_t *pBuf, int16_t bytes);



extern int16_t TX_TAB_f[8],self_car,stop;
void TX_Mode_f(void); 
int16_t NRF24L01_Check_f(void);
void init_io_f(void);
int16_t NRF24L01_TxPacket_f(int16_t *txbuf);
extern int Count_yuanhuan;
#endif

