#include "include.h"

/****电感原始值采集*****/
int32_t AD[6];//电感原始值
int16_t L_AD[6] = {0};
int16_t R_ADMax;


void GetADValue(void)
{
	int i, j, k, adTmp;
	int32_t adValue[6][5] 	= {0};

	for(i = 0; i < 5; i++)//采初值
	{   
		adValue[0][i] = Adc_Get_Result(ADC1,10);//横左
		adValue[1][i] = Adc_Get_Result(ADC1,15);//横右
		adValue[2][i] = Adc_Get_Result(ADC1,13);//中
		adValue[3][i] = Adc_Get_Result(ADC1,11);//竖左
		adValue[4][i] = Adc_Get_Result(ADC1,14);//竖右
		adValue[5][i] = Adc_Get_Result(ADC1,12);//竖右
	}  
	for(i = 0;i < 6; i++)//冒泡
	{ 
		for(k = 0;k < 5; k++) 
		{
			for(j = 0;j < 5-k; j++)
			{
				if(adValue[i][j] > adValue[i][j+1])
				{ 
					adTmp = adValue[i][j+1];
					adValue[i][j+1] = adValue[i][j];
					adValue[i][j] = adTmp;
				}
			}
		}
	}   
	for(i = 0;i < 6; i++)//平均滤波
	{
		AD[i] =  adValue[i][2];//(int)((adValue[i][1] + adValue[i][2] + adValue[i][3]) / 3);
	} 	
}
void GetADValue_filter(void)
{
	AD[0] = Adc_Get_Result(ADC1,10);//横左
	AD[1] = Adc_Get_Result(ADC1,15);//横右
	AD[2] = Adc_Get_Result(ADC1,13);//中
	AD[3] = Adc_Get_Result(ADC1,11);//竖左
	AD[4] = Adc_Get_Result(ADC1,14);//竖右	
	AD[5] = Adc_Get_Result(ADC1,12);
	
	
	AD[0] = 0.13 * AD[0] + 0.87 * L_AD[0];
	AD[1] = 0.13 * AD[1] + 0.87 * L_AD[1];
	AD[2] = 0.13 * AD[2] + 0.87 * L_AD[2];
	AD[3] = 0.13 * AD[3] + 0.87 * L_AD[3];
	AD[4] = 0.13 * AD[4] + 0.87 * L_AD[4];
	AD[5] = 0.13 * AD[5] + 0.87 * L_AD[5];
	
	L_AD[0] = AD[0]; 
	L_AD[1] = AD[1]; 
	L_AD[2] = AD[2]; 
	L_AD[3] = AD[3]; 
	L_AD[4] = AD[4];
	L_AD[5] = AD[5];
}

/***电感最值采集*****/
int32_t GetADMax[6]={1,1,1,1,1,1},GetADMin[6]={4000,4000,4000,0,0,0};
void GetHengMaxMin(void)
{
	GetADValue(); 
	if(AD[0] > GetADMax[0])
	{
		GetADMax[0] = AD[0];
		GetADMin[1] = AD[1];
	}
	if(AD[1] > GetADMax[1])
	{
		GetADMax[1] = AD[1];
		GetADMin[0] = AD[0];
	}
	if(AD[2] > GetADMax[2])
	{
		GetADMax[2] = AD[2];
	}
	GetADMin[2] = (int32_t)((GetADMin[0] + GetADMin[1]) / 2);
	GetADMax[3]  = (GetADMax[0] + GetADMax[1]) / 2;
	GetADMax[4] = GetADMax[3];
	GetADMin[3] = (int32_t)(GetADMin[2] / 3);
	GetADMin[4] = GetADMin[3];
}
void GetShuMaxMin(void)
{
	GetADValue();
//	GetADMax[3] = AD[3] > GetADMax[3] ? AD[3] : GetADMax[3];
//	GetADMax[4] = AD[4] > GetADMax[4] ? AD[4] : GetADMax[4];
	GetADMax[5] = AD[5] > GetADMax[5] ? AD[5] : GetADMax[5];
	//竖电感最小值默认给0
	GetADMin[5] = 0;
}

/***归一化算法3000***/
int G_AD[6];//归一化后值
float tmpAD[6];
void GuiYi(void)
{
	int i;
	int32_t adLast[6];//归一化中间值
	
	for(i = 0;i < 6;i++)
	{
		if(AD[i] < GetADMin[i])        adLast[i] = 0;
		else if(AD[i] > GetADMax[i])   adLast[i] = GetADMax[i] - GetADMin[i];
		else   adLast[i] = AD[i] - GetADMin[i];
	} 
	for(i = 0;i < 6;i++)
	{
		G_AD[i] = (int)(tmpAD[i] * adLast[i]);
	}
}
/******电感处理*************
	3		   4
	0     2    1   
**************************/
float WeightCbh01 = 0.345,WeightCbh34 = 0.345,WeightOffsetDan = 0.31;

