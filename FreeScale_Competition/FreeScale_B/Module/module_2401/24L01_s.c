#include "include.h"
 
//----------------------------------硬件地址配置-----------------------
#define CE_s    PTE0_O
#define CSN_s   PTE1_O
#define SCK_s   PTE2_O
#define MOSI_s  PTE3_O
#define MISO_s  PTE4_I
#define IRQ_s   PTE5_I

//写端口
#define NRF24L01_MOSI_H_s    MOSI_s=1
#define NRF24L01_MOSI_L_s    MOSI_s=0   
#define NRF24L01_SCK_H_s     SCK_s=1   
#define NRF24L01_SCK_L_s     SCK_s=0 
#define NRF24L01_CE_H_s      CE_s=1
#define NRF24L01_CE_L_s      CE_s=0 
#define NRF24L01_CSN_H_s     CSN_s=1
#define NRF24L01_CSN_L_s     CSN_s=0 
//读端口
#define NRF24L01_MISO_s      PTE4_I
#define NRF24L01_IRQ_s       PTE5_I
//-----------------------------------常数定义-----------------------------------------------
#define TX_ADR_WIDTH_s    5   	// 5 uints TX address width
#define RX_ADR_WIDTH_s    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH_s  8 	// 20 uints TX payload
#define RX_PLOAD_WIDTH_s  8	// 20 uints TX payload

int16_t  TX_ADDRESS_s[TX_ADR_WIDTH_s]= {0x19,0x97,0x02,0x06,0x55};	//本地地址
int16_t  RX_ADDRESS_s[RX_ADR_WIDTH_s]= {0x19,0x97,0x02,0x06,0x55};	//接收地址

int16_t  RX_TAB_s[8]={0};
//-------------------------------------相关标志定义-----------------------------------------
#define MAX_TX_s  	0x10  //达到最大发送次数中断
#define TX_OK_s   	0x20  //TX发送完成中断
#define RX_OK_s   	0x40  //接收到数据中断
//-----------------------------------NRF24L01 寄存器指令------------------------------------
#define READ_REG_s        0x00  //读寄存器指令// Define read command to register  
#define WRITE_REG_s       0x20  //写寄存器指令// Define write command to register  
#define RD_RX_PLOAD_s     0x61  //读取接收数据指令// Define RX payload register address 
#define WR_TX_PLOAD_s     0xA0  //写待发数据指令// Define TX payload register address 
#define FLUSH_TX_s        0xE1  //刷新发送 FIFO 指令// Define flush TX register command  
#define FLUSH_RX_s        0xE2  //刷新接收 FIFO 指令// Define flush RX register command 
#define REUSE_TX_PL_s     0xE3  //定义重复装载数据指令// Define reuse TX payload register command 
#define NOP_s             0xFF  //保留// Define No Operation, might be used to read status register  
//----------------------------------SPI(nRF24L01) 寄存器地址--------------------------------------
#define CONFI_s          0x00  //配置收发状态，CRC 校验模式以及收发状态响应方式// 'Config' register address 
#define EN_AA_s           0x01  //自动应答功能设置// 'Enable Auto Acknowledgment' register address 
#define EN_RXADDR_s       0x02  //可用信道设置// 'Enabled RX addresses' register address 
#define SETUP_AW_s        0x03  //收发地址宽度设置// 'Setup address width' register address 
#define SETUP_RETR_s      0x04  //自动重发功能设置// 'Setup Auto. Retrans' register address 
#define RF_CH_s           0x05  //工作频率设置// 'RF channel' register address 
#define RF_SETUP_s        0x06  //发射速率、功耗功能设置// 'RF setup' register address 
#define STATUS_s          0x07  //状态寄存器// 'Status' register address 
#define OBSERVE_TX_s      0x08  //发送监测功能// 'Observe TX' register address 
#define CD_s              0x09  //地址检测 // 'Carrier Detect' register address 
#define RX_ADDR_P0_s      0x0A  //频道0 接收数据地址// 'RX address pipe0' register address 
#define RX_ADDR_P1_s      0x0B  //频道1 接收数据地址// 'RX address pipe1' register address 
#define RX_ADDR_P2_s      0x0C  //频道2 接收数据地址 // 'RX address pipe2' register address 
#define RX_ADDR_P3_s      0x0D  //频道3 接收数据地址// 'RX address pipe3' register address 
#define RX_ADDR_P4_s      0x0E  //频道4 接收数据地址// 'RX address pipe4' register address 
#define RX_ADDR_P5_s      0x0F  //频道5 接收数据地址// 'RX address pipe5' register address 
#define TX_ADDR_s         0x10  //发送地址寄存器// 'TX address' register address 
#define RX_PW_P0_s        0x11  //接收频道0 接收数据长度// 'RX payload width, pipe0' register address 
#define RX_PW_P1_s        0x12  //接收频道1 接收数据长度// 'RX payload width, pipe1' register address 
#define RX_PW_P2_s        0x13  //接收频道2 接收数据长度// 'RX payload width, pipe2' register address 
#define RX_PW_P3_s        0x14  //接收频道3 接收数据长度// 'RX payload width, pipe3' register address 
#define RX_PW_P4_s        0x15  //接收频道4 接收数据长度// 'RX payload width, pipe4' register address 
#define RX_PW_P5_s        0x16  //接收频道5 接收数据长度// 'RX payload width, pipe5' register address 
#define FIFO_STATUS_s     0x17  //FIFO栈入栈出状态寄存器设置// 'FIFO Status Register' register address 

