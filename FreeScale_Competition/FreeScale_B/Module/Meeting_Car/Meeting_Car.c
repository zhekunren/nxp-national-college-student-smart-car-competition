#include "include.h"

/*0		1		2		3		4		5		 6   	                      7
 发车  会车	  终点	  会车没  坡道    PTE9_I  Enable_Meeting_Car		End_Meeting_Flag
*/
int8_t Double_Car_Mode = 0;
int16_t Double_Car_Count = 0;
int8_t Waiting_Flag = 0;
int32_t UpRamp_Distance = 0;
int32_t DownRamp_Distance = 0;
int32_t Meeting_Distance = 0;
int8_t End_Meeting_Flag = 0;
int8_t Asist_Meeting_Flag = 0;
int8_t Meeting_Car_Choice_Flag = 0;
int8_t Meeting_Back_Flag = 0;
int32_t Cur_Dis = 0;
int8_t Cur_Enable_M_Car = 0;
int8_t Cur_Enter = 0;
int8_t Cur_Asist_Flag = 0;
int32_t Cur_Buffer_D = 0;
int8_t Cur_M_A_f = 0;
int32_t A_End_D = 0;
//int32_t R_D = 0;
int8_t End_Falg = 0;
int8_t M_Back_F = 0;
int32_t Meeting_Back_Distance = 0;
int8_t Test_F = 0;
int8_t Test_F1 = 0;
int8_t Test_F2 = 0;

