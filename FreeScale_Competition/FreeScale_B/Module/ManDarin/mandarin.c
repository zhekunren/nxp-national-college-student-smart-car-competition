#include "include.h"

PID_Mandarin Mandarin;

uint32_t distance = 0;
uint32_t count1=0,count2=0;
uint32_t distance_set=0;
int16_t speed_add=0;

void mandarin_init(void)
{
    mandarin_pit2_init();
    mandarin_gpio_init();
    Mandarin_PID_SetMax(&Mandarin, 100);
    Mandarin_PID_Init(&Mandarin, 0.5, 0.0, 0.0);//7.5, 1.1, 9.5
}

void mandarin_pit2_init(void)
{
  uint32_t ldval = 0xFFFFFFFF; //
    
  SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;//
  
  PIT->MCR = 0x00;//
  
  PIT->CHANNEL[2].LDVAL = ldval-1;//
  
  PIT->CHANNEL[2].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void mandarin_gpio_init(void)
{
	gpio_pin_config_t gpio_mandarin = {kGPIO_DigitalInput, 0};
    GPIO_PinInit(GPIOE, 10U, &gpio_mandarin);
    PORT_SetPinInterruptConfig(PORTE, 10U, kPORT_InterruptRisingEdge);  
    EnableIRQ(PORTC_IRQn);
}

uint32_t dis_check[8];
float k=0;
uint32_t distance_f=5000,distance_s=5000;

void PORTE_IRQHandler(void)
{
    //DisableInterrupts;
     PORTC->ISFR = 0xFFFFFFFF;
    if(PTE10_I == 1)
    {
        DisableIRQ(PORTC_IRQn);
        count1 = PIT->CHANNEL[2].CVAL;
        PORT_SetPinInterruptConfig(PORTE, 10U, kPORT_InterruptFallingEdge);
        EnableIRQ(PORTC_IRQn);
    }
    else if(PTE10_I == 0)
    {
        
         DisableIRQ(PORTC_IRQn);
        count2 = PIT->CHANNEL[2].CVAL;
        PORT_SetPinInterruptConfig(PORTE, 10U, kPORT_InterruptRisingEdge);  
        EnableIRQ(PORTC_IRQn);
        
        distance_f = ((count1-count2)/(50000000/1000000))*350/(1000);
//        dis_check[k]=distance_f;
//        distance_s += dis_check[k];
//        k++;
//        if(k>7)
//        {
//            k=0;
//            distance = distance_s/8;
//            distance_s=0;
//        }
        //2017.03.22 15.34 距离计算修改为低通计算，减少8次平均带来的延时性。
        k=0.87f;
        distance = (int32_t)(k * distance_s + (1-k) * distance_f);
        distance_s = distance;
    }
    //EnableInterrupts;
}

void Mandarin_PID_SetMax(PID_Mandarin* Newstruct, int32_t max)
{
    Newstruct->Max = max;
}

void Mandarin_PID_Init(PID_Mandarin* Newstruct, float kp, float ki, float kd)
{
    Newstruct->P = kp;
    Newstruct->I = ki;
    Newstruct->D = kd;
}

int16_t Mandarin_PID_Calculate(PID_Mandarin* Newstruct, int32_t sdist, int32_t acdist)
{
    Newstruct->SetDist = sdist;
    Newstruct->ActualDist = acdist;
    //限制，防止过追
    if(Newstruct->ActualDist==0)
    {
        Newstruct->ActualDist=Newstruct->SetDist;
    }
    if(Newstruct->ActualDist>2500)
    {
        Newstruct->ActualDist=1500;
    }
    
    Newstruct->Err = Newstruct->SetDist - Newstruct->ActualDist;
    //新加偏差限幅 限制在1cm~10cm之间 限制有待检测
    if(Absolute(Newstruct->Err)<5)
    {
        Newstruct->Err=0;
    }
    
    if(Absolute(Newstruct->Err)>100)
    {
        if(Newstruct->Err<0)
        {
            Newstruct->Err = -100;
        }
        else if(Newstruct->Err>0)
        {
            Newstruct->Err = 100;
        }
    }
    
    Newstruct->SE += Newstruct->Err;
    
    if(Newstruct->SE>100)
    {
        Newstruct->SE=100;
    }
    else if(Newstruct->SE<(-100))
    {
        Newstruct->SE=-100;
    }
    
    Newstruct->DE = Newstruct->Err - Newstruct->LE;
     
    Newstruct->OUTPUT = (int16_t)(Newstruct->P * Newstruct->Err + Newstruct->I * Newstruct->SE + Newstruct->D * Newstruct->DE);
    
    if(Absolute(Newstruct->OUTPUT)>=Newstruct->Max)//
    {
      if(Newstruct->OUTPUT>0)
        Newstruct->OUTPUT=Newstruct->Max;
      else
        Newstruct->OUTPUT=-Newstruct->Max;
    }
    
    Newstruct->LE=Newstruct->Err;//
    
    return -(Newstruct->OUTPUT); //偏差为正输出为-，偏差为负输出为+
}