/*********************************************************
函数名称：init_io
功    能：初始化I/O口
参    数：无
返回值  ：无
*********************************************************/
void init_io_s()
{

           
    NRF24L01_CE_L_s;  // 待机        
    NRF24L01_CSN_H_s; // SPI禁止
    NRF24L01_SCK_L_s; // SPI时钟置低
    
//    添加?
//    NRF24L01_IRQ = 1;
    //    
}
/*********************************************************
函数名称：SPI_RW
功    能：完成 GPIO模拟 SPI 的功能
参    数：byte--写入SPI总线上的一个字节数据
返回值  ：读取SPI总线上的一个字节数据
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
函数名称：SPI_RW_Reg
功    能：用来设置 24L01 的寄存器的值
参    数：reg--要写入数据的寄存器地址
          value--要写入的数据
返回值  ：该地址的原有值
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
函数名称：SPI_Read
功    能：读取 24L01 的寄存器的值
参    数：reg--要读取数据的寄存器地址
返回值  ：该地址的值
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
函数名称：SPI_Read_Buf
功    能：主要用来在接收时读取 FIFO 缓冲区中的值
参    数：reg--要读取数据的寄存器地址
          *pBuf--数据绶存数组
          bytes--读取数据的长度
返回值  ：该地址的原有值
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
函数名称：SPI_Write_Buf
功    能：主要用来把数组里的数放到发射 FIFO缓冲区中
参    数：reg--要写放数据的寄存器地址
          *pBuf--数据缓存数组
          bytes--写入数据的长度
返回值  ：该地址的原有值
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
函数名称：RX_Mode
功    能：
参    数：
返回值  ：
*********************************************************/
void RX_Mode_s(void) 
{ 
    NRF24L01_SCK_L_s;
         NRF24L01_CE_L_s; 
    	SPI_Write_Buf_s(WRITE_REG_s + RX_ADDR_P0_s, TX_ADDRESS_s, TX_ADR_WIDTH_s); // Use the same address on the RX device as the TX device

 	SPI_RW_Reg_s(WRITE_REG_s + EN_AA_s, 0x01);      // Enable Auto.Ack:Pipe0
 	SPI_RW_Reg_s(WRITE_REG_s + EN_RXADDR_s, 0x01);  // Enable Pipe0
  	SPI_RW_Reg_s(WRITE_REG_s + RF_CH_s, 0);        // Select RF channel 40  改为0 有改动
  	SPI_RW_Reg_s(WRITE_REG_s + RX_PW_P0_s, TX_PLOAD_WIDTH_s); // Select same RX payload width as TX Payload width
  	SPI_RW_Reg_s(WRITE_REG_s + RF_SETUP_s, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  	SPI_RW_Reg_s(WRITE_REG_s + CONFI_s, 0x0f);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..
        
  	                        
    NRF24L01_CE_H_s; //CE为高,10us后启动发送
 
}		

/*********************************************************
函数名称：NRF24L01_Check
功    能：检测24L01是否存在
参    数：无
返回值  ：0，成功;1，失败
*********************************************************/
int16_t buf_s[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
int16_t NRF24L01_Check_s(void)
{
	
	int16_t i; 	 
	SPI_Write_Buf_s(WRITE_REG_s+TX_ADDR_s,buf_s,5);//写入5个字节的地址.	
	SPI_Read_Buf_s(TX_ADDR_s,buf_s,5); 
        //读出写入的地址  
	for(i=0;i<5;i++)if(buf_s[i]!=0XA5) break;	 							   
	if(i!=5)return 0;//检测24L01错误	
	return 1;        //检测到24L01
}	
/*********************************************************
函数名称：NRF24L01_Check
功    能：启动NRF24L01发送一次数据
参    数：txbuf:待发送数据首地址
返回值  ：0，接收完成；其他，错误代码
*********************************************************/
int16_t NRF24L01_RxPacket_s(int16_t *rxbuf)
{
	int16_t sta,tt=0; 
       
        //if(!( NRF24L01_IRQ))
        {
	sta=SPI_Read_s(STATUS_s);  //读取状态寄存器的值    	 
	if(sta&RX_OK_s)//接收到数据
	{
              //此处有改动  RX_TAB
	      SPI_Read_Buf_s(RD_RX_PLOAD_s,RX_TAB_s,RX_PLOAD_WIDTH_s);//读取数据
	      SPI_RW_Reg_s(FLUSH_RX_s,0xff);//清除RX FIFO寄存器 
	      SPI_RW_Reg_s(WRITE_REG_s+STATUS_s,0x7f); //清除TX_DS或MAX_RT中断标志
              tt=1;
              
	}
        }
        return (tt);
}	

