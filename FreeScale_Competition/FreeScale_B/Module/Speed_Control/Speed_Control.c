#include "include.h"
/**************---2---23200---2000---*********/
Pid_Inc_T Left;
Pid_Inc_T Right;
Pid_Pos_T E_Distance;
Pid_Pos_T M_Distance;
Pid_Pos_T C_Distance;
Pid_Pos_T B_Distance;

int Speed_0 = 2000;

int16_t Left_Pulse = 0, Right_Pulse = 0;
int16_t Left_Actual_Speed, Right_Actual_Speed;
uint16_t protect_motor_count = 0;
uint8_t protect_flag = 0;
int Motor_Proportion = 8;
int32_t Start_Distance = 0;
int32_t R_Distance = 0;
int8_t Start_Flag = 0;
int16_t Stra_Sp_F = 0;
int8_t Count_L = 0;


void PIT0_IRQHandler()
{
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
	
	Left_Pulse = FTM_GetQuadDecoderCounterValue(FTM1);
	Right_Pulse = -FTM_GetQuadDecoderCounterValue(FTM2);
	FTM_ClearQuadDecoderCounterValue(FTM1);
	FTM_ClearQuadDecoderCounterValue(FTM2);
	
	GetADValue();
	GuiYi();
	Deal_DianGan();	
	//Get_Angle();
	
//	if(Straight_Line_Flag && Roundabout_Asist_Flag == 0 && Meeting_Car_Flag == 0 && MeetingPlace_flag == 0 && Ramp_Flag == 0 && Start_Flag != 0)
//	{
//		Stra_Sp_F = 1;
//		Motor_Set_Speed = Speed_0 + 700;
//	}
//	else if(((Straight_Line_Flag == 0 && Roundabout_Asist_Flag == 0 && Meeting_Car_Flag == 0 && MeetingPlace_flag == 0) || Stra_Sp_F) && Start_Flag != 0)
//	{
//		Stra_Sp_F = 0;
//		Motor_Set_Speed = Speed_0;
//	}
//	if(Straight_Line_Flag && Roundabout_Asist_Flag == 0 && Meeting_Car_Flag == 0 && MeetingPlace_flag == 0)
//	{
//		DirectionKp = Steering_P - 3;
//		DirectionKd = Steering_D;
//	}
//	else
//	{
//		DirectionKp = Steering_P;
//		DirectionKd = Steering_D;
//	}
	
	if(Roundabout_Asist_Flag != 0)
	{
		R_Distance += Left_Pulse;
		if(Roundabout_Distance <= 5000 && (Roundabout_Asist_Flag == 5 || Roundabout_Asist_Flag == 6))
		{
			Roundabout_Distance += Left_Pulse + Right_Pulse;
			if(Roundabout_Distance >= 2000)
			{
				Roundabout_Asist_Flag = 6;
			}
		}
		else if(Roundabout_Asist_Flag == 6 && ((G_AD[5] > 1600 && (G_AD[3] > 2950 || G_AD[4] > 2950)) || 
			AD[3] * 0.93 > GetADMax[3] || AD[4] * 0.93 > GetADMax[4]) && Roundabout_Distance > 5000)
		{
			Change_R_Coumt++;
			if(Change_R_Coumt > 1)
			{
				Change_R_Coumt = 0;
				Roundabout_Asist_Flag = 7;
			}
		}
		else if(Roundabout_Asist_Flag == 7 && Left_Roundabout_Flag && (G_AD[0] > 2900 || (G_AD[1] > 2900 && G_AD[5] > 2500)))
		{
			Change_R_Coumt++;
			if(Change_R_Coumt > 1)
			{
				Change_R_Coumt = 0;
				Roundabout_Asist_Flag = 8;
			}
		}
		else if(Roundabout_Asist_Flag == 7 && Right_Roundabout_Flag && (G_AD[1] > 2900 || (G_AD[0] > 2900 && G_AD[5] > 2500)))
		{
			Change_R_Coumt++;
			if(Change_R_Coumt > 1)
			{
				Change_R_Coumt = 0;
				Roundabout_Asist_Flag = 8;
			}
		}
		else if(Roundabout_Asist_Flag == 8 && Left_Roundabout_Flag && G_AD[0] < 2600 && G_AD[5] < 1500)// && G_AD[1] < 2800
		{
			Roundabout_Asist_Flag = 0;
			Left_Roundabout_Flag = 0;
			Roundabout_Distance = 0;
			R_Distance = 0;
//			R_D = 0;
			Motor_Set_Speed =Speed_0;
		}
		else if(Roundabout_Asist_Flag == 8 && Right_Roundabout_Flag && G_AD[1] < 2600 && G_AD[5] < 1500)// && G_AD[0] < 2800
		{
			Roundabout_Asist_Flag = 0;
			Right_Roundabout_Flag = 0;
			Roundabout_Distance = 0;
			R_Distance = 0;
//			R_D = 0;
			Motor_Set_Speed = Speed_0;
		}
		else
		{
			Change_R_Coumt = 0;
		}
		if(Roundabout_Asist_Flag != 6 && Roundabout_Asist_Flag != 0)
		{
			if(Speed_0 > 2600)
			{
				Motor_Set_Speed = 2600;
			}
		}
		else if(Roundabout_Asist_Flag == 6 && Roundabout_Distance > 5000)
		{
			Motor_Set_Speed = Speed_0;
		}
		if(R_Distance > 28000 && Roundabout_Asist_Flag == 1)
		{
			Roundabout_Asist_Flag = 0;
			Motor_Set_Speed = Speed_0;
			Left_Roundabout_Flag = 0;
			Right_Roundabout_Flag = 0;
			R_Distance = 0;
//			R_D = 0;
			Roundabout_Distance = 0;
		}
		else if(R_Distance > 110000)
		{
			Roundabout_Asist_Flag = 0;
			R_Distance = 0;
//			R_D = 0;
			Left_Roundabout_Flag = 0;
			Right_Roundabout_Flag = 0;
			Motor_Set_Speed = Speed_0;
			Roundabout_Distance = 0;
		}
	}
//	if(Roundabout_Asist_Flag == 6)
//	{
//		PTD5_O = 1;
//	}
//	else
//	{
//		PTD5_O = 0;
//	}
//	if(Ramp_Flag)
//	{
//		PTD5_O = 1;
//	}
//	else
//	{
//		PTD5_O = 0;
//	}
	Meeting_Car_Process();
	
	//if(Roundabout_Asist_Flag < 3 || Roundabout_Asist_Flag > 5)
	{
		Servo_DinaGan_Out();
//		if(Start_Distance < 50000)
//		{
//			Start_Distance += Left_Pulse;
//			if(Start_Distance < 8000)
//			{
//				PwmServoOut = PwmServoMid + 2;
//				if(Start_Flag)
//				{
//					Motor_Set_Speed = 2100;
//				}
//			}
//			else if(Start_Distance > 10000 && Start_Flag == 1)
//			{
//				Motor_Set_Speed = Speed_0;
//				Start_Flag = 2;
//			}
//		}
		SCC_CMT_Duty(PwmServoOut);
	}
	if(max01 < 50 &&  G_AD[3] < 50 && G_AD[3] < 50 && Start_Flag)
	{
		if(Count_L < 7)
		{
			Count_L++;
		}
		else
		{
			Motor_Set_Speed = 0;
		}
	}
	else
	{
		Count_L = 0;
	}
	Speed_Calculate();
	Pid_Increment(&Left, Left.Set_Speed, Left_Actual_Speed);//闭环
	Pid_Increment(&Right, Right.Set_Speed, Right_Actual_Speed);
	Motor_Out_Pwm();
	//Send_Data();
}

