#include "include.h"


uint8_t imgbuff[OV7725_EAGLE_SIZE];
uint8_t imgbuff1[OV7725_EAGLE_SIZE];
uint8_t image_buffer[CAMERA_H][CAMERA_W];

uint8_t Camera=0;
uint8_t *OV7725_eagle_img_buff;
volatile IMG_STATUS_e      ov7725_eagle_img_flag = IMG_FINISH;

static uint8_t OV7725_eagle_reg_init(void);
static void OV7725_eagle_port_init(void);
void OV7725_eagle_init(uint8_t *imgaddr)
{
      OV7725_eagle_img_buff = imgaddr;
      while(OV7725_eagle_reg_init() == 0);
      OV7725_eagle_port_init();
}


/*OV7725??????*/
reg_s OV7725_eagle_reg[] =
{
  //???,?????
  /*
  50?:
{COM4         , 0xC1},
{CLKRC        , 0x02},


75?:
{COM4         , 0x41},
{CLKRC        , 0x00},


112?:
{COM4         , 0x81},
{CLKRC        , 0x00},


150?:
{COM4         , 0xC1},
{CLKRC        , 0x00},
*/
  {OV7725_COM4         , 0x81},
  {OV7725_CLKRC        , 0x00},//112?
  {OV7725_COM2         , 0x03},
  {OV7725_COM3         , 0xD0},
  {OV7725_COM7         , 0x40},
  {OV7725_HSTART       , 0x3F},
  {OV7725_HSIZE        , 0x50},
  {OV7725_VSTRT        , 0x03},
  {OV7725_VSIZE        , 0x78},
  {OV7725_HREF         , 0x00},
  {OV7725_SCAL0        , 0x0A},
  {OV7725_AWB_Ctrl0    , 0xE0},
  {OV7725_DSPAuto      , 0xff},
  {OV7725_DSP_Ctrl2    , 0x0C},
  {OV7725_DSP_Ctrl3    , 0x00},
  {OV7725_DSP_Ctrl4    , 0x00},
  
#if (CAMERA_W == 80)
  {OV7725_HOutSize     , 0x14},
#elif (CAMERA_W == 160)
  {OV7725_HOutSize     , 0x28},
#elif (CAMERA_W == 240)
  {OV7725_HOutSize     , 0x3c},
#elif (CAMERA_W == 320)
  {OV7725_HOutSize     , 0x50},
#else
  
#endif
  
#if (CAMERA_H == 60 )
  {OV7725_VOutSize     , 0x1E},
#elif (CAMERA_H == 120 )
  {OV7725_VOutSize     , 0x3c},
#elif (CAMERA_H == 180 )
  {OV7725_VOutSize     , 0x5a},
#elif (CAMERA_H == 240 )
  {OV7725_VOutSize     , 0x78},
#else
  
#endif
  
  {OV7725_EXHCH        , 0x00},
  {OV7725_GAM1         , 0x0c},
  {OV7725_GAM2         , 0x16},
  {OV7725_GAM3         , 0x2a},
  {OV7725_GAM4         , 0x4e},
  {OV7725_GAM5         , 0x61},
  {OV7725_GAM6         , 0x6f},
  {OV7725_GAM7         , 0x7b},
  {OV7725_GAM8         , 0x86},
  {OV7725_GAM9         , 0x8e},
  {OV7725_GAM10        , 0x97},
  {OV7725_GAM11        , 0xa4},
  {OV7725_GAM12        , 0xaf},
  {OV7725_GAM13        , 0xc5},
  {OV7725_GAM14        , 0xd7},
  {OV7725_GAM15        , 0xe8},
  {OV7725_SLOP         , 0x20},
  {OV7725_LC_RADI      , 0x00},
  {OV7725_LC_COEF      , 0x13},
  {OV7725_LC_XC        , 0x08},
  {OV7725_LC_COEFB     , 0x14},
  {OV7725_LC_COEFR     , 0x17},
  {OV7725_LC_CTR       , 0x05},
  {OV7725_BDBase       , 0x99},
  {OV7725_BDMStep      , 0x03},
  {OV7725_SDE          , 0x04},
  {OV7725_BRIGHT       , 0x00},
  {OV7725_CNST         , 0x50},//4B
  {OV7725_SIGN         , 0x06},
  {OV7725_UVADJ0       , 0x11},
  {OV7725_UVADJ1       , 0x02},
  
};