void Meeting_Car_Process(void)
{
	if(Ramp_Flag != 0)
	{
		//Get_Angle();
		if(((TX_TAB_f[0] & 0x08) || Meeting_Car_Choice_Flag != 4 || Double_Car_Mode == 0) && Meeting_Car_Flag == 0)
		{
/**********************上坡减速************************************/
			if(UpRamp_Distance <= 14000)
			{
				Motor_Set_Speed = 2100;
				UpRamp_Distance += Left_Pulse;
			}
			if(UpRamp_Distance > 14000)
			{
				Motor_Set_Speed = Speed_0;
				DownRamp_Distance += Left_Pulse;
				if(DownRamp_Distance > 20000)
				{
					Ramp_Flag = 0;
					UpRamp_Distance = 0;
					DownRamp_Distance = 0;
				}
			}
		}
		else if(Meeting_Car_Choice_Flag == 4 && Double_Car_Mode && Meeting_Car_Flag == 0)
		{
			Motor_Set_Speed = 2000;
			if(UpRamp_Distance < 60000)
			{
				UpRamp_Distance += Left_Pulse;
			}
			else
			{
				Ramp_Flag = 0;
				UpRamp_Distance = 0;
			}
			if(ANGLE_Y > 6.0 && Ramp_Flag == 1 && UpRamp_Distance >= 6000)
			{
				UpRamp_Distance = 0;
				Ramp_Flag = 2;
			}
			if(Ramp_Flag == 3 && ANGLE_Y < 12.0)
			{
				Waiting_Flag = 1;
				Ramp_Flag = 0;
			}
		}	
	}
	
/*********************单双车停车控制**************************/	
	if(End_Line_Flag)
	{
		End_Distance += Left_Pulse + Right_Pulse;
		if(Double_Car_Mode)
		{
			if(End_Meeting_Flag == 0)
			{
				Pid_Position(&E_Distance, -2300, End_Distance);
				Motor_Set_Speed = E_Distance.Out;
				if(End_Distance < -1000 && Left_Pulse < 10)
				{
					if(End_Line_Flag == 0)
					{
						Pro_End_Line_Flag = 0;
						End_Distance = 0;
						Asist_End_LineErr = 0;
						Asist_End_LineCount = 0;
						Motor_Set_Speed = Speed_0;
					}
					else
					{
						M_Back_F = 1;
					}
				}
			}
		}
		else
		{
			if(End_Distance > 36000)
			{
				Motor_Set_Speed = 0;
			}
		}
	}
/******************************会车区等待会车*******************************/	
	else if(MeetingPlace_flag && Meeting_Car_Flag == 0 && Double_Car_Mode)
	{
		Cur_Dis += Left_Pulse + Right_Pulse;
		Pid_Position(&M_Distance, -800, Cur_Dis);
		Motor_Set_Speed = M_Distance.Out;
		if(Cur_Dis < 0 && Left_Pulse < 10)
		{
			M_Back_F = 1;
		}
	}
	else if(CurMeetingPlace_flag  && Cur_Enable_M_Car == 0 && Double_Car_Mode)//
	{
		Cur_Dis += Left_Pulse + Right_Pulse;
		Pid_Position(&C_Distance, -3000, Cur_Dis);
		Motor_Set_Speed = C_Distance.Out;
		if(Cur_Dis < -2000)
		{
			Cur_M_A_f  = 1;
		}
	}
/*********************单双车控制**************************/	
	if(Double_Car_Mode && Waiting_Flag)
	{
		Motor_Set_Speed = 0;
	}
	else if(Double_Car_Mode == 0 && Waiting_Flag)
	{
		Waiting_Flag = 0;
		Motor_Set_Speed = Speed_0;
	}
	else if(CurMeetingPlace_flag && Double_Car_Mode == 0)
	{
		CurMeetingPlace_flag = 0;
		Motor_Set_Speed = Speed_0;
		Cur_Enable_M_Car = 0;
	}
	else if(Double_Car_Mode == 0 && (Meeting_Car_Flag || MeetingPlace_flag) && End_Meeting_Flag == 0)
	{
		Meeting_Car_Flag = 0;
		MeetingPlace_flag = 0;
		Motor_Set_Speed = Speed_0;
	}
	
/*****************会车时距离不够倒退*********************************/	
	if(Meeting_Car_Flag && Enable_Meeting_Car == 0)
	{
		if(distance_s > 1200)//&& (RX_TAB_s[0] & 0x20)
		{
			Enable_Meeting_Car = 1;
			Meeting_Back_Flag = 0;
			Waiting_Flag = 0;
			M_Back_F = 0;
		}
		else
		{
			if(Waiting_Flag || M_Back_F)
			{
				Meeting_Back_Distance += (Left_Pulse + Right_Pulse) / 2;
				Pid_Position(&B_Distance, -2800, Meeting_Back_Distance);
				if(Left_Pulse < 4)
				{
					Meeting_Back_Flag = 1;
					B_Distance.Out = B_Distance.Out > 1500 ? 1500 : B_Distance.Out;
					B_Distance.Out = B_Distance.Out < -1500 ? -1500 : B_Distance.Out;
				}
				Motor_Set_Speed = B_Distance.Out;
			}
			else
			{
				Pid_Position(&B_Distance, 1400 * 11, distance_s * 11);
				if(Left_Pulse < 4)
				{
					Meeting_Back_Flag = 1;
					B_Distance.Out = B_Distance.Out > 1500 ? 1500 : B_Distance.Out;
					B_Distance.Out = B_Distance.Out < -1500 ? -1500 : B_Distance.Out;
				}
				Motor_Set_Speed = -B_Distance.Out;
			}
		}
	}
	if(A_End_D < 30000)
	{
		A_End_D += Left_Pulse;
	}
//	if(R_D < 10000)
//	{
//		R_D += Left_Pulse;
//	}
/****************************直道会车****************************************/		
	if(Meeting_Car_Flag && Enable_Meeting_Car)
	{
		Waiting_Flag = 0;
		M_Back_F = 0;
		Meeting_Back_Distance = 0;
		Meeting_Back_Flag = 0;
		if(End_Meeting_Flag)
		{
			Motor_Set_Speed = Speed_0;
			if(End_Distance > 29000)
			{
				Meeting_Car_Flag = 0;
				Enable_Meeting_Car = 0;
				End_Meeting_Flag = 0;
				A_End_D = 0;
				End_Falg = 1;
				Cur_Dis = 0;
				End_Line_Flag = 0;
				Pro_End_Line_Flag = 0;
				Motor_Set_Speed = 2000;
			}
		}
		else
		{
			Motor_Set_Speed = 2200;
			Meeting_Distance += Left_Pulse;
			if(Meeting_Distance > 13000)
			{
				Meeting_Car_Flag = 0;
				Enable_Meeting_Car = 0;
				Cur_Dis = 0;
				Asist_Meeting_Flag = 1;
				TX_TAB_f[0] |= 0x08;
				MeetingPlace_flag = 0;
				Motor_Set_Speed = Speed_0;
				A_End_D = 0;
			}
		}
	}
/*****************************弯道会车***************************************/
	else if(Cur_Enable_M_Car)	
	{
		if(Right_MeetingPlace_flag)
		{
			Motor_Set_Speed = 1300;
			if(Cbh34 > 0 && Cbh01 < -4.0 && Cur_Enter < 7)
			{
				Cur_Enter ++;
			}
			if(Cur_Enter > 6 && Cbh34 * Cbh01 > 0)
			{
				Cur_Enable_M_Car = 0;
				CurMeetingPlace_flag = 0;
				Cur_Dis = 0;
				TX_TAB_f[0] |= 0x08;
				Cur_Asist_Flag = 1;
				Motor_Set_Speed = 1300;
			}
		}
		else
		{
			Motor_Set_Speed = 1400;
			if(Cbh34 > 0 && Cbh01 > 6.0 && Cur_Enter < 7)
			{
				Cur_Enter ++;
			}
			if(Cur_Enter > 6 && Cbh34 * Cbh01 < 0)
			{
				Cur_Enable_M_Car = 0;
				CurMeetingPlace_flag = 0;
				Cur_Dis = 0;
				TX_TAB_f[0] |= 0x08;
				Cur_Asist_Flag = 1;
				Motor_Set_Speed = 1300;
			}
		}
	}
	if(Cur_Buffer_D < 15000 && Cur_Asist_Flag)
	{
		Cur_Buffer_D += Left_Pulse + Right_Pulse;
		if(Cur_Buffer_D > 12000)
		{
			Motor_Set_Speed = Speed_0;
		}
	}
/****************************停车在赛道上***********************************/
	if(End_Falg && End_Line_Flag == 0)
	{
		End_Distance += Left_Pulse + Right_Pulse;
		if(End_Distance > 36000)
		{
			Motor_Set_Speed = 0;
		}
	}
//	if(Cur_Enable_M_Car)
//	{
//		PTD5_O = 1;
//	}
//	else
//	{
//		PTD5_O = 0;
//	}
}

