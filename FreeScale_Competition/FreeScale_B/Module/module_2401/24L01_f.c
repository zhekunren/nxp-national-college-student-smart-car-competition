#include "include.h"
 

//----------------------------------Ӳ����ַ����-----------------------
#define CE_f    PTD10_O
#define CSN_f   PTD11_O
#define SCK_f   PTD12_O
#define MOSI_f  PTD13_O
#define MISO_f  PTD14_I
#define IRQ_f   PTD15_I

//д�˿�
#define NRF24L01_MOSI_H_f    MOSI_f=1
#define NRF24L01_MOSI_L_f    MOSI_f=0   
#define NRF24L01_SCK_H_f    SCK_f=1   
#define NRF24L01_SCK_L_f    SCK_f=0 
#define NRF24L01_CE_H_f      CE_f=1
#define NRF24L01_CE_L_f      CE_f=0 
#define NRF24L01_CSN_H_f     CSN_f=1
#define NRF24L01_CSN_L_f     CSN_f=0 
//���˿�
#define NRF24L01_MISO_f     PTD14_I
#define NRF24L01_IRQ_f      PTD15_I
//-----------------------------------��������-----------------------------------------------
#define TX_ADR_WIDTH_f    5   	// 5 uints TX address width
#define RX_ADR_WIDTH_f    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH_f  8 	// 20 uints TX payload
#define RX_PLOAD_WIDTH_f  8	// 20 uints TX payload

int16_t  TX_ADDRESS_f[TX_ADR_WIDTH_f]= {0x20,0x18,0x03,0x18,0x55};	//���ص�ַ
//int16_t  RX_ADDRESS_s[RX_ADR_WIDTH]= {0x19,0x97,0x02,0x06,0x55};	//���յ�ַ
int16_t  TX_TAB_f[8]={0};

//-------------------------------------��ر�־����-----------------------------------------
#define MAX_TX_f  	0x10  //�ﵽ����ʹ����ж�
#define TX_OK_f   	0x20  //TX��������ж�
#define RX_OK_f   	0x40  //���յ������ж�
//-----------------------------------NRF24L01 �Ĵ���ָ��------------------------------------
#define READ_REG_f        0x00  //���Ĵ���ָ��// Define read command to register  
#define WRITE_REG_f       0x20  //д�Ĵ���ָ��// Define write command to register  
#define RD_RX_PLOAD_f     0x61  //��ȡ��������ָ��// Define RX payload register address 
#define WR_TX_PLOAD_f     0xA0  //д��������ָ��// Define TX payload register address 
#define FLUSH_TX_f        0xE1  //ˢ�·��� FIFO ָ��// Define flush TX register command  
#define FLUSH_RX_f        0xE2  //ˢ�½��� FIFO ָ��// Define flush RX register command 
#define REUSE_TX_PL_f     0xE3  //�����ظ�װ������ָ��// Define reuse TX payload register command 
#define NOP_f             0xFF  //����// Define No Operation, might be used to read status register  
//----------------------------------SPI(nRF24L01) �Ĵ�����ַ--------------------------------------
#define CONFI_f          0x00  //�����շ�״̬��CRC У��ģʽ�Լ��շ�״̬��Ӧ��ʽ// 'Config' register address 
#define EN_AA_f           0x01  //�Զ�Ӧ��������// 'Enable Auto Acknowledgment' register address 
#define EN_RXADDR_f       0x02  //�����ŵ�����// 'Enabled RX addresses' register address 
#define SETUP_AW_f        0x03  //�շ���ַ�������// 'Setup address width' register address 
#define SETUP_RETR_f      0x04  //�Զ��ط���������// 'Setup Auto. Retrans' register address 
#define RF_CH_f           0x05  //����Ƶ������// 'RF channel' register address 
#define RF_SETUP_f        0x06  //�������ʡ����Ĺ�������// 'RF setup' register address 
#define STATUS_f          0x07  //״̬�Ĵ���// 'Status' register address 
#define OBSERVE_TX_f      0x08  //���ͼ�⹦��// 'Observe TX' register address 
#define CD_f              0x09  //��ַ��� // 'Carrier Detect' register address 
#define RX_ADDR_P0_f      0x0A  //Ƶ��0 �������ݵ�ַ// 'RX address pipe0' register address 
#define RX_ADDR_P1_f      0x0B  //Ƶ��1 �������ݵ�ַ// 'RX address pipe1' register address 
#define RX_ADDR_P2_f      0x0C  //Ƶ��2 �������ݵ�ַ // 'RX address pipe2' register address 
#define RX_ADDR_P3_f      0x0D  //Ƶ��3 �������ݵ�ַ// 'RX address pipe3' register address 
#define RX_ADDR_P4_f      0x0E  //Ƶ��4 �������ݵ�ַ// 'RX address pipe4' register address 
#define RX_ADDR_P5_f      0x0F  //Ƶ��5 �������ݵ�ַ// 'RX address pipe5' register address 
#define TX_ADDR_f         0x10  //���͵�ַ�Ĵ���// 'TX address' register address 
#define RX_PW_P0_f        0x11  //����Ƶ��0 �������ݳ���// 'RX payload width, pipe0' register address 
#define RX_PW_P1_f        0x12  //����Ƶ��1 �������ݳ���// 'RX payload width, pipe1' register address 
#define RX_PW_P2_f        0x13  //����Ƶ��2 �������ݳ���// 'RX payload width, pipe2' register address 
#define RX_PW_P3_f        0x14  //����Ƶ��3 �������ݳ���// 'RX payload width, pipe3' register address 
#define RX_PW_P4_f        0x15  //����Ƶ��4 �������ݳ���// 'RX payload width, pipe4' register address 
#define RX_PW_P5_f        0x16  //����Ƶ��5 �������ݳ���// 'RX payload width, pipe5' register address 
#define FIFO_STATUS_f     0x17  //FIFOջ��ջ��״̬�Ĵ�������// 'FIFO Status Register' register address 

