#include "include.h"

PID_Mandarin Mandarin;

uint32_t distance=5000;
uint32_t count1=0,count2=0;
uint32_t distance_set=0;
int16_t speed_add=0;

void mandarin_init(void)
{
    mandarin_pit2_init();
    mandarin_gpio_init();
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
	GPIO_PinInit(GPIOE, 9U, &gpio_mandarin);
    GPIO_PinInit(GPIOE, 10U, &gpio_mandarin);
    PORT_SetPinInterruptConfig(PORTE, 10U, kPORT_InterruptRisingEdge);  
    EnableIRQ(PORTE_IRQn);
	
}

uint32_t dis_check[8];
float k=0;
uint32_t distance_f = 5000,distance_s=5000;

void PORTE_IRQHandler(void)
{
     PORTE->ISFR = 0xFFFFFFFF;
    if(PTE10_I == 1)
    {
        DisableIRQ(PORTE_IRQn);
        count1 = PIT->CHANNEL[2].CVAL;
        PORT_SetPinInterruptConfig(PORTE, 10U, kPORT_InterruptFallingEdge);
        EnableIRQ(PORTE_IRQn);
    }
    else if(PTE10_I == 0)
    {
        
        DisableIRQ(PORTE_IRQn);
        count2 = PIT->CHANNEL[2].CVAL;
        PORT_SetPinInterruptConfig(PORTE, 10U, kPORT_InterruptRisingEdge);  
        EnableIRQ(PORTE_IRQn);
        
        distance_s = ((count1-count2)/(50000000/1000000))*350/(1000);
//        k=0.1f;
//        distance = (int32_t)(k * distance_s + (1-k) * distance_f);
//		distance_s = distance;
		
//		if(Start_Distance > 48000 && distance_s < 460 && PTE9_I && End_Meeting_Flag == 0 &&
//			MeetingPlace_flag == 0 && Meeting_Car_Flag == 0 && (!(TX_TAB_f[0] & 0x08)))//�������ж��ǻᳵ  �����⣬ͻȻ��ͨʱ
//		{
//			Meeting_Car_Flag = 1;
//		}
    }
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
    //���ƣ���ֹ��׷
    if(Newstruct->ActualDist==0)
    {
        Newstruct->ActualDist=Newstruct->SetDist;
    }
    if(Newstruct->ActualDist>2500)
    {
        Newstruct->ActualDist=1500;
    }
    
    Newstruct->Err = Newstruct->SetDist - Newstruct->ActualDist;
    //�¼�ƫ���޷� ������1cm~10cm֮�� �����д����
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
    
    return -(Newstruct->OUTPUT); //ƫ��Ϊ�����Ϊ-��ƫ��Ϊ�����Ϊ+
}