void R_Data(void)
{
	if(NRF24L01_RxPacket_s(RX_TAB_s))
	{
		Double_Car_Count = 0;
	}
	else if(Double_Car_Count <= 120)
	{
		Double_Car_Count++;
	}
	if(Double_Car_Count > 120)
	{
		Double_Car_Mode = 0;
	}
	else
	{
		Double_Car_Mode = 1;
	}
	
	distance_s = RX_TAB_s[1] * 100 + RX_TAB_s[2];
	if((RX_TAB_s[5] & 0x01) && End_Falg == 0)
	{
		Test_F2 = 1;
		Meeting_Car_Flag = 1;
		End_Meeting_Flag = 1;
		End_Line_Flag = 1;
		if(RX_TAB_s[0] & 0X40)
		{
			Enable_Meeting_Car = 1;
			Meeting_Back_Flag = 0;
			Waiting_Flag = 0;
			M_Back_F = 0;
		}
		//Enable_Meeting_Car = 1;
	}
	
	if(Asist_Meeting_Flag == 0 && End_Falg == 0)
	{
		if(RX_TAB_s[0] & 0X40)
		{
			Enable_Meeting_Car = 1;
			Meeting_Back_Flag = 0;
			Waiting_Flag = 0;
			M_Back_F = 0;
		}
	}	
	if((RX_TAB_s[4] & 0x02) && Cur_Asist_Flag == 0)
	{
		Cur_Enable_M_Car = 1;
	}
	if((RX_TAB_s[4] & 0x04) && CurMeetingPlace_flag)
	{
		Cur_Asist_Flag = 1;
		Cur_Enable_M_Car = 0;
		CurMeetingPlace_flag = 0;
		Cur_Dis = 0;
		TX_TAB_f[0] |= 0x08;
		Motor_Set_Speed = 1300;
	}
/*****************************会车区********************************************/	
	if(Meeting_Car_Flag == 0 && MeetingPlace_flag && Asist_Meeting_Flag == 0 && ((RX_TAB_s[3] & 0x01) || (RX_TAB_s[0] & 0x02)))//(distance_s < 1200 && PTE9_I) || 
	{
		Meeting_Car_Flag = 1;
	}
	else if(RX_TAB_s[0] & 0x02 && Asist_Meeting_Flag == 0 && !(RX_TAB_s[0] & 0x04) && Meeting_Car_Flag == 0)//会车标志
	{
		Meeting_Car_Flag = 1;
	}
	else if((TX_TAB_f[0] & 0x08) && Meeting_Car_Flag == 0 && End_Falg == 0 && ((End_Line_Flag && ((RX_TAB_s[0] & 0x04) || (RX_TAB_s[0] & 0x02))) || (Meeting_Car_Flag && (RX_TAB_s[0] & 0x04))))
	{
		Test_F = 1;
		End_Line_Flag = 1;
		Meeting_Car_Flag = 1;
		End_Meeting_Flag = 1;
		//Enable_Meeting_Car = 1;
	}
	else if((RX_TAB_s[0] & 0x10) && Ramp_Flag != 0 && Meeting_Car_Flag == 0 && Asist_Meeting_Flag == 0)//坡道
	{
		Meeting_Car_Flag = 1;
		Enable_Meeting_Car = 1;
	}
/*****************************弯道会车区********************************************/	
	if(Cur_Enable_M_Car == 0 && RX_TAB_s[4] & 0x01 && CurMeetingPlace_flag && Cur_M_A_f && RX_TAB_s[4] & 0x08)
	{
		Cur_Enable_M_Car = 1;
	}
}

