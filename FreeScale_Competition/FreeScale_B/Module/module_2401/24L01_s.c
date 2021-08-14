#include "include.h"
 
//----------------------------------Ӳ����ַ����-----------------------
#define CE_s    PTE0_O
#define CSN_s   PTE1_O
#define SCK_s   PTE2_O
#define MOSI_s  PTE3_O
#define MISO_s  PTE4_I
#define IRQ_s   PTE5_I

//д�˿�
#define NRF24L01_MOSI_H_s    MOSI_s=1
#define NRF24L01_MOSI_L_s    MOSI_s=0   
#define NRF24L01_SCK_H_s     SCK_s=1   
#define NRF24L01_SCK_L_s     SCK_s=0 
#define NRF24L01_CE_H_s      CE_s=1
#define NRF24L01_CE_L_s      CE_s=0 
#define NRF24L01_CSN_H_s     CSN_s=1
#define NRF24L01_CSN_L_s     CSN_s=0 
//���˿�
#define NRF24L01_MISO_s      PTE4_I
#define NRF24L01_IRQ_s       PTE5_I
//-----------------------------------��������-----------------------------------------------
#define TX_ADR_WIDTH_s    5   	// 5 uints TX address width
#define RX_ADR_WIDTH_s    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH_s  8 	// 20 uints TX payload
#define RX_PLOAD_WIDTH_s  8	// 20 uints TX payload

int16_t  TX_ADDRESS_s[TX_ADR_WIDTH_s]= {0x19,0x97,0x02,0x06,0x55};	//���ص�ַ
int16_t  RX_ADDRESS_s[RX_ADR_WIDTH_s]= {0x19,0x97,0x02,0x06,0x55};	//���յ�ַ

int16_t  RX_TAB_s[8]={0};
//-------------------------------------��ر�־����-----------------------------------------
#define MAX_TX_s  	0x10  //�ﵽ����ʹ����ж�
#define TX_OK_s   	0x20  //TX��������ж�
#define RX_OK_s   	0x40  //���յ������ж�
//-----------------------------------NRF24L01 �Ĵ���ָ��------------------------------------
#define READ_REG_s        0x00  //���Ĵ���ָ��// Define read command to register  
#define WRITE_REG_s       0x20  //д�Ĵ���ָ��// Define write command to register  
#define RD_RX_PLOAD_s     0x61  //��ȡ��������ָ��// Define RX payload register address 
#define WR_TX_PLOAD_s     0xA0  //д��������ָ��// Define TX payload register address 
#define FLUSH_TX_s        0xE1  //ˢ�·��� FIFO ָ��// Define flush TX register command  
#define FLUSH_RX_s        0xE2  //ˢ�½��� FIFO ָ��// Define flush RX register command 
#define REUSE_TX_PL_s     0xE3  //�����ظ�װ������ָ��// Define reuse TX payload register command 
#define NOP_s             0xFF  //����// Define No Operation, might be used to read status register  
//----------------------------------SPI(nRF24L01) �Ĵ�����ַ--------------------------------------
#define CONFI_s          0x00  //�����շ�״̬��CRC У��ģʽ�Լ��շ�״̬��Ӧ��ʽ// 'Config' register address 
#define EN_AA_s           0x01  //�Զ�Ӧ��������// 'Enable Auto Acknowledgment' register address 
#define EN_RXADDR_s       0x02  //�����ŵ�����// 'Enabled RX addresses' register address 
#define SETUP_AW_s        0x03  //�շ���ַ�������// 'Setup address width' register address 
#define SETUP_RETR_s      0x04  //�Զ��ط���������// 'Setup Auto. Retrans' register address 
#define RF_CH_s           0x05  //����Ƶ������// 'RF channel' register address 
#define RF_SETUP_s        0x06  //�������ʡ����Ĺ�������// 'RF setup' register address 
#define STATUS_s          0x07  //״̬�Ĵ���// 'Status' register address 
#define OBSERVE_TX_s      0x08  //���ͼ�⹦��// 'Observe TX' register address 
#define CD_s              0x09  //��ַ��� // 'Carrier Detect' register address 
#define RX_ADDR_P0_s      0x0A  //Ƶ��0 �������ݵ�ַ// 'RX address pipe0' register address 
#define RX_ADDR_P1_s      0x0B  //Ƶ��1 �������ݵ�ַ// 'RX address pipe1' register address 
#define RX_ADDR_P2_s      0x0C  //Ƶ��2 �������ݵ�ַ // 'RX address pipe2' register address 
#define RX_ADDR_P3_s      0x0D  //Ƶ��3 �������ݵ�ַ// 'RX address pipe3' register address 
#define RX_ADDR_P4_s      0x0E  //Ƶ��4 �������ݵ�ַ// 'RX address pipe4' register address 
#define RX_ADDR_P5_s      0x0F  //Ƶ��5 �������ݵ�ַ// 'RX address pipe5' register address 
#define TX_ADDR_s         0x10  //���͵�ַ�Ĵ���// 'TX address' register address 
#define RX_PW_P0_s        0x11  //����Ƶ��0 �������ݳ���// 'RX payload width, pipe0' register address 
#define RX_PW_P1_s        0x12  //����Ƶ��1 �������ݳ���// 'RX payload width, pipe1' register address 
#define RX_PW_P2_s        0x13  //����Ƶ��2 �������ݳ���// 'RX payload width, pipe2' register address 
#define RX_PW_P3_s        0x14  //����Ƶ��3 �������ݳ���// 'RX payload width, pipe3' register address 
#define RX_PW_P4_s        0x15  //����Ƶ��4 �������ݳ���// 'RX payload width, pipe4' register address 
#define RX_PW_P5_s        0x16  //����Ƶ��5 �������ݳ���// 'RX payload width, pipe5' register address 
#define FIFO_STATUS_s     0x17  //FIFOջ��ջ��״̬�Ĵ�������// 'FIFO Status Register' register address 