uint8_t OV7725_eagle_cfgnum = ARR_SIZE(OV7725_eagle_reg );
uint8_t OV7725_eagle_reg_init(void)
{
  uint16_t i = 0;
  uint8_t Sensor_IDCode = 0;
  SCCB_GPIO_Init();
  
  //OV7725_Delay_ms(50);
  if( 0 == SCCB_WriteByte ( OV7725_COM7, 0x80 ) ) /*??sensor */
  {
    printf("\n??:SCCB?????");
    return 0 ;
  }
  
  OV7725_EAGLE_Delay_ms(80);
  
  if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, OV7725_VER ) )    /* ??sensor ID?*/
  {
    printf("\n??:??ID??");
    return 0;
  }
  
  printf("\nGet ID success,SENSOR ID is 0x%x", Sensor_IDCode);
  printf("\nConfig Register Number is %d ", OV7725_eagle_cfgnum);
  if(Sensor_IDCode == OV7725_ID)
  {
    for( i = 0 ; i < OV7725_eagle_cfgnum ; i++ )
    {
      if( 0 == SCCB_WriteByte(OV7725_eagle_reg[i].addr, OV7725_eagle_reg[i].val) )
      {
        printf("\n??:????0x%x??", OV7725_eagle_reg[i].addr);
        return 0;
      }
    }
  }
  else
  {
    return 0;
  }
  printf("\nOV7725 Register Config Success!");
  return 1;
}

void OV7725_EAGLE_Delay_ms(uint16_t ms)
{
  uint16_t i,j;
  for(i = 0;i < ms;i++)
    for(j = 0;j < ms;j++);
}