/*********************************************************
�������ƣ�init_io
��    �ܣ���ʼ��I/O��
��    ������
����ֵ  ����
*********************************************************/
void init_io_f(void)
{

           
    NRF24L01_CE_L_f;  // ����        
    NRF24L01_CSN_H_f; // SPI��ֹ
    NRF24L01_SCK_L_f; // SPIʱ���õ�
    
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
int16_t SPI_RW_f(int16_t byteg) 
{ 
    int16_t bit_ctr; 
    for(bit_ctr=0;bit_ctr<8;bit_ctr++)     
    { 
        if(byteg & 0x80)  NRF24L01_MOSI_H_f;
        
        else             NRF24L01_MOSI_L_f;
        
        byteg = (byteg << 1);
        
        NRF24L01_SCK_H_f;
        
        if(NRF24L01_MISO_f) byteg |= 1;
        
        NRF24L01_SCK_L_f;
        
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
int16_t SPI_RW_Reg_f(int16_t reg, int16_t value) 
{ 
    int16_t status; 
    NRF24L01_CSN_L_f;            
    status = SPI_RW_f(reg);      
    SPI_RW_f(value);             
    NRF24L01_CSN_H_f;            
    return(status);           
}
/*********************************************************
�������ƣ�SPI_Read
��    �ܣ���ȡ 24L01 �ļĴ�����ֵ
��    ����reg--Ҫ��ȡ���ݵļĴ�����ַ
����ֵ  ���õ�ַ��ֵ
*********************************************************/
int16_t SPI_Read_f(int16_t reg) 
{ 
    int16_t reg_val; 
    NRF24L01_CSN_L_f;          
    SPI_RW_f(reg);             
    reg_val = SPI_RW_f(0);     
    NRF24L01_CSN_H_f;          
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
int16_t SPI_Read_Buf_f(int16_t reg, int16_t *pBuf, int16_t bytes) 
{ 
    int16_t status,byte_ctr; 
 
    NRF24L01_CSN_L_f;                           
    status = SPI_RW_f(reg);                     
    for(byte_ctr=0;byte_ctr<bytes;byte_ctr++) 
    pBuf[byte_ctr] = SPI_RW_f(0);               
    NRF24L01_CSN_H_f;                           
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
int16_t SPI_Write_Buf(int16_t reg, int16_t *pBuf, int16_t bytes) 
{ 
  int16_t status,byte_ctr; 
 
    NRF24L01_CSN_L_f;                             
    status = SPI_RW_f(reg);                        
    for(byte_ctr=0; byte_ctr<bytes; byte_ctr++)  
    SPI_RW_f(*pBuf++); 
    NRF24L01_CSN_H_f;                             
    return(status);                             
}
	
/*********************************************************
�������ƣ�TX_Mode
��    �ܣ��ú�����ʼ��NRF24L01��TXģʽ
          ����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
          PWR_UP,CRCʹ��
          ��CE��ߺ����10us,�������������������
��    ����
����ֵ  ��
*********************************************************/
void TX_Mode_f(void) 
{ 
      	 NRF24L01_SCK_L_f;
         NRF24L01_CE_L_f; 
  	SPI_Write_Buf(WRITE_REG_f + TX_ADDR_f, TX_ADDRESS_f, TX_ADR_WIDTH_f);    // Writes TX_Address to nRF24L01
  	SPI_Write_Buf(WRITE_REG_f + RX_ADDR_P0_f, TX_ADDRESS_f, TX_ADR_WIDTH_f); // RX_Addr0 same as TX_Adr for Auto.Ack
//���
//  	SPI_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	
//end
  	SPI_RW_Reg_f(WRITE_REG_f + EN_AA_f, 0x01);      // Enable Auto.Ack:Pipe0
  	SPI_RW_Reg_f(WRITE_REG_f + EN_RXADDR_f, 0x01);  // Enable Pipe0
  	SPI_RW_Reg_f(WRITE_REG_f + SETUP_RETR_f, 0x1a); // 500us + 86us, 10 retrans...
  	//SPI_RW_Reg(WRITE_REG_f + RF_CH_f, 40);        // Select RF channel 40
  	SPI_RW_Reg_f(WRITE_REG_f + RF_CH_f, 0); //2.4ghz//�Ķ������  ԭ�����Ͼ�
        SPI_RW_Reg_f(WRITE_REG_f + RF_SETUP_f, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  	SPI_RW_Reg_f(WRITE_REG_f + CONFI_f, 0x0e);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled..	
	 NRF24L01_CE_H_f; 
        
}
/*********************************************************
�������ƣ�NRF24L01_Check
��    �ܣ����24L01�Ƿ����
��    ������
����ֵ  ��0���ɹ�;1��ʧ��
*********************************************************/
int16_t buf_f[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
int16_t NRF24L01_Check_f(void)
{
	
	int16_t i; 	 
	SPI_Write_Buf(WRITE_REG_f+TX_ADDR_f,buf_f,5);//д��5���ֽڵĵ�ַ.	
	SPI_Read_Buf_f(TX_ADDR_f,buf_f,5); 
        //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf_f[i]!=0XA5) break;	 							   
	if(i!=5)return 0;//���24L01����	
	return 1;        //��⵽24L01
}	
/*********************************************************
�������ƣ�NRF24L01_TxPacket
��    �ܣ�����NRF24L01����һ������
��    ����txbuf:�����������׵�ַ
����ֵ  ���������״��
*********************************************************/
int16_t NRF24L01_TxPacket_f(int16_t *txbuf)
{					
	int16_t sta;
        NRF24L01_CE_L_f;
        //�иĶ�   TX_TAB
  	SPI_Write_Buf(WR_TX_PLOAD_f,TX_TAB_f,TX_PLOAD_WIDTH_f);//д���ݵ�TX BUF  32���ֽ�
 	NRF24L01_CE_H_f;//��������	   
	//while(NRF24L01_IRQ);//�ȴ��������
	sta=SPI_Read_f(STATUS_f);  //��ȡ״̬�Ĵ�����ֵ
        //�иĶ�  ԭ���˾�Ϊ����״̬       
	SPI_RW_Reg_f(WRITE_REG_f+STATUS_f,sta); 
        //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX_f)//�ﵽ����ط�����sta&MAX_TX
	{
	    SPI_RW_Reg_f(FLUSH_TX_f,0xff);//���TX FIFO�Ĵ��� 
	    return MAX_TX_f; 
            
	}
        
	if(sta&TX_OK_f)//�������
	{   
            
            return 0x01; 
          
	}
	return 0;//����ԭ����ʧ��
}

