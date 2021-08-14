#ifndef _INIT_H_
#define _INIT_H_

#define Key0 PTA26_I
#define Key1 PTA27_I
#define Key2 PTA28_I
#define Key3 PTA29_I

#define Dial0 PTB8_I
#define Dial1 PTB9_I
#define Dial2 PTB16_I
#define Dial3 PTB17_I
#define Dial4 PTB23_I 
#define Dial5 PTB22_I 
#define Dial6 PTB21_I 
#define Dial7 PTB20_I 


void All_Init(void);
void Uart0_Send(uint8_t *pTxBuff,uint16_t len);
void Uart_Init(void);
void Dial_Init(void);
void Key_Init(void);
void Ftm_pwm_init(void);
void Encoder_Init(void);
void Oled_Init(void);
void pit_init(void);
void Beep_Init(void);
void ADC_init(void);
uint16_t Adc_Get_Result(ADC_Type *adcx, uint8_t chn);


void Nrfl2401_shou_Init(void);
void Nrfl2401_fa_Init(void);
#endif
