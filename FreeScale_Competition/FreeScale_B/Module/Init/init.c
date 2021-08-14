#include "include.h"


void All_Init(void)
{
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	NVIC_SetPriority(PORTC_IRQn,0);
	NVIC_SetPriority(PIT2_IRQn,1);
	NVIC_SetPriority(PIT0_IRQn,2);
	SCC_CMT_Init(50,PwmServoMid);
	Meeting_Speed_Choice();
	Steering_Init();
	Crl_Pid_Init();
	Servo_Image_Init();
	Dial_Init();
	Key_Init();
	Beep_Init();
	Ftm_pwm_init();
	Encoder_Init();
	Oled_Init();
	Uart_Init();
	ADC_init();
	OV7725_eagle_init(imgbuff);

	BOARD_I2C_ConfigurePins();
	while(Mpu6050_Init());
	sensor_calibrate1();
	Flash_Init();
	pit_init();
	Nrfl2401_fa_Init();
	Nrfl2401_shou_Init();
}


void Key_Init(void)
{
	gpio_pin_config_t gpio_key_config;
	gpio_key_config.pindirection = kGPIO_DigitalInput;
	GPIO_PinInit(PTA, 26, &gpio_key_config);
	GPIO_PinInit(PTA, 27, &gpio_key_config);
	GPIO_PinInit(PTA, 28, &gpio_key_config);
	GPIO_PinInit(PTA, 29, &gpio_key_config);
	
}

void Dial_Init(void)//boma
{
	gpio_pin_config_t gpio_dial_config;
	gpio_dial_config.pindirection = kGPIO_DigitalInput;
	GPIO_PinInit(PTB, 8, &gpio_dial_config);
	GPIO_PinInit(PTB, 9, &gpio_dial_config);
	GPIO_PinInit(PTB, 16, &gpio_dial_config);
	GPIO_PinInit(PTB, 17, &gpio_dial_config);
	GPIO_PinInit(PTB, 20, &gpio_dial_config);
	GPIO_PinInit(PTB, 21, &gpio_dial_config);
	GPIO_PinInit(PTB, 22, &gpio_dial_config);
	GPIO_PinInit(PTB, 23, &gpio_dial_config);
}

void Beep_Init(void)
{
	gpio_pin_config_t gpio_beep_config;
	gpio_beep_config.pindirection = kGPIO_DigitalOutput;
	gpio_beep_config.outputLogic = 0;
	GPIO_PinInit(PTD, 5, &gpio_beep_config);
}

void Encoder_Init(void)
{
	ftm_phase_params_t ftm1_phab;
    ftm1_phab.enablePhaseFilter = false;
    ftm1_phab.phasePolarity = kFTM_QuadPhaseNormal; 
    CLOCK_EnableClock(kCLOCK_Ftm1);
    FTM1->MODE = FTM_MODE_FTMEN_MASK | FTM_MODE_WPDIS_MASK;
    FTM_SetQuadDecoderModuloValue(FTM1, 0x00, 0xffff);
    FTM_ClearQuadDecoderCounterValue(FTM1);
    FTM_SetupQuadDecode(FTM1,
                         &ftm1_phab,
                         &ftm1_phab,
                         kFTM_QuadPhaseEncode);
	
    CLOCK_EnableClock(kCLOCK_Ftm2);
    FTM2->MODE = FTM_MODE_FTMEN_MASK | FTM_MODE_WPDIS_MASK;
    FTM_SetQuadDecoderModuloValue(FTM2, 0x00, 0xffff);
    FTM_ClearQuadDecoderCounterValue(FTM2);
    FTM_SetupQuadDecode(FTM2,
                         &ftm1_phab,
                         &ftm1_phab,
                         kFTM_QuadPhaseEncode);
}

void Ftm_pwm_init(void)
{
    ftm_config_t ftm_pwm_con;
    ftm_chnl_pwm_signal_param_t ftm_chnl_pwm[4] = {kFTM_Chnl_0,kFTM_HighTrue,0,0,
                                                   kFTM_Chnl_1,kFTM_HighTrue,0,0,
                                                   kFTM_Chnl_2,kFTM_HighTrue,0,0,
                                                   kFTM_Chnl_3,kFTM_HighTrue,0,0};
    FTM_GetDefaultConfig(&ftm_pwm_con);
    FTM_Init(FTM0,&ftm_pwm_con);
    
    FTM_SetupPwm(FTM0,
                 ftm_chnl_pwm,
                 4U,
                 kFTM_EdgeAlignedPwm,
                 13333,
                 CLOCK_GetFreq(kCLOCK_BusClk));
                                                   
    FTM_StartTimer(FTM0, kFTM_SystemClock);
												   
}

void Oled_Init(void)
{
    gpio_pin_config_t gpio_oled_config;
	gpio_oled_config.pindirection = kGPIO_DigitalOutput;
	gpio_oled_config.outputLogic = 1;
	
    GPIO_PinInit(GPIOA,4,&gpio_oled_config);
    GPIO_PinInit(GPIOA,5,&gpio_oled_config);
    GPIO_PinInit(GPIOA,12,&gpio_oled_config);
    GPIO_PinInit(GPIOA,15,&gpio_oled_config);
    LCD_Init();
    LCD_Fill(0x00);
}