/*0		1		2		3		4		5		 6   	                      7
 发车  会车	  终点	  会车没  坡道    PTE9_I  Enable_Meeting_Car		End_Meeting_Flag
*/
void T_Data(void)
{
	if(Meeting_Car_Flag == 1)
	{
		TX_TAB_f[0] |= 0x02;
	}
	else
	{
		TX_TAB_f[0] &= ~0x02;
	}
	if(End_Line_Flag == 1)
	{
		TX_TAB_f[0] |= 0x04;
	}
	else
	{
		TX_TAB_f[0] &= ~0x04;
	}
	if(Ramp_Flag != 0)
	{
		TX_TAB_f[0] |= 0x10;
	}
	else
	{
		TX_TAB_f[0] &= ~0x10;
	}
	if(Enable_Meeting_Car)
	{
		TX_TAB_f[0] |= 0x40;
	}
	else
	{
		TX_TAB_f[0] &= ~0x40;
	}
	if(End_Meeting_Flag == 1)
	{
		TX_TAB_f[5] |= 0x01;
	}
	else
	{
		TX_TAB_f[5] &= ~0x01;
	}
	
	if(MeetingPlace_flag)
	{
		TX_TAB_f[3] |= 0x01;
	}
	else
	{
		TX_TAB_f[3] &= ~0x01;
	}
/**************************弯道会车**********************************/
	if(CurMeetingPlace_flag == 1)
	{
		TX_TAB_f[4] |= 0x01;
	}
	else
	{
		TX_TAB_f[4] &= ~0x01;
	}
	if(Cur_Enable_M_Car == 1)
	{
		TX_TAB_f[4] |= 0x02;
	}
	else
	{
		TX_TAB_f[4] &= ~0x02;
	}
	if(Cur_Asist_Flag)
	{
		TX_TAB_f[4] |= 0x04;
	}
	else
	{
		TX_TAB_f[4] &= ~0x04;
	}
	if(Cur_M_A_f)
	{
		TX_TAB_f[4] |= 0x08;
	}
	else
	{
		TX_TAB_f[4] &= ~0x08;
	}
	NRF24L01_TxPacket_f(TX_TAB_f);
}

void Meeting_Speed_Choice(void)
{
	if(PTB23_I == 0 && PTB22_I == 0)
	{
		Speed_0 = 2800;
		Steering_P = 8.40;
		Steering_D = 28.0;
		Coefficient_S = 0.178;
	}
	else if(PTB23_I == 1 && PTB22_I == 0)
	{
		Speed_0 = 2600;
		Steering_P = 8.39;
		Steering_D = 24.5;
		Coefficient_S = 0.16;
	}
	else if(PTB23_I == 0 && PTB22_I == 1)
	{
		Speed_0 = 2400;
		Steering_P = 8.38;
		Steering_D = 19.0  ;
		Coefficient_S = 0.125;
	}
	else if(PTB23_I == 1 && PTB22_I == 1)
	{
//		Speed_0 = 3000;
//		Steering_P = 8.41;
//		Steering_D = 32.5;
//		Coefficient_S = 0.209;
		
		Speed_0 = 2200;
		Steering_P = 8.37;
		Steering_D = 15.0;
		Coefficient_S = 0.095;
	}
	
	if(PTB21_I == 0 && PTB20_I == 0)
	{
		Meeting_Car_Choice_Flag = 1;
	}
	else if(PTB21_I == 1 && PTB20_I == 0)
	{
		Meeting_Car_Choice_Flag = 2;
	}
	else if(PTB21_I == 0 && PTB20_I == 1)
	{
		Meeting_Car_Choice_Flag = 3;
	}
	else if(PTB21_I == 1 && PTB20_I == 1)
	{
		Meeting_Car_Choice_Flag = 4;//坡道
	}
}