void OV7725_eagle_port_init()
{
    //OV7725????????:PTC7
    gpio_pin_config_t gpio_ptc7 = {kGPIO_DigitalInput, 0};
    GPIO_PinInit(GPIOC, 7U, &gpio_ptc7);
    PORT_SetPinInterruptConfig(PORTC, 7U, kPORT_InterruptRisingEdge);  
    EnableIRQ(PORTC_IRQn);
    
    //OV7725PCLK???????:PTC6-PCLK
    gpio_pin_config_t gpio_ptc6={kGPIO_DigitalInput,0};
    GPIO_PinInit(GPIOC ,6,&gpio_ptc6);
    PORT_SetPinInterruptConfig(PORTC, 6U, kPORT_DMAFallingEdge); 

    //shuju 
    gpio_pin_config_t gpio_ptc8_15={kGPIO_DigitalInput, 0};
    GPIO_PinInit(GPIOC ,8,&gpio_ptc8_15);
    GPIO_PinInit(GPIOC ,9,&gpio_ptc8_15);
    GPIO_PinInit(GPIOC ,10,&gpio_ptc8_15);
    GPIO_PinInit(GPIOC ,11,&gpio_ptc8_15);
    GPIO_PinInit(GPIOC ,12,&gpio_ptc8_15);
    GPIO_PinInit(GPIOC ,13,&gpio_ptc8_15);
    GPIO_PinInit(GPIOC ,14,&gpio_ptc8_15);
    GPIO_PinInit(GPIOC ,15,&gpio_ptc8_15);
    PORT_SetPinInterruptConfig(PORTC,8,kPORT_InterruptOrDMADisabled);
    PORT_SetPinInterruptConfig(PORTC,9,kPORT_InterruptOrDMADisabled);
    PORT_SetPinInterruptConfig(PORTC,10,kPORT_InterruptOrDMADisabled);
    PORT_SetPinInterruptConfig(PORTC,11,kPORT_InterruptOrDMADisabled);
    PORT_SetPinInterruptConfig(PORTC,12,kPORT_InterruptOrDMADisabled);
    PORT_SetPinInterruptConfig(PORTC,13,kPORT_InterruptOrDMADisabled);
    PORT_SetPinInterruptConfig(PORTC,14,kPORT_InterruptOrDMADisabled);
    PORT_SetPinInterruptConfig(PORTC,15,kPORT_InterruptOrDMADisabled);

    //DMA???? 
    SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;//开启DMA通道多路器复用时钟
    SIM ->SCGC7 |= SIM_SCGC7_DMA_MASK;//开启DMA模块时钟
    DMA0 ->ERQ &= ~(1 << 0);//关闭通道0的DMA请求
    //选择 通道x 配置外设的DMA源请求编号
    DMAMUX0 ->CHCFG[0] = ((DMAMUX0 ->CHCFG[0] & ~DMAMUX_CHCFG_SOURCE_MASK) | DMAMUX_CHCFG_SOURCE(51));
    DMAMUX0 ->CHCFG[0] &= ~(DMAMUX_CHCFG_TRIG_MASK);//禁止周期触发
    //设置源地址
    DMA0 ->TCD[0].SADDR = 0x400ff091;//DMA_SADDR_SADDR((&PTC->Pdir));
    //在执行完针对源地址的操作之后，在源地址的基础上增加/减少偏移地址
    DMA0->TCD[0].SOFF = DMA_SOFF_SOFF(0);
    //设置源地址的传输大小   8bit
    DMA0->TCD[0].ATTR = 0 | DMA_ATTR_SSIZE(0);
    //主的计数次数（major iteration count）达到后，重新调整源地址
    DMA0->TCD[0].SLAST = DMA_SLAST_SLAST(0);

    //设置目的地址 
    DMA0->TCD[0].DADDR = DMA_DADDR_DADDR(OV7725_eagle_img_buff);
    //在执行完针对目的地址的操作之后，在目的地址的基础上增加/减少偏移地址
    DMA0->TCD[0].DOFF = DMA_DOFF_DOFF(1);
    //设置目的地址的传输宽度
    DMA0->TCD[0].ATTR |= DMA_ATTR_DSIZE(0);
    //主的计数次数（major iteration count）达到后，重新调整目的地址
    DMA0->TCD[0].DLAST_SGA = DMA_DLAST_SGA_DLASTSGA(0);
    
    //===============设置主计数器长度，循环次数====================================
    //设置主循环计数器 current major loop count
    DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(OV7725_EAGLE_SIZE);
    //起始循环计数器，当主循环计数器为零的时候，将装载起始循环计数器的值
    DMA0->TCD[0].BITER_ELINKNO = DMA_CITER_ELINKNO_CITER(OV7725_EAGLE_SIZE);
    
     //次循环一次传输字节的个数
    DMA0->TCD[0].NBYTES_MLNO = DMA_NBYTES_MLNO_NBYTES(1);
    
    //清空TCD控制寄存器     
    DMA0->TCD[0].CSR = 0;
    DMA0->TCD[0].CSR |= DMA_CSR_INTMAJOR_MASK; //使能DMA 主循环计数器减到零 中断
    DMA0->TCD[0].CSR |= DMA_CSR_DREQ_MASK; //主循环计数器等于零后，自动关闭DMA 
    DMAMUX->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK;  //DMA通道使能
    //使能eDMA中断
    NVIC_EnableIRQ(DMA0_IRQn); 
                      
}
void PORTC_IRQHandler()
{
    PORTC->ISFR = 1;
    if(Camera==0)
    {
        OV7725_eagle_img_buff = imgbuff;
        if(PORT_GetPinsInterruptFlags(PORTC) & 0x80)
        {
            NVIC_DisableIRQ(PORTC_IRQn);//??PTC??
            PORTC->ISFR = 1;
            LPLD_DMA_EnableReq(0);//??DMA??
            PORTC->ISFR = 1;
            LPLD_DMA_LoadDstAddr(0, (uint32_t)OV7725_eagle_img_buff);    
        }
    }
    else if(Camera==1)
    {
        OV7725_eagle_img_buff = imgbuff1;
        if(PORT_GetPinsInterruptFlags(PORTC) & 0x80)
        {
            NVIC_DisableIRQ(PORTC_IRQn);//??PTC??
            PORTC->ISFR = 1;
            LPLD_DMA_EnableReq(0);//??DMA??
            PORTC->ISFR = 1;
            LPLD_DMA_LoadDstAddr(0, (uint32_t)OV7725_eagle_img_buff);    
        }
    }
    PORTC->ISFR = 0xFFFFFFFF;
}

uint8_t ImgFlag=0;
char Image_Finish = 0;
void DMA_Callback(void)
{
  ov7725_eagle_img_flag = IMG_FINISH;
  Image_Finish = 1;                      
  NVIC_DisableIRQ(PORTC_IRQn);//
  
  if(Camera == 0)
  {
    ImgFlag=0;
  }
  else ImgFlag = 1;
  
  Camera++;
  if(Camera >= 2)
    Camera = 0;
  
  PORTC->ISFR = ~0;                   //?1??????(???,????????????????)
  NVIC_EnableIRQ(PORTC_IRQn); 
}

void StartGetImg()
{
  PORTC->ISFR = 1<<0;
  LPLD_DMA_EnableReq(0);//??DMA??
  PORTC->ISFR = 1<<0;
  LPLD_DMA_LoadDstAddr(0, (uint32_t)OV7725_eagle_img_buff);
  PORTC->ISFR = ~0;                        //?1??????(???,????????????????)
  EnableIRQ(PORTC_IRQn);                         //??PTC???
}


