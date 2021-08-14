#include "include.h"

int Page_Number=1;
//0100010
void First_Page(void)
{
	Dis_Num3(0,0,TX_TAB_f[0],4);
	Dis_Num3(0,30,Roundabout_Asist_Flag,1);
	Dis_Num3(0,40,Left_Roundabout_Flag,1);
	Dis_Num3(0,50,Right_Roundabout_Flag,1);
	Dis_Num3(0,60,End_Falg,1);
	Dis_Num3(0,70,Pro_End_Line_Flag,1);
	Dis_Num3(0,80,Ramp_Flag,1);
	Dis_Num3(0,90,Test_F1,1);
	Dis_Num3(0,100,Test_F2,1);
	Dis_Num3(0,110,MeetingPlace_flag,1);

	Dis_Num3(1,0,AD[0],4); 
	Dis_Num3(1,45,AD[2],4); 
	Dis_Num3(1,90,AD[1],4);
	
	Dis_Num3(2,0,AD[3],4);
	Dis_Num3(2,45,AD[5],4);
	Dis_Num3(2,90,AD[4],4);

	LCD_Print(25,3,"GuiYi_value");
	Dis_Num3(4,0,G_AD[0],4); 
	Dis_Num3(4,45,G_AD[2],4);
	Dis_Num3(4,90,G_AD[1],4);
	
	Dis_Num3(5,0,G_AD[3],4);
	Dis_Num3(5,45,G_AD[5],4);	
	Dis_Num3(5,90,G_AD[4],4);

	Dis_Num4(6,0,Cbh01,7);
	Dis_Num4(6,45,Cbh34,7);
	Dis_Num4(6,90,OffsetDan,7);
	
	Dis_Num3(7,0,Straight_Line_Flag,1);
	Dis_Num3(7,15,Meeting_Car_Flag,1);
	Dis_Num3(7,30,Enable_Meeting_Car,1);
	Dis_Num3(7,45,End_Meeting_Flag,1);
	//Dis_Num3(7,60,Motor_Set_Speed,4);
	//Dis_Num4(7,60,ANGLE_Y,7);
	Dis_Num4(7,60,ExpectOffset,7);
	
}
void Second_Page(void)
{
	LCD_Print(0,0,"p2");

	Dis_Num3(1,0,abs(Gyr_Z),5);
	
	Dis_Num3(1,60,PDoutAll,5);
	
	Dis_Num3(2,0,(int)(abs(MpuNowError)),5);
}
void Third_Page(void)
{
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
}


void Oled_Disply(void)
{
	if(Page_Number == 1)
		First_Page();
	else if(Page_Number == 2)
		Second_Page();
	else if(Page_Number == 3)
		Third_Page();
}
void Key_Scan(void)
{
	if(Key0==0)
	{   
		Delay_Time(10);
		if(Key0 == 0)
		{
			LCD_Fill(0x00);  
			Page_Number++;
			if(Page_Number > 3)
			{
				Page_Number = 1;
			}
		}	
		while(Key0 == 0);
	}
	if(Key3 == 0)
	{
		Delay_Time(10);
		if(Key3 == 0)   
		{
			LCD_Fill(0x00);  
			Page_Number--;
			if(Page_Number < 1)
			{
				Page_Number = 3;
			}
		}
		while(Key3 == 0); 
	}
}


void Seng_Image(void)
{	
    int i,j;
	for(i = 0; i < CAMERA_H; i ++)
	{
		for(j = 0; j < CAMERA_W; j ++)
		{
			if(j == 80)
			{
				UART_WriteByte(UART4, 0xaa);
			}
			else
			{
				UART_WriteByte(UART4 , image_buffer[i][j]);
			}
		}
	}
	UART_WriteByte(UART4 , 0xff);
}

void Send_Data(void)
{
	
//	UserData[0] = (int16_t)(Left.Out);
//	UserData[1] = (int16_t)(Left.Err);
//	UserData[2] = (int16_t)(Left.Set_Speed);
//	UserData[3] =  (int16_t)(Left_Actual_Speed);
//	UserData[4] = (int16_t)(Right.Set_Speed);
//	UserData[5] =  (int16_t)(Right_Actual_Speed);
//	UserData[0] = (int16_t)(Cbh01 * 10);
//	UserData[1] = (int16_t)(Cbh34 * 10);
//	UserData[2] = (int16_t)(OffsetDan * 10);
//	UserData[3] = (int16_t)(ExpectOffset * 10);
//	UserData[4] = (int16_t)(Meeting_Car_Flag * 100);
//	UserData[5] = (int16_t)(Enable_Meeting_Car * 100);
//	UserData[6] = (int16_t)(G_AD[2]);
//	send_user_data1(UserData);
//	
//	OutData[0] = (int16_t)(PTE9_I * 500);
//	OutData[1] = (int16_t)(distance_s);
//	OutData[2] = (int16_t)(distance_s);
	
//	OutData[0] = (int16_t)(ExpectOffset * 10);
//	OutData[1] = (int16_t)(DirectionPDout);
//	OutData[2] = (int16_t)(Roundabout_Asist_Flag * 10);
	OutData[0] = (int16_t)(Left.Set_Speed);
	OutData[1] = (int16_t)(Left_Actual_Speed);
	OutData[2] = (int16_t)(Right.Set_Speed);
	OutData[3] = (int16_t)(Right_Actual_Speed);
	
	OutPut_Data();
}