/*********************************************************
�������ƣ�init_io
��    �ܣ���ʼ��I/O��
��    ������
����ֵ  ����
*********************************************************/
void init_io_s()
{

           
    NRF24L01_CE_L_s;  // ����        
    NRF24L01_CSN_H_s; // SPI��ֹ
    NRF24L01_SCK_L_s; // SPIʱ���õ�
    
//    ���?
//    NRF24L01_IRQ = 1;
    //    
}
/*********************************************************
�������ƣ�SPI_RW
��    �ܣ���� GPIOģ�� SPI �Ĺ���
��    ����byte--д��SPI�����ϵ�һ���ֽ�����
����ֵ  ����ȡSPI�����ϵ�һ���ֽ�����
*********************************************************/
int16_t SPI_RW_s(int16_t byteg) 
{ 
    int16_t bit_ctr; 
    for(bit_ctr=0;bit_ctr<8;bit_ctr++)     
    { 
        if(byteg & 0x80)  NRF24L01_MOSI_H_s;
        
        else             NRF24L01_MOSI_L_s;
        
        byteg = (byteg << 1);
        
        NRF24L01_SCK_H_s;
        
        if(NRF24L01_MISO_s) byteg |= 1;
        
        NRF24L01_SCK_L_s;
        
    } 
    return(byteg);                          
} 
/*********************************************************
�������ƣ�SPI_RW_Reg
��    �ܣ��������� 24L01 �ļĴ�����ֵ
��    ����reg--Ҫд�����ݵļĴ�����ַ
          value--Ҫд�������
����ֵ  ���õ�ַ��ԭ��ֵ
*********************************************************/
int16_t SPI_RW_Reg_s(int16_t reg, int16_t value) 
{ 
    int16_t status; 
    NRF24L01_CSN_L_s;            
    status = SPI_RW_s(reg);      
    SPI_RW_s(value);             
    NRF24L01_CSN_H_s;            
    return(status);           
}
/*********************************************************
�������ƣ�SPI_Read
��    �ܣ���ȡ 24L01 �ļĴ�����ֵ
��    ����reg--Ҫ��ȡ���ݵļĴ�����ַ
����ֵ  ���õ�ַ��ֵ
*********************************************************/
int16_t SPI_Read_s(int16_t reg) 
{ 
    int16_t reg_val; 
    NRF24L01_CSN_L_s;          
    SPI_RW_s(reg);             
    reg_val = SPI_RW_s(0);     
    NRF24L01_CSN_H_s;          
    return(reg_val);         
}
/*********************************************************
�������ƣ�SPI_Read_Buf
��    �ܣ���Ҫ�����ڽ���ʱ��ȡ FIFO �������е�ֵ
��    ����reg--Ҫ��ȡ���ݵļĴ�����ַ
          *pBuf--����練�����
          bytes--��ȡ���ݵĳ���
����ֵ  ���õ�ַ��ԭ��ֵ
*********************************************************/
int16_t SPI_Read_Buf_s(int16_t reg, int16_t *pBuf, int16_t bytes) 
{ 
    int16_t status,byte_ctr; 
 
    NRF24L01_CSN_L_s;                           
    status = SPI_RW_s(reg);                     
    for(byte_ctr=0;byte_ctr<bytes;byte_ctr++) 
    pBuf[byte_ctr] = SPI_RW_s(0);               
    NRF24L01_CSN_H_s;                           
    return(status);                           
} 
/*********************************************************
�������ƣ�SPI_Write_Buf
��    �ܣ���Ҫ����������������ŵ����� FIFO��������
��    ����reg--Ҫд�����ݵļĴ�����ַ
          *pBuf--���ݻ�������
          bytes--д�����ݵĳ���
����ֵ  ���õ�ַ��ԭ��ֵ
*********************************************************/
int16_t SPI_Write_Buf_s(int16_t reg, int16_t *pBuf, int16_t bytes) 
{ 
  int16_t status,byte_ctr; 
 
    NRF24L01_CSN_L_s;                             
    status = SPI_RW_s(reg);                        
    for(byte_ctr=0; byte_ctr<bytes; byte_ctr++)  
    SPI_RW_s(*pBuf++); 
    NRF24L01_CSN_H_s;                             
    return(status);                             
}