void PIT2_IRQHandler()
{
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);
	//GetADValue_filter();
}

int16_t Motor_Set_Speed = 0;
int16_t Diff_Speed;
float Coefficient_S = 0;

void Speed_Calculate(void)
{
	Diff_Speed = (int)(Coefficient_S * DirectionPDout * abs(DirectionPDout) * Motor_Set_Speed / Speed_0);// 
	if(Diff_Speed > Motor_Set_Speed / 3)
	{
		Diff_Speed = Motor_Set_Speed / 3;
	}
	else if(Diff_Speed < -Motor_Set_Speed / 3)
	{
		Diff_Speed = -Motor_Set_Speed / 3;
	}
	
    Left.Set_Speed = Motor_Set_Speed - Diff_Speed;
	Right.Set_Speed = Motor_Set_Speed + Diff_Speed;
	
    Left_Actual_Speed = Left_Pulse * Motor_Proportion;
	Right_Actual_Speed = Right_Pulse * Motor_Proportion;
	
/***************************堵转保护*************************************/	
//	if(protect_flag == 0 && Absolute(Left.Set_Speed-Left_Pulse + Right.Set_Speed - Right_Pulse) > 1800 && Absolute(Left_Pulse + Right_Pulse) < 100)
//	{
//		if(protect_motor_count < 100)
//		{
//			protect_motor_count++;
//		}
//			
//		else
//		{
//			protect_motor_count = 0;
//			protect_flag=1;
//		}
//	}
//	else
//	{
//		protect_motor_count=0; 
//	}
//	if(protect_flag == 1)
//	{
//		Left.Set_Speed = 0;
//		Right.Set_Speed = 0;
//	}

//	Left.Set_Speed = 2500;
//	Right.Set_Speed = 2500;
//	Left.Out = Left.Set_Speed;//开环
//	Right.Out = Right.Set_Speed;
}
	