float Cbh01, CbhOld01,Cbh34, CbhOld34;
int DirGan = 0, DirGanOld;
float OffsetDan,L_OffsetDan = 0.0;
int8_t Cross_Flag = 0;
int8_t Cross_Flag_E = 0;
int Dir_E = 0, Dir_EOld;
int min01,max01,min34,L_min01;
int16_t Sum_T = 0, Sum_T_L = 0;
int16_t R_Dir_Cal;
int8_t Left_Roundabout_Flag = 0;
int8_t Right_Roundabout_Flag = 0;
int8_t Roundabout_Asist_Flag = 0;
int32_t Roundabout_Distance = 0;
int16_t Roundabout_Count = 0;
int16_t Change_R_Coumt = 0;
int16_t R_IN_Count = 0;
void Deal_DianGan(void)
{
	int add01,delta01;
	int add34,delta34;
	
	min34 = G_AD[3] > G_AD[4] ? G_AD[4] : G_AD[3];
	if(G_AD[0] > G_AD[1])
	{
		min01 = G_AD[1];
		max01 = G_AD[0];
	}
	else
	{
		min01 = G_AD[0];
		max01 = G_AD[1];
	}
	
	//左右电感Cbh01
	delta01 = G_AD[0] - G_AD[1];                        
	add01 = G_AD[0] + G_AD[1];
	if(add01 == 0) add01 = 1; 		
	Cbh01 = (float)(10.00 * delta01) / add01;   
//	Cbh01 = min01 < 20 ? CbhOld01 : Cbh01;
//方向DirGan
	if(delta01 > 0)
	{
		DirGan = 1;
	}
	else  if(delta01 < 0)
	{
		DirGan = -1;
	}
	if(min01 < 20 && (!(DirGan != DirGanOld && abs(delta01) > 200)))
	{
		Cbh01 = CbhOld01;
		DirGan = DirGanOld;
	}
	DirGanOld = DirGan;
	if(min01 < 20 && abs(Cbh01) < 9.3)
	{
		Cbh01 = 9.3 * DirGan;
	}
	CbhOld01 = Cbh01;

	
	delta34 = G_AD[3] - G_AD[4];                        
	add34 = G_AD[3] + G_AD[4];
	if(add34 == 0) add34 = 1; 		
	Cbh34 = (float)(10.00 * delta34) / add34;   
	//Cbh34 = min34 < 10 ? CbhOld34 : Cbh34;
	if(delta34 > 0)
	{
		Dir_E = 1;
	}
	else  if(delta34 < 0)
	{
		Dir_E = -1;
	}
	if(min34 < 10 && (!(Dir_E != Dir_EOld && abs(delta34) > 300)))
	{
		Cbh34 = CbhOld34;
		Dir_E = Dir_EOld;
	}
	Dir_EOld = Dir_E;
	if(min34 < 10 && abs(Cbh34) < 9.5)
	{
		Cbh34 = 9.5 * Dir_E;
	}
	CbhOld34 = Cbh34;

	//单电感Offset_Dan
	OffsetDan =(float)(10.0 * (3000 - G_AD[2])) / 3000;
	
	if(abs(Cbh34) < 1.2 && OffsetDan > abs(Cbh34) && Enable_Meeting_Car == 0)
	{
		OffsetDan = abs(Cbh34);
	}
	if(Enable_Meeting_Car)
	{
		OffsetDan = OffsetDan * abs(Cbh01) / 10.0;	//归化到0到10
	}
	else
	{
		OffsetDan = OffsetDan * abs(Cbh34) / 10.0;	//归化到0到10
	}
/***************************************弯道会车时外径补偿***************************************/
	if(AD[2] < GetADMin[2] && Cbh34 * Cbh01 > 0 && CurMeetingPlace_flag)// 
	{
		OffsetDan += 22.0 * (GetADMin[2] - AD[2]) * (GetADMin[2] - AD[2]) / GetADMin[2] / GetADMin[2];
	}
	else if(Enable_Meeting_Car && AD[2] < GetADMin[2])
	{
		OffsetDan += 7.0 * (GetADMin[2] - AD[2]) * (GetADMin[2] - AD[2]) / GetADMin[2] / GetADMin[2];
	}
	L_OffsetDan = OffsetDan;
/*************************************十字************************************************/	
	if(Cross_Flag == 1 && G_AD[5] > 1300)
	{
		Cross_Flag = 2;
	}
	if(Cross_Flag == 2 && (G_AD[5] < 300 || abs(Cbh34) < 2.0))
	{
		Cross_Flag = 0;
	}
	if(Ramp_Flag == 0 && End_Line_Flag == 0 && Roundabout_Asist_Flag == 0 && abs(Cbh01 - Cbh34) > 6.0 &&
		abs(Cbh01) < abs(Cbh34) && G_AD[5] > 1200 && Cross_Flag == 0 && (Straight_Line_Flag || Cross_Flag_E))
	{
		Cross_Flag_E = 1;
	}
	else
	{
		Cross_Flag_E = 0;
	}
	
	//偏差汇总
/****************************直道会车******************************************/
	if(Enable_Meeting_Car && CurMeetingPlace_flag == 0)
	{
		ExpectOffset = 0.5 * Cbh01 + 0.5 * OffsetDan * DirGan;
	}
/****************************弯道会车******************************************/
	else if(CurMeetingPlace_flag && Cur_M_A_f)
	{
		ExpectOffset = 0.55 * Cbh01 + 0.05 * Cbh34+ 0.4 * OffsetDan * Dir_E;
	}
	else if(Cross_Flag != 0 || Cross_Flag_E)
	{
		ExpectOffset = WeightCbh01 * Cbh01;
	}
	else
	{
		ExpectOffset = WeightCbh01 * Cbh01 + WeightCbh34 * Cbh34+ WeightOffsetDan * OffsetDan * Dir_E;
	}
//	if(Roundabout_Asist_Flag == 7 && abs(ExpectOffset) < abs(ExpectOffset_old))
//	{
//		ExpectOffset = ExpectOffset_old;
//	}
	Sum_T = AD[0] + AD[1] + AD[2] - R_ADMax;
	if(End_Line_Flag == 0 && Sum_T > 900 && Roundabout_Asist_Flag == 0 && Sum_T_L <= Sum_T)// R_D > 8000 max01 > 2900 && G_AD[2] > 2900 && Ramp_Flag == 0 && 
	{
		Roundabout_Count++;
		if(Roundabout_Count > 2)
		{
			Roundabout_Asist_Flag = 1;
			R_IN_Count = 1;
			R_Dir_Cal = (AD[0] - GetADMin[0]) * tmpAD[0] + (AD[4] - GetADMin[4]) * tmpAD[4] - (AD[1] - GetADMin[1]) * tmpAD[1] - (AD[3] - GetADMin[3]) * tmpAD[3];
			if(R_Dir_Cal > 0 && PTB17_I == 0)
			{
				Left_Roundabout_Flag = 1;
			}
			else if(PTB17_I == 0)
			{
				 Right_Roundabout_Flag = 1;
			}
		}
	}
	else
	{
		Roundabout_Count = 0;
	}
	Sum_T_L = Sum_T;
//	if(End_Line_Flag == 0 && max01 > 2980 && G_AD[2] > 2980 && min01 > 2500 && Roundabout_Asist_Flag == 0  && (L_min01 < min01 || min01 > 2900))//Ramp_Flag == 0 && 
//	{
//		Roundabout_Count++;
//		if(Roundabout_Count > 2)
//		{
//			Roundabout_Asist_Flag = 1;
//		}
//	}
//	else
//	{
//		Roundabout_Count = 0;
//	}
//	L_min01 = min01;
	if(Roundabout_Asist_Flag && Roundabout_Asist_Flag != 6)// 
	{
		GuiYi_double();
		Round();
	}
}

