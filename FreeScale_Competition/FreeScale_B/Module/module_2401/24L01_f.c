#include "include.h"
 

//----------------------------------硬件地址配置-----------------------
#define CE_f    PTD10_O
#define CSN_f   PTD11_O
#define SCK_f   PTD12_O
#define MOSI_f  PTD13_O
#define MISO_f  PTD14_I
#define IRQ_f   PTD15_I

//写端口
#define NRF24L01_MOSI_H_f    MOSI_f=1
#define NRF24L01_MOSI_L_f    MOSI_f=0   
#define NRF24L01_SCK_H_f    SCK_f=1   
#define NRF24L01_SCK_L_f    SCK_f=0 
#define NRF24L01_CE_H_f      CE_f=1
#define NRF24L01_CE_L_f      CE_f=0 
#define NRF24L01_CSN_H_f     CSN_f=1
#define NRF24L01_CSN_L_f     CSN_f=0 
//读端口
#define NRF24L01_MISO_f     PTD14_I
#define NRF24L01_IRQ_f      PTD15_I
//-----------------------------------常数定义-----------------------------------------------
#define TX_ADR_WIDTH_f    5   	// 5 uints TX address width
#define RX_ADR_WIDTH_f    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH_f  8 	// 20 uints TX payload
#define RX_PLOAD_WIDTH_f  8	// 20 uints TX payload

int16_t  TX_ADDRESS_f[TX_ADR_WIDTH_f]= {0x20,0x18,0x03,0x18,0x55};	//本地地址
//int16_t  RX_ADDRESS_s[RX_ADR_WIDTH]= {0x19,0x97,0x02,0x06,0x55};	//接收地址
int16_t  TX_TAB_f[8]={0};

//-------------------------------------相关标志定义-----------------------------------------
#define MAX_TX_f  	0x10  //达到最大发送次数中断
#define TX_OK_f   	0x20  //TX发送完成中断
#define RX_OK_f   	0x40  //接收到数据中断
//-----------------------------------NRF24L01 寄存器指令------------------------------------
#define READ_REG_f        0x00  //读寄存器指令// Define read command to register  
#define WRITE_REG_f       0x20  //写寄存器指令// Define write command to register  
#define RD_RX_PLOAD_f     0x61  //读取接收数据指令// Define RX payload register address 
#define WR_TX_PLOAD_f     0xA0  //写待发数据指令// Define TX payload register address 
#define FLUSH_TX_f        0xE1  //刷新发送 FIFO 指令// Define flush TX register command  
#define FLUSH_RX_f        0xE2  //刷新接收 FIFO 指令// Define flush RX register command 
#define REUSE_TX_PL_f     0xE3  //定义重复装载数据指令// Define reuse TX payload register command 
#define NOP_f             0xFF  //保留// Define No Operation, might be used to read status register  
//----------------------------------SPI(nRF24L01) 寄存器地址--------------------------------------
#define CONFI_f          0x00  //配置收发状态，CRC 校验模式以及收发状态响应方式// 'Config' register address 
#define EN_AA_f           0x01  //自动应答功能设置// 'Enable Auto Acknowledgment' register address 
#define EN_RXADDR_f       0x02  //可用信道设置// 'Enabled RX addresses' register address 
#define SETUP_AW_f        0x03  //收发地址宽度设置// 'Setup address width' register address 
#define SETUP_RETR_f      0x04  //自动重发功能设置// 'Setup Auto. Retrans' register address 
#define RF_CH_f           0x05  //工作频率设置// 'RF channel' register address 
#define RF_SETUP_f        0x06  //发射速率、功耗功能设置// 'RF setup' register address 
#define STATUS_f          0x07  //状态寄存器// 'Status' register address 
#define OBSERVE_TX_f      0x08  //发送监测功能// 'Observe TX' register address 
#define CD_f              0x09  //地址检测 // 'Carrier Detect' register address 
#define RX_ADDR_P0_f      0x0A  //频道0 接收数据地址// 'RX address pipe0' register address 
#define RX_ADDR_P1_f      0x0B  //频道1 接收数据地址// 'RX address pipe1' register address 
#define RX_ADDR_P2_f      0x0C  //频道2 接收数据地址 // 'RX address pipe2' register address 
#define RX_ADDR_P3_f      0x0D  //频道3 接收数据地址// 'RX address pipe3' register address 
#define RX_ADDR_P4_f      0x0E  //频道4 接收数据地址// 'RX address pipe4' register address 
#define RX_ADDR_P5_f      0x0F  //频道5 接收数据地址// 'RX address pipe5' register address 
#define TX_ADDR_f         0x10  //发送地址寄存器// 'TX address' register address 
#define RX_PW_P0_f        0x11  //接收频道0 接收数据长度// 'RX payload width, pipe0' register address 
#define RX_PW_P1_f        0x12  //接收频道1 接收数据长度// 'RX payload width, pipe1' register address 
#define RX_PW_P2_f        0x13  //接收频道2 接收数据长度// 'RX payload width, pipe2' register address 
#define RX_PW_P3_f        0x14  //接收频道3 接收数据长度// 'RX payload width, pipe3' register address 
#define RX_PW_P4_f        0x15  //接收频道4 接收数据长度// 'RX payload width, pipe4' register address 
#define RX_PW_P5_f        0x16  //接收频道5 接收数据长度// 'RX payload width, pipe5' register address 
#define FIFO_STATUS_f     0x17  //FIFO栈入栈出状态寄存器设置// 'FIFO Status Register' register address 