void Crl_Pid_Init(void)
{
	Left.P = 6.9;
	Left.I = 2.37;
	Left.D = 2.2;
	Left.Err = 0;
	Left.Err_Last = 0;
	Left.Err_Next = 0;
	Left.Err_Pwm = 1300;
	Left.Tdifferential = 0;
	Left.Out = 0;
	Left.Set_Speed = 0;
	
	Right.P = 7.0;
	Right.I = 2.5;
	Right.D = 2.3;
	Right.Err = 0;
	Right.Err_Last = 0;
	Right.Err_Next = 0;
	Right.Err_Pwm = 1300;
	Right.Tdifferential = 0;
	Right.Out = 0;
	Right.Set_Speed = 0;
	
	E_Distance.P = 1.0;
	E_Distance.I = 0.001;
	E_Distance.D = 5.5;
	E_Distance.Err_Last = 0.0;
	E_Distance.I_Err_Sprt = 1000.0;
	E_Distance.I_Out_Sum = 0;
	E_Distance.Limit_Up = 3000;
	E_Distance.Limit_Dn = -3000;
	E_Distance.Out = 0;
	
	M_Distance.P = 1.0;
	M_Distance.I = 0.001;
	M_Distance.D = 5.5;
	M_Distance.Err_Last = 0.0;
	M_Distance.I_Err_Sprt = 1000.0;
	M_Distance.I_Out_Sum = 0;
	M_Distance.Limit_Up = 3000;
	M_Distance.Limit_Dn = -3000;
	M_Distance.Out = 0;
	
	C_Distance.P = 1.0;
	C_Distance.I = 0.001;
	C_Distance.D = 5.5;
	C_Distance.Err_Last = 0.0;
	C_Distance.I_Err_Sprt = 1000.0;
	C_Distance.I_Out_Sum = 0;
	C_Distance.Limit_Up = 3000;
	C_Distance.Limit_Dn = -3000;
	C_Distance.Out = 0;
	
	B_Distance.P = 1.0;
	B_Distance.I = 0.001;
	B_Distance.D = 5.5;
	B_Distance.Err_Last = 0.0;
	B_Distance.I_Err_Sprt = 1000.0;
	B_Distance.I_Out_Sum = 0;
	B_Distance.Limit_Up = 3000;
	B_Distance.Limit_Dn = -3000;
	B_Distance.Out = 0;
}



//位置式pid
void  Pid_Position(Pid_Pos_T *pid, float set, float real)
{
    float out_p;
    float out_i;
    float out_d;
	float out;
    
    pid->Err = set - real;
    out_p = pid->P * pid->Err;
    //积分分离
    if(pid->Err > -pid->I_Err_Sprt && pid->Err < pid->I_Err_Sprt)
    {
        out_i = pid->I * pid->Err + pid->I_Out_Sum;
    }
    else
	{
	  	out_i = pid->I_Out_Sum;
	}
	
	//积分限幅
	if(out_i > pid->Limit_Up)
	{
        out_i = pid->Limit_Up;
	}
	else if(out_i < pid->Limit_Dn)
	{
        out_i = pid->Limit_Dn;
	}
	pid->I_Out_Sum = out_i;
    
    out_d = pid->D * (pid->Err - pid->Err_Last);
	
	out = out_p + out_i + out_d;
	
	if(out > 5000)
	{
		pid->Out = 5000;
	}
	else if(out < -5000)
	{
		pid->Out = -5000;
	}
	else
	{
		pid->Out = out;
	}
	pid->Err_Last = pid->Err;
}

