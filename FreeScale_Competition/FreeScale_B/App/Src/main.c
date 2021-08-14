#include "include.h"

/*0		1		2		3		4		5		6		7		8		9		10
 发车  会车	  终点	A车运行    B车运行  会车没
*/
void Delay_Time(uint32_t n)
{
	uint16_t i;
	while(n--) 
	{
		for(i=0; i<5000; i++)
		{
			__ASM("nop");
		}
	}
}
void CheckResetReason(void);

//绿车
int16_t shou1,shou2;
int32_t Satrt_Count = 0;
int16_t ggggg = 0;
float Time_Counter = 0;

int main(void)
{	
	int8_t i;
	BOARD_InitPins();
	BOARD_BootClockRUN();
    
     __disable_irq();
	All_Init();
	do
	{
		Flash_Heng();
		Flash_Shu();
		
		Dis_Num3(2,0,GetADMax[0],4); 
		Dis_Num3(2,45,GetADMax[2],4); 
		Dis_Num3(2,90,GetADMax[1],4); 

		Dis_Num3(3,0,GetADMin[0],4); 
		Dis_Num3(3,45,GetADMin[2],4); 
		Dis_Num3(3,90,GetADMin[1],4); 
		
		LCD_Print(10,4,"H_ai_value");
		Dis_Num3(5,0,GetADMax[3],4); 
		Dis_Num3(5,45,GetADMax[5],4); 
		Dis_Num3(5,90,GetADMax[4],4); 

		Dis_Num3(6,0,GetADMin[3],4); 
		Dis_Num3(6,45,GetADMin[5],4); 
		Dis_Num3(6,90,GetADMin[4],4);
	}while(1==Dial2);
	R_ADMax = GetADMax[0] + GetADMax[1] + GetADMax[2];
	for(i = 0;i < 6;i++)
	{
		tmpAD[i] = 3000.0 / (GetADMax[i] - GetADMin[i]);   
	}
	LCD_Fill(0x00); 
	__enable_irq();
	
//	while(PTA27_I && (!(RX_TAB_s[0] & 0x01)))
//	{
//		//PTD5_O = 1;
//		Key_Scan();
//		Oled_Disply();
//		NRF24L01_RxPacket_s(RX_TAB_s);
//	}
//	//PTD5_O = 0;
//	TX_TAB_f[0] = 0x01;
//	while(!NRF24L01_TxPacket_f(TX_TAB_f));
//	PIT_StartTimer(PIT, kPIT_Chnl_1);
//	while(Satrt_Count < 100);
//	PIT_StopTimer(PIT, kPIT_Chnl_1);
	
	Motor_Set_Speed = Speed_0;
	Start_Flag = 1;
	
	while(1)
	{
		if(Image_Finish == 1)
		{ 
			Image_Finish = 0;
			if(ImgFlag == 0)
			{
				img_extract((uint8_t *)image_buffer, imgbuff, OV7725_EAGLE_SIZE);
			}
			else if(ImgFlag == 1)
			{
				img_extract((uint8_t *)image_buffer, imgbuff1, OV7725_EAGLE_SIZE);
			}
			Search_Black_Edge();
			Extract_Middle_Line();
			//Transform_Send_Image();
//			if(Roundabout_Asist_Flag > 2 && Roundabout_Asist_Flag < 6)
//			{	
//				Calculate_Image_Differ();
//				Servo_Image_Out();
//			}
			R_Data();
			T_Data();
		}	
		Key_Scan();
		Oled_Disply();
	}
}


void CheckResetReason(void)
{
    //printf(" The most recent reset caused by: ");
    if (RCM->SRS0 & RCM_SRS0_POR_MASK)
    {
        Dis_Num(6,20,0,7);//printf("POR(Power-on reset)\r\n");//上电复位
    }
    else if (RCM->SRS0 & RCM_SRS0_LVD_MASK)
    {
      
        Dis_Num(6,20,1,7);//printf("LVD(low-voltage detect) trip\r\n");//低电压复位
    }
    if (RCM->SRS0 & RCM_SRS0_PIN_MASK)  Dis_Num(6,20,2,7);//printf("External reset pin\r\n");//外部复位引脚
    if (RCM->SRS0 & RCM_SRS0_WDOG_MASK) Dis_Num(6,20,3,7);//printf("Watchdog timeout\r\n");//看门狗
    if (RCM->SRS0 & RCM_SRS0_LOC_MASK) Dis_Num(6,20,4,7);//printf("A loss of external clock\r\n");//外部时钟丢失
    if (RCM->SRS0 & RCM_SRS0_WAKEUP_MASK) Dis_Num(6,20,5,7);//printf("LLWU(Low leakage wakeup reset) module wakeup source\r\n");//低泄漏唤醒复位
    if (RCM->SRS1 & RCM_SRS1_SACKERR_MASK) Dis_Num(6,20,6,7);//printf("Peripheral failure to acknowledge attempt to enter stop mode\r\n");//外围故障确认尝试进入停止模式
    if (RCM->SRS1 & RCM_SRS1_EZPT_MASK) Dis_Num(6,20,7,7);//printf("EzPort receiving the RESET command while the device is in EzPort mode\r\n");//
    if (RCM->SRS1 & RCM_SRS1_MDM_AP_MASK) Dis_Num(6,20,8,7);//printf("Host debugger system setting of the System Reset Request bit\r\n");
    if (RCM->SRS1 & RCM_SRS1_SW_MASK) Dis_Num(6,20,9,7);//printf("Software setting of SYSRESETREQ bit\r\n");
    if (RCM->SRS1 & RCM_SRS1_LOCKUP_MASK) Dis_Num(6,20,10,7);//printf("Core LOCKUP event\r\n");
    if (RCM->SRS1 & RCM_SRS1_JTAG_MASK) Dis_Num(6,20,11,7);//printf("JTAG\r\n");
    printf("-------------------------------------------------------------------------------\r\n\r\n");
    
}