int G_AD_double[6];//1éò??ˉoó?μ
void GuiYi_double(void)
{
	int i;
	int32_t adLast[6];//1éò??ˉ?D???μ
	for(i = 0;i < 6;i++)
	{
		if(AD[i] < GetADMin[i])        adLast[i] = 0;
		else if(AD[i] > GetADMax[i] * 2)   adLast[i] = 2 * GetADMax[i] - GetADMin[i];
		else   adLast[i] = AD[i] - GetADMin[i];
	} 
	for(i = 0;i < 6;i++)
	{
		G_AD_double[i] = (int)(tmpAD[i] * adLast[i]);
	}
}

void Round(void)
{
	int l_sum,r_sum;
	if(Roundabout_Asist_Flag < 6 && Roundabout_Asist_Flag >= 1)
	{
		if(Left_Roundabout_Flag)
		{
			l_sum = G_AD_double[0] / 5 + G_AD_double[3];
			r_sum = G_AD_double[1] / 6  + G_AD_double[4] / 4.0;	
		}
		else
		{
			l_sum = G_AD_double[0] /6 + G_AD_double[3] / 4.0;
			r_sum = G_AD_double[1] / 5  + G_AD_double[4];	
		}
	}
	else if(Roundabout_Asist_Flag > 6)
	{
		if(Left_Roundabout_Flag)
		{
			l_sum = G_AD_double[3] * 0.77;
			r_sum = G_AD_double[1];	
		}
		else
		{
			l_sum = G_AD_double[0] ;
			r_sum = G_AD_double[4] * 0.77;	
		}
	}
	if(Roundabout_Asist_Flag == 1)
	{
		
		ExpectOffset = 10.0 * (l_sum - r_sum) / (l_sum + r_sum);
		if(R_IN_Count > 0)
		{
			T_ExpectOffset_old = ExpectOffset;
			R_IN_Count--;
		}
	}
	else if(Left_Roundabout_Flag)
	{
		ExpectOffset = 9.0 * (l_sum - r_sum) / (l_sum + r_sum) + 3.0 * G_AD_double[5] / 3000;
	}
	else
	{
		ExpectOffset = 9.0 * (l_sum - r_sum) / (l_sum + r_sum) - 3.0 * G_AD_double[5] / 3000;
	}
}