/*********************************************************
函数名称：init_io
功    能：初始化I/O口
参    数：无
返回值  ：无
*********************************************************/
void init_io_f(void)
{

           
    NRF24L01_CE_L_f;  // 待机        
    NRF24L01_CSN_H_f; // SPI禁止
    NRF24L01_SCK_L_f; // SPI时钟置低
    
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
函数名称：SPI_RW_Reg
功    能：用来设置 24L01 的寄存器的值
参    数：reg--要写入数据的寄存器地址
          value--要写入的数据
返回值  ：该地址的原有值
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
函数名称：SPI_Read
功    能：读取 24L01 的寄存器的值
参    数：reg--要读取数据的寄存器地址
返回值  ：该地址的值
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
函数名称：SPI_Read_Buf
功    能：主要用来在接收时读取 FIFO 缓冲区中的值
参    数：reg--要读取数据的寄存器地址
          *pBuf--数据绶存数组
          bytes--读取数据的长度
返回值  ：该地址的原有值
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
函数名称：SPI_Write_Buf
功    能：主要用来把数组里的数放到发射 FIFO缓冲区中
参    数：reg--要写放数据的寄存器地址
          *pBuf--数据缓存数组
          bytes--写入数据的长度
返回值  ：该地址的原有值
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
函数名称：TX_Mode
功    能：该函数初始化NRF24L01到TX模式
          设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
          PWR_UP,CRC使能
          当CE变高后大于10us,则可以启动发送数据了
参    数：
返回值  ：
*********************************************************/
void TX_Mode_f(void) 
{ 
      	 NRF24L01_SCK_L_f;
         NRF24L01_CE_L_f; 
  	SPI_Write_Buf(WRITE_REG_f + TX_ADDR_f, TX_ADDRESS_f, TX_ADR_WIDTH_f);    // Writes TX_Address to nRF24L01
  	SPI_Write_Buf(WRITE_REG_f + RX_ADDR_P0_f, TX_ADDRESS_f, TX_ADR_WIDTH_f); // RX_Addr0 same as TX_Adr for Auto.Ack
//添加
//  	SPI_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
 	
//end
  	SPI_RW_Reg_f(WRITE_REG_f + EN_AA_f, 0x01);      // Enable Auto.Ack:Pipe0
  	SPI_RW_Reg_f(WRITE_REG_f + EN_RXADDR_f, 0x01);  // Enable Pipe0
  	SPI_RW_Reg_f(WRITE_REG_f + SETUP_RETR_f, 0x1a); // 500us + 86us, 10 retrans...
  	//SPI_RW_Reg(WRITE_REG_f + RF_CH_f, 40);        // Select RF channel 40
  	SPI_RW_Reg_f(WRITE_REG_f + RF_CH_f, 0); //2.4ghz//改动后代码  原来是上句
        SPI_RW_Reg_f(WRITE_REG_f + RF_SETUP_f, 0x07);   // TX_PWR:0dBm, Datarate:2Mbps, LNA:HCURR
  	SPI_RW_Reg_f(WRITE_REG_f + CONFI_f, 0x0e);     // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled..	
	 NRF24L01_CE_H_f; 
        
}
/*********************************************************
函数名称：NRF24L01_Check
功    能：检测24L01是否存在
参    数：无
返回值  ：0，成功;1，失败
*********************************************************/
int16_t buf_f[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
int16_t NRF24L01_Check_f(void)
{
	
	int16_t i; 	 
	SPI_Write_Buf(WRITE_REG_f+TX_ADDR_f,buf_f,5);//写入5个字节的地址.	
	SPI_Read_Buf_f(TX_ADDR_f,buf_f,5); 
        //读出写入的地址  
	for(i=0;i<5;i++)if(buf_f[i]!=0XA5) break;	 							   
	if(i!=5)return 0;//检测24L01错误	
	return 1;        //检测到24L01
}	
/*********************************************************
函数名称：NRF24L01_TxPacket
功    能：启动NRF24L01发送一次数据
参    数：txbuf:待发送数据首地址
返回值  ：发送完成状况
*********************************************************/
int16_t NRF24L01_TxPacket_f(int16_t *txbuf)
{					
	int16_t sta;
        NRF24L01_CE_L_f;
        //有改动   TX_TAB
  	SPI_Write_Buf(WR_TX_PLOAD_f,TX_TAB_f,TX_PLOAD_WIDTH_f);//写数据到TX BUF  32个字节
 	NRF24L01_CE_H_f;//启动发送	   
	//while(NRF24L01_IRQ);//等待发送完成
	sta=SPI_Read_f(STATUS_f);  //读取状态寄存器的值
        //有改动  原来此句为屏蔽状态       
	SPI_RW_Reg_f(WRITE_REG_f+STATUS_f,sta); 
        //清除TX_DS或MAX_RT中断标志
	if(sta&MAX_TX_f)//达到最大重发次数sta&MAX_TX
	{
	    SPI_RW_Reg_f(FLUSH_TX_f,0xff);//清除TX FIFO寄存器 
	    return MAX_TX_f; 
            
	}
        
	if(sta&TX_OK_f)//发送完成
	{   
            
            return 0x01; 
          
	}
	return 0;//其他原因发送失败
}