/*********************************************************
�������ƣ�RX_Mode
��    �ܣ�
��    ����
����ֵ  ��
*********************************************************/
void RX_Mode_s(void) 
{ 
    NRF24L01_SCK_L_s;
         NRF24L01_CE_L_s; 
    	SPI_Write_Buf_s(WRITE_REG_s + RX_ADDR_P0_s, TX_ADDRESS_s, TX_ADR_WIDTH_s); // Use the same address on the RX device as the TX device

 	SPI_RW_Reg_s(WRITE_REG_s + EN_AA_s, 0x01);      // Enable Auto.Ack:Pipe0
 	SPI_RW_Reg_s(WRITE_REG_s + EN_RXADDR_s, 0x01);  // Enable Pipe0
  	SPI_RW_Reg_s(WRITE_REG_s + RF_CH_s, 0);        // Select RF channel 40  ��Ϊ0 �иĶ�
  	SPI_RW_Reg_s(WRITE_REG_s + RX_PW_P0_s, TX_PLOAD_WIDTH_s); // Select same RX payload width as TX Payload width
  	SPI_RW_Reg_s(WRITE_REG_s + RF_SETUP_s, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  	SPI_RW_Reg_s(WRITE_REG_s + CONFI_s, 0x0f);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..
        
  	                        
    NRF24L01_CE_H_s; //CEΪ��,10us����������
 
}		

/*********************************************************
�������ƣ�NRF24L01_Check
��    �ܣ����24L01�Ƿ����
��    ������
����ֵ  ��0���ɹ�;1��ʧ��
*********************************************************/
int16_t buf_s[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
int16_t NRF24L01_Check_s(void)
{
	
	int16_t i; 	 
	SPI_Write_Buf_s(WRITE_REG_s+TX_ADDR_s,buf_s,5);//д��5���ֽڵĵ�ַ.	
	SPI_Read_Buf_s(TX_ADDR_s,buf_s,5); 
        //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf_s[i]!=0XA5) break;	 							   
	if(i!=5)return 0;//���24L01����	
	return 1;        //��⵽24L01
}	
/*********************************************************
�������ƣ�NRF24L01_Check
��    �ܣ�����NRF24L01����һ������
��    ����txbuf:�����������׵�ַ
����ֵ  ��0��������ɣ��������������
*********************************************************/
int16_t NRF24L01_RxPacket_s(int16_t *rxbuf)
{
	int16_t sta,tt=0; 
       
        //if(!( NRF24L01_IRQ))
        {
	sta=SPI_Read_s(STATUS_s);  //��ȡ״̬�Ĵ�����ֵ    	 
	if(sta&RX_OK_s)//���յ�����
	{
              //�˴��иĶ�  RX_TAB
	      SPI_Read_Buf_s(RD_RX_PLOAD_s,RX_TAB_s,RX_PLOAD_WIDTH_s);//��ȡ����
	      SPI_RW_Reg_s(FLUSH_RX_s,0xff);//���RX FIFO�Ĵ��� 
	      SPI_RW_Reg_s(WRITE_REG_s+STATUS_s,0x7f); //���TX_DS��MAX_RT�жϱ�־
              tt=1;
              
	}
        }
        return (tt);
}	

