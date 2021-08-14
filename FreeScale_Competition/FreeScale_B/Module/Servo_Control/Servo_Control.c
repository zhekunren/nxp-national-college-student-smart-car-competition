#include "include.h"

/********************************电感计算角度************************************/
int16_t PwmServoOut = PwmServoMid;

float Steering_P = 8.1, Steering_D = 25.0;

float DirectionKp = 0.0, DirectionKd = 0.0; //pd参数
float ExpectOffset = 0, ExpectOffset_old = 0,T_ExpectOffset_old = 0;
int16_t DirectionPDout;

void Servo_DinaGan_Out(void)
{
	if(Enable_Meeting_Car == 0 && CurMeetingPlace_flag == 0)
	{
		DirectionPDout = (int16_t)(DirectionKp * ExpectOffset + DirectionKd * (ExpectOffset - T_ExpectOffset_old));
	}
	else if(CurMeetingPlace_flag && Cur_M_A_f)
	{
		ExpectOffset += 7.2; 
		DirectionPDout = (int16_t)(DirectionKp * ExpectOffset + DirectionKd * (ExpectOffset - ExpectOffset_old));
	}
	else if(Enable_Meeting_Car)
	{
		ExpectOffset -= 6.6; 
		DirectionPDout = (int16_t)(DirectionKp * ExpectOffset + DirectionKd * (ExpectOffset - ExpectOffset_old));
	}
	
	DirectionPDout = DirectionPDout > 82 ? 82 : DirectionPDout;
	DirectionPDout = DirectionPDout < -82 ? -82 : DirectionPDout;
	
	if(Meeting_Back_Flag || (Left_Pulse < 0 && Right_Pulse < 0 && Pro_End_Line_Flag && Double_Car_Mode) || (MeetingPlace_flag && Left_Pulse < 0 && Right_Pulse < 0))
	{
		DirectionPDout = -DirectionPDout;
	}
	PwmServoOut = (int)(PwmServoMid- DirectionPDout);
	T_ExpectOffset_old = ExpectOffset_old;
	ExpectOffset_old = ExpectOffset;
}
void Steering_Init(void)
{
	DirectionKp = Steering_P;
	DirectionKd = Steering_D;
}

/***串级***/
float MpuKp = 0.6, MpuKd =0.8; //pd参数
float MpuNowError, MpuLastError;
int16_t PDoutAll;
void MpuChuanJiPID(void)
{
	Gyr_Z=-Gyr_Z/400.0;
	MpuNowError=DirectionPDout-Gyr_Z;
	PDoutAll=MpuKp*MpuNowError+MpuKd*(MpuNowError-MpuLastError);
	
	PDoutAll = PDoutAll > 82 ? 82 : PDoutAll;
	PDoutAll = PDoutAll < -82 ? -82 : PDoutAll;
	
	PwmServoOut = (int)(PwmServoMid-PDoutAll);
	
	MpuLastError=MpuNowError;
}

/*******************图像计算角度*****************************/
Servo_Engine Servo;
float Last_Image_Differ = 0;
void Servo_Image_Init(void)
{
	Servo.P = 2.0;
	Servo.D = 0.8;
	Servo.OUT = PwmServoMid;
}

void Servo_Image_Out(void)
{
	Servo.OUT = Image_Differ * Servo.P + (Image_Differ - Last_Image_Differ) * Servo.D;
	
	
	Servo.OUT = Servo.OUT > 82 ? 82 : Servo.OUT;
	Servo.OUT = Servo.OUT < -82 ? -82 : Servo.OUT;
	
	Servo.OUT = (int)(PwmServoMid - Servo.OUT);

	SCC_CMT_Duty(Servo.OUT);
	Last_Image_Differ = Image_Differ;
}