void pit_init(void)
{
    //定时器0
    pit_config_t pit_con;
    PIT_GetDefaultConfig(&pit_con);
    
    PIT_Init(PIT,&pit_con);//初始化
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(10000U, CLOCK_GetFreq(kCLOCK_BusClk)));//10ms
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0,kPIT_TimerInterruptEnable);
    EnableIRQ(PIT0_IRQn);
    PIT_StartTimer(PIT, kPIT_Chnl_0);
	
	//定时器1
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(20000U, CLOCK_GetFreq(kCLOCK_BusClk)));//20ms
    PIT_EnableInterrupts(PIT, kPIT_Chnl_1,kPIT_TimerInterruptEnable);
    EnableIRQ(PIT1_IRQn);
	//定时器2
//	PIT_SetTimerPeriod(PIT, kPIT_Chnl_2, USEC_TO_COUNT(1000U, CLOCK_GetFreq(kCLOCK_BusClk)));//1ms
//    PIT_EnableInterrupts(PIT, kPIT_Chnl_2,kPIT_TimerInterruptEnable);
//    EnableIRQ(PIT2_IRQn);
//	PIT_StartTimer(PIT, kPIT_Chnl_2);
}


void ADC_init(void)
{
    adc16_config_t adc_config;
    adc16_channel_config_t adc_cahnnel_config;
	adc_cahnnel_config.channelNumber = 10;
	adc_cahnnel_config.enableInterruptOnConversionCompleted = false;
    ADC16_GetDefaultConfig(&adc_config);//kADC16_ClockDivider8,kADC16_ResolutionSE12Bit
	adc_config.clockDivider = kADC16_ClockDivider1;
	adc_config.clockSource = kADC16_ClockSourceAlt1;

    ADC16_Init(ADC1,&adc_config);
    ADC16_SetChannelConfig(ADC1,0,&adc_cahnnel_config);//SC1A  
}
 

uint16_t Adc_Get_Result(ADC_Type *adcx, uint8_t chn)
{
  adcx->SC1[0] &= ~(ADC_SC1_AIEN_MASK);
  adcx->SC1[0] &= ~(ADC_SC1_ADCH_MASK);
  adcx->SC1[0] |= ADC_SC1_ADCH(chn);
  while((adcx->SC1[0] & ADC_SC1_COCO_MASK) == 0); //等待转换完成  
  return adcx->R[0];
}



void Uart_Init(void)
{
    uart_config_t uart_config;
    uart_config.baudRate_Bps = 256000;
    uart_config .enableRx = true;
    uart_config .enableTx =true;
    uart_config .parityMode = kUART_ParityDisabled;
    UART_Init(UART4,&uart_config,CLOCK_GetFreq(kCLOCK_BusClk));
    
//    uart_con.baudRate_Bps = 38400;
//    uart_con .enableRx = true;
//    uart_con .enableTx =true;
//    uart_con .parityMode = kUART_ParityDisabled;
//    UART_Init(UART1, &uart_con, CLOCK_GetFreq(kCLOCK_CoreSysClk));
}


void Uart0_Send(uint8_t *pTxBuff,uint16_t len)
{
	while(len > 0)
    {
        len--;
        UART_WriteByte(UART4,*pTxBuff++);
    }
}

void Nrfl2401_fa_Init(void)/////////
{
	gpio_pin_config_t gpio_nrf_config1;
	gpio_nrf_config1.pindirection = kGPIO_DigitalOutput;
	gpio_nrf_config1.outputLogic = 0;
	GPIO_PinInit(PTD, 13, &gpio_nrf_config1);
	GPIO_PinInit(PTD, 12, &gpio_nrf_config1);
	GPIO_PinInit(PTD, 11, &gpio_nrf_config1);
	GPIO_PinInit(PTD, 10, &gpio_nrf_config1);

	gpio_pin_config_t gpio_nrf_config2;
	gpio_nrf_config2.pindirection = kGPIO_DigitalInput;
	GPIO_PinInit(PTD, 15, &gpio_nrf_config2);
	GPIO_PinInit(PTD, 14, &gpio_nrf_config2);

	
  init_io_f();
  while(!NRF24L01_Check_f())//检测24L01是否存在
  {
    LCD_Print(0,1,"fff:");
  }
  TX_Mode_f();
}

void Nrfl2401_shou_Init(void)
{
	gpio_pin_config_t gpio_nrf_config1;
	gpio_nrf_config1.pindirection = kGPIO_DigitalOutput;
	gpio_nrf_config1.outputLogic = 0;
	GPIO_PinInit(PTE, 3, &gpio_nrf_config1);
	GPIO_PinInit(PTE, 2, &gpio_nrf_config1);
	GPIO_PinInit(PTE, 1, &gpio_nrf_config1);
	GPIO_PinInit(PTE, 0, &gpio_nrf_config1);

	gpio_pin_config_t gpio_nrf_config2;
	gpio_nrf_config2.pindirection = kGPIO_DigitalInput;
	GPIO_PinInit(PTE, 5, &gpio_nrf_config2);
	GPIO_PinInit(PTE, 4, &gpio_nrf_config2);

  init_io_s();
  while(!NRF24L01_Check_s())//检测24L01是否存在
  {
    LCD_Print(0,1,"sss:");
  }
  RX_Mode_s();
}


void PIT1_IRQHandler()
{
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);
	Satrt_Count++;
}