//增量式
void Pid_Increment(Pid_Inc_T *pid, float set, float real)
{
  //  float err;
    float increment;
    
    pid->Err = set - real;
    //鲁棒控制
    if(pid->Err > pid->Err_Pwm)
    {
        pid->Out = (int16_t)(0.82f * pid->Out + 0.18f * 9000); 
    }
    else if(pid->Err < -pid->Err_Pwm)
    {
        pid->Out = (int16_t)(0.82f * pid->Out - 0.18f * 9000); 
    }
    else
    {
        increment = pid->P * (pid->Err - pid->Err_Next) + pid->I * pid->Err + pid->D * (pid->Err - 2 * pid->Err_Next + pid->Err_Last);
        pid->Out = (int16_t)(pid->Out + increment);
        pid->Err_Last = pid->Err_Next;
        pid->Err_Next = pid->Err;
    }
	
	
}

/*********************************************************
描述
按照S型增长计算速度输出值
S曲线增长率 慢-快-慢
（纯数学运算！！！以3次曲线为基准）
参数
x		：自变量
maxK	：S曲线中，S曲线中间的斜率（最大增长率）
maxPos	：S曲线中，S曲线的中间位置（增长率最大的位置）
scale	：偏离中线程度，1为基准值，scale越大，偏离越多
n ：     切换舵机和速度拟合曲线 1 速度 -1 舵机
注释： 人为将正区域反转，应用于速度 和舵机中
**********************************************************/
float data_3Power_SCurve(float x, float maxK, float maxPos, float scale, float n)
{
  float rslt;
  float tem1, tem2;
  int mFlag = 1;
  
  if(x<0)
  {
    mFlag = -1;
    x = -x;
  }
  if(x > 2 * maxPos)		//使输出最大值恒定
  {
    x = 2 * maxPos;
  }
  tem2 = maxK / maxPos * x * x;	//数学计算
  tem1 = -tem2 / 3 / maxPos * x;
  
  rslt = tem1 + tem2;
  
  if(scale != 1 )		//按照要求的偏离程度纠正结果
  {
    rslt = scale * (rslt - (maxK * 2 / 3) * x) + (2 * maxK / 3) * x;
  }
  
  if(mFlag == -1)
    return (1.0*n*rslt* mFlag);
  else
    return (-1.0*n*rslt* mFlag);
}
void Motor_Out_Pwm(void)
{	
	Left.Out = Left.Out > 9000 ? 9000 : Left.Out;
    Left.Out = Left.Out < -9000 ? -9000 : Left.Out;
	Right.Out = Right.Out > 9000 ? 9000 : Right.Out;
    Right.Out  = Right.Out  < -9000 ? -9000 : Right.Out ;

	if(Left.Out >= 0)
	{
		FTM_UpdatePwmDutycycle(FTM0,
								kFTM_Chnl_0,
								kFTM_EdgeAlignedPwm,
								Left.Out);
		FTM_UpdatePwmDutycycle(FTM0,
								kFTM_Chnl_1,
								kFTM_EdgeAlignedPwm,
								0);
	}
	else
	{
		FTM_UpdatePwmDutycycle(FTM0,
								kFTM_Chnl_0,
								kFTM_EdgeAlignedPwm,
								0);
		FTM_UpdatePwmDutycycle(FTM0,
								kFTM_Chnl_1,
								kFTM_EdgeAlignedPwm,
								-Left.Out);
	}
	if(Right.Out >= 0)
	{
		FTM_UpdatePwmDutycycle(FTM0,
								kFTM_Chnl_2,
								kFTM_EdgeAlignedPwm,
								Right.Out);
		FTM_UpdatePwmDutycycle(FTM0,
								kFTM_Chnl_3,
								kFTM_EdgeAlignedPwm,
								0);
	}
	else
	{
		FTM_UpdatePwmDutycycle(FTM0,
								kFTM_Chnl_2,
								kFTM_EdgeAlignedPwm,
								0);
		FTM_UpdatePwmDutycycle(FTM0,
								kFTM_Chnl_3,
								kFTM_EdgeAlignedPwm,
								-Right.Out);
	}
}


