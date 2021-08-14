#include "include.h"

IMAGE Jump_Left[Row_Max];
IMAGE Jump_Right[Row_Max];
IMAGE Jump_Middle[Row_Max];
IMAGE Jump_Roundabout[Column_Max];


First_Point Left_Find_Point;
First_Point Right_Find_Point;

int16_t Left_End_Row;
int16_t Right_End_Row;
int16_t Middle_End_Row;
int8_t Ramp_Flag = 0;
int8_t Ramp_Count = 0;


int16_t Left_Inflection_Row1;
int16_t Left_Inflection_Row2;
int8_t Left_Inflection_Flag;
int16_t Right_Inflection_Row1;
int16_t Right_Inflection_Row2;
int8_t Right_Inflection_Flag;
int8_t Left_Straight_Line_Flag;
int8_t Right_Straight_Line_Flag;
int8_t Straight_Line_Flag = 0;
int8_t End_Line_Flag = 0;
int8_t Pro_End_Line_Flag = 0;
int8_t Asist_End_Line = 0;
int8_t Asist_End_LineCount = 0;
int8_t Asist_End_LineErr = 0;
int32_t End_Distance = 0;
int8_t End_Line_Slow_Flag = 0;
int8_t Meeting_Car_Flag = 0, Asist_Meeting_Car_Flag = 0;
int8_t Enable_Meeting_Car = 0;
float Inflection_Slope1;
float Inflection_Slope2;
int16_t Inflection_Intercept;


/***********************左边界断点找点极限*******************************
************(30,100);(119;140)******************************************
************k = (140 - 100) /(119 - 30) = 0.4494************************
************b = 140 - k * 119 = 86.5************************************
************************************************************************/
/***********************右边边界断点找点极限*****************************
************(30,60);(119;20)******************************************
************k = (20 - 60) /(119 - 30) =-0.4494************************
************b = 20 - k * 119 = 73.5************************************
***********************************************************************/

float Left_Limit_Slope = 0.4494;
float Left_Limit_Intercept = 86.5;
float Right_Limit_Slope = -0.4494;
float Right_Limit_Intercept = 73.5;

int8_t left_add_line_flag = 0;
int8_t right_add_line_flag = 0;
void Image_Data_Init(void)
{
	int16_t row;
	Left_End_Row = 119;
	Right_End_Row = 119;
	Middle_End_Row = 119;
	Left_Find_Point.flag = 0;
	Left_Find_Point.row = 119;
	Right_Find_Point.flag = 0;
	Right_Find_Point.row = 119;
	Left_Inflection_Flag = 0;
	Right_Inflection_Flag = 0;
	Left_Straight_Line_Flag = 0;
	Right_Straight_Line_Flag = 0;
	left_add_line_flag = 0;
	right_add_line_flag = 0;
	
	for(row = 119; row>= 45; row--)
	{
		Jump_Left[row].column = 0;
		Jump_Left[row].flag = 0;
		Jump_Right[row].column = 159;
		Jump_Right[row].flag = 0;
		Jump_Middle[row].column = 80;
	}
}

/*****************寻找边沿***********************/
int8_t Right_Asist_row_flag;
int8_t Left_Asist_row_flag;
void Search_Black_Edge(void)
{
	int16_t row,column;
	int16_t forecast_column;
	float k,b;
	int16_t x0, x1, y0, y1;
	int8_t point_count = 0;
	int8_t lose_count = 0;
	int8_t left_lose_line_flag = 0;
	int16_t right_break_find_row;
	int16_t left_break_find_row;
	int16_t right_lose_line_row;
	int16_t left_lose_line_row;
	int8_t right_lose_line_flag = 0;
	int16_t Asist_row;
	int16_t left_limit_column;
	int16_t right_limit_column;
	uint16_t left119_jump_point_column = 100;
	uint16_t right119_jump_point_column = 60;
	int16_t Right_First_column = 159;
	int16_t Left_First_column = 0;
	
	Image_Data_Init();//初始化数据

/********************************************十字*********************************************/
	
/*******************寻找第一行左边跳变点*******************************/
	for(column = 130; column >= 80; column--)
	{
		if(image_buffer[119][column] == White && image_buffer[119][column - 1] == Black &&
			image_buffer[119][column - 3] == Black && image_buffer[115][column - 2] == Black)
		{
			left119_jump_point_column = column + 15;
			break;
		}
	}
	
/*******************寻找第一行右边边跳变点*******************************/
	for(column = 30; column <= 80; column++)
	{
		if(image_buffer[119][column] == White && image_buffer[119][column + 1] == Black &&
			image_buffer[119][column + 3] == Black && image_buffer[115][column + 2] == Black)
		{
			right119_jump_point_column = column - 15;
			break;
		}
	}
	
/*******************搜寻左边界********************************/
	edge_detection_left1(119, left119_jump_point_column, 0);
	for(row = 118 ;row >= 45; row--)
	{
		edge_detection_left(row, left119_jump_point_column, 0);
		if(Jump_Left[row].flag == 1)
		{
			point_count++;
		}
		else 
		{
			point_count = 0;
		}
		
		if(point_count >= 2 && Jump_Left[row + 1].column - Jump_Left[row].column >= 40 && Jump_Left[row].column < 20)
		{
			left119_jump_point_column = 50;
			point_count = 0;
		}
		else if(Absolute(Jump_Left[row + 1].column - Jump_Left[row].column) >= 15 && point_count >= 2)
		{
			point_count--;
		}
		if(point_count >= 7)
		{
			Left_Find_Point.flag = 1;
			Left_Find_Point.row = row + 3;
			Left_First_column = Jump_Left[Left_Find_Point.row + 3].column;
			break;
		}
	}
/*****************从第一个找到的寻找点边界************************/
	if(Left_Find_Point.flag == 1)
	{
		Left_End_Row = 45;
		for(row = Left_Find_Point.row - 4; row >= 45;row--)
		{
			forecast_column = Jump_Left[row + 1].column + (Jump_Left[row + 1].column -  Jump_Left[row + 3].column) / 2;
			edge_detection_left(row, forecast_column + Range_Column, forecast_column - Range_Column);
			if(Jump_Left[row].flag == 0)
			{
				lose_count++;
				Jump_Left[row].column = Jump_Left[row + 1].column;
				Jump_Left[row].flag = 1;
			}
			else
			{
				lose_count = 0;
			}
			if(lose_count >= 3)
			{
				Jump_Left[row].flag = 0;
				Jump_Left[row + 1].flag = 0;
				Jump_Left[row + 2].flag = 0;
				left_lose_line_row = row + 6;
				Left_End_Row = row + 3;
				left_lose_line_flag = 1;
				break;
			}
		}
/*********************************搜寻拐点,用于十字路口补线处理***************************************/
		if(Left_First_column < 12)
		{
			Search_LeftInflectionPoint(Left_Find_Point.row + 3, Left_End_Row + 3);
		}
		if(Left_Inflection_Flag)
		{
			if(Left_Find_Point.row + 3 == Left_Inflection_Row1)
			{
				Left_Inflection_Row1++;
			}
			Inflection_Slope1 = 1.0 * (Left_First_column - Jump_Left[Left_Inflection_Row1].column) / (Left_Find_Point.row + 3 - Left_Inflection_Row1); 
			Inflection_Slope2 = 1.0 * (Jump_Left[Left_End_Row + 3].column - Jump_Left[Left_Inflection_Row2].column) / (Left_End_Row + 3 - Left_Inflection_Row2); 
			Inflection_Intercept = (int16_t)(Jump_Left[Left_Inflection_Row1].column - Inflection_Slope1 * Left_Inflection_Row1);
		}
/***************前几行补线**********************/
		if(Left_Find_Point.row < 115)
		{
			x0 = 119;
			y0 = 0;
			x1 = Left_Find_Point.row;
			y1 = Jump_Left[x1].column ;
			k = (float)(1.0 * (y1 - y0) / (x1 - x0));
			b = (float)(y1 - k * x1);
			for(row = Left_Find_Point.row; row <= 119; row++)
			{
				Jump_Left[row].column = Add_Line(row, k, b);
				Jump_Left[row].flag = 1;
				if(Jump_Left[row].column < 0)
				{
					Jump_Left[row].column = 0;
				}
				if(image_buffer[row][Jump_Left[row].column + 4] == Black && image_buffer[row][Jump_Left[row].column + 1] == Black &&
					 image_buffer[row - 2][Jump_Left[row].column + 8] == Black && image_buffer[row][Jump_Left[row].column + 7] == Black)//补出来线左边为黑
				{
					if(!(y1 <= 12 && Jump_Left[row].column < 10 && image_buffer[row][Jump_Left[row].column + 30] == White  && 
						image_buffer[row][Jump_Left[row].column + 50] == White))
					{
						Left_End_Row = 119;
						Jump_Left[Left_End_Row].column = 0;
						Left_Find_Point.flag = 0;
						left_lose_line_flag = 0;
						Left_Inflection_Flag = 0;
					}
					break;
				}
			}
		}
/**********************************判断左边补线是否正确以及是否继续扫线*****************************************************/
		if(Left_Find_Point.flag)
		{
			left_limit_column = (int16_t)(Left_Limit_Slope * Left_End_Row + Left_Limit_Intercept);
			if(Left_Inflection_Flag && ((Inflection_Slope2 > 0.8 && Inflection_Slope1 < -1.2) || (Inflection_Slope1 * Inflection_Slope2 < -1.1) || Inflection_Slope1 < -1.9))
			{
				Asist_row = Left_Inflection_Row1 - 1;
				for(row = Left_Inflection_Row1 - 1; row >= Left_End_Row; row--)
				{
					Jump_Left[row].flag = 0;
				}
				Left_End_Row = Left_Inflection_Row1;
				Left_Asist_row_flag = 1;
			}
			else
			{
				Asist_row = left_lose_line_row - 7;
				Left_Asist_row_flag = 0;
			}
			if(Left_Inflection_Flag && Jump_Left[Left_Inflection_Row1].column >= 90 &&
				Inflection_Slope2 >= 2.0)
			{
				Left_End_Row = Left_Inflection_Row1;
			}
			else if((left_lose_line_flag && left_limit_column >= Jump_Left[Left_End_Row].column) || Left_Asist_row_flag)
			{
				point_count = 0;
				if(Asist_row > 51)
				{
					for(row = Asist_row; row >= 45; row--)
					{
						if(Left_Asist_row_flag)
						{
							forecast_column = Inflection_Slope1 * row + Inflection_Intercept + 40;
							edge_detection_left(row, forecast_column, Jump_Left[Left_Inflection_Row1].column + 8);
						}
						else 
						{
							edge_detection_left(row, 100, 0);
						}
						if(Jump_Left[row].flag == 1)
						{
							point_count++;
						}
						else
						{
							point_count = 0;
						}
						if(point_count >= 6)
						{
							left_add_line_flag = 1;
							left_break_find_row = row + 2;
							break;
						}
					}
				}
				if(left_add_line_flag)
				{
					lose_count = 0;
					Left_End_Row = 45;
					for(row = left_break_find_row - 3; row >= 45; row--)
					{
						forecast_column = Jump_Left[row + 1].column + (Jump_Left[row + 1].column -  Jump_Left[row + 3].column) / 2;
						edge_detection_left(row, forecast_column + Range_Column, forecast_column - Range_Column);
						if(Jump_Left[row].flag == 0)
						{
							lose_count++;
							Jump_Left[row].column = Jump_Left[row + 1].column;
							Jump_Left[row].flag = 1;
						}
						else
						{
							lose_count = 0;
						}
						if(lose_count >= 3)
						{
							Jump_Left[row].flag = 0;
							Jump_Left[row + 1].flag = 0;
							Jump_Left[row + 2].flag = 0;
							Left_End_Row = row + 3;
							break;
						}
					}
	/***************中间几行补线**********************/
					x0 = left_break_find_row;
					y0 = Jump_Left[x0].column;
					x1 = Asist_row + 4;
					y1 = Jump_Left[x1].column ;
					k = (float)(1.0 * (y1 - y0) / (x1 - x0));
					b = (float)(y1 - k * x1);
					for(row = x0; row < x1; row++)
					{
						Jump_Left[row].column = Add_Line(row, k, b);
						Jump_Left[row].flag = 1;
						if(Jump_Left[row].column < 0)
						{
							Jump_Left[row].column = 0;
						}
						if(image_buffer[row][Jump_Left[row].column + 4] == Black && image_buffer[row][Jump_Left[row].column + 7] == Black &&
							image_buffer[row - 2][Jump_Left[row].column + 7] == Black)//补出来线左边为黑
						{
							Left_End_Row = Asist_row + 3;
							break;
						}
					}				
				}
			}
		}
	}
/*********************************************************************
************************搜寻右边界************************************
*********************************************************************/	
	point_count = 0;
	Inflection_Slope1 = 0;
	Inflection_Slope2 = 0;
	edge_detection_right(119, right119_jump_point_column, 159);
	for(row = 118 ;row >= 45; row--)
	{
		edge_detection_right(row, right119_jump_point_column, 159);
		if(Jump_Right[row].flag == 1)
		{
			point_count++;
		}
		else 
		{
			point_count = 0;
		}
		if(point_count >= 2 && Jump_Right[row].column - Jump_Right[row + 1].column >= 40 && Jump_Right[row].column > 140)
		{
			right119_jump_point_column = 110;
			point_count = 0;
		}
		else if(Absolute(Jump_Right[row].column - Jump_Right[row + 1].column) >= 15 && point_count >= 2)
		{
			point_count--;
		}
		if(point_count >= 7)
		{
			Right_Find_Point.flag = 1;
			Right_Find_Point.row = row + 3;
			Right_First_column = Jump_Right[Right_Find_Point.row + 3].column;
			break;
		}
	}
/*****************从第一个找到的寻找点边界************************/
	lose_count = 0;
	if(Right_Find_Point.flag)
	{
		Right_End_Row = 45;
		for(row = Right_Find_Point.row - 4; row >= 45; row--)
		{
			forecast_column = Jump_Right[row + 1].column + (Jump_Right[row + 1].column -  Jump_Right[row + 3].column) / 2;
			edge_detection_right(row, forecast_column - Range_Column, forecast_column + Range_Column);
			if(Jump_Right[row].flag == 0)
			{
				lose_count++;
				Jump_Right[row].column = Jump_Right[row + 1].column;
				Jump_Right[row].flag = 1;
			}
			else
			{
				lose_count = 0;
			}
			if(lose_count >= 3)
			{
				Jump_Right[row].flag = 0;
				Jump_Right[row + 1].flag = 0;
				Jump_Right[row + 2].flag = 0;
				right_lose_line_row = row + 6;
				Right_End_Row = row + 3;
				right_lose_line_flag = 1;
				break;
			}
		}
/****************************寻找右边拐点*************************************************/
		if(Right_First_column > 148)
		{
			Search_RightInflectionPoint(Right_Find_Point.row + 3, Right_End_Row + 3);
		}
		if(Right_Inflection_Flag)
		{
			if(Right_Find_Point.row + 3 == Right_Inflection_Row1)
			{
				Right_Inflection_Row1++;
			}
			Inflection_Slope1 =1.0 * (Right_First_column - Jump_Right[Right_Inflection_Row1].column) / (Right_Find_Point.row + 3 - Right_Inflection_Row1); 
			Inflection_Slope2 =1.0 * (Jump_Right[Right_End_Row + 3].column - Jump_Right[Right_Inflection_Row2].column) / (Right_End_Row + 3 - Right_Inflection_Row2); 
			Inflection_Intercept = (int16_t)(Jump_Right[Right_Inflection_Row1].column - Inflection_Slope1 * Right_Inflection_Row1);
		}
/***************前几行补线**********************/
		if(Right_Find_Point.row < 115)
		{
			x0 = 119;
			y0 = 159;
			x1 = Right_Find_Point.row;
			y1 = Jump_Right[x1].column;
			k = (float)(1.0 * (y1 - y0) / (x1 - x0));
			b = (float)(y1 - k * x1);
			for(row = Right_Find_Point.row; row <= 119; row++)
			{
				Jump_Right[row].column = Add_Line(row, k, b);
				Jump_Right[row].flag = 1;
				if(Jump_Right[row].column > 159)
				{
					Jump_Right[row].column = 159;
				}
				if(image_buffer[row][Jump_Right[row].column - 4] == Black && image_buffer[row][Jump_Right[row].column - 7] == Black && 
					image_buffer[row- 2][Jump_Right[row].column - 7] == Black)//补出来线右边为黑
				{
					if(!(Jump_Right[row].column > 150 && y1>= 148 && image_buffer[row][Jump_Right[row].column - 50] == White &&
						image_buffer[row][Jump_Right[row].column - 30] == White))
					{
						Right_End_Row = 119;
						Jump_Right[Right_End_Row].column = 159;
						right_lose_line_flag = 0;
						Right_Find_Point.flag = 0;
						Right_Inflection_Flag = 0;
						break;
					}
					
				}
			}
		}
/*************************判断右边补线是否正确以及是否继续扫线***************************************/
		if(Right_Find_Point.flag)
		{
			right_limit_column = (int16_t)(Right_Limit_Slope * Right_End_Row + Right_Limit_Intercept);
			if(Right_Inflection_Flag && ((Inflection_Slope2 < -0.8 && Inflection_Slope1 > 1.2) || (Inflection_Slope1 * Inflection_Slope2 < -1.1) || Inflection_Slope1 > 1.9))//十字扫线处理
			{
				for(row = Right_Inflection_Row1 - 1; row >= Right_End_Row; row--)
				{
					Jump_Right[row].flag = 0;
				}
				Right_End_Row = Right_Inflection_Row1;
				Asist_row = Right_Inflection_Row1 - 1;
				Right_Asist_row_flag = 1;
			}
			else
			{
				Asist_row = right_lose_line_row - 7;
				Right_Asist_row_flag = 0;
			}
			if(Right_Inflection_Flag == 1 && Jump_Right[Right_Inflection_Row1].column <= 70 && 
				Inflection_Slope2 <= -2.0)
			{
				Right_End_Row = Right_Inflection_Row1;
			}
			else if((right_lose_line_flag == 1 && right_limit_column <= Jump_Right[Right_End_Row].column) || Right_Asist_row_flag)
			{
				point_count = 0;
				
				if(Asist_row > 51)
				{
					for(row = Asist_row; row >= 45; row--)
					{
						
						if(Right_Asist_row_flag)
						{
							forecast_column = Inflection_Slope1 * row + Inflection_Intercept - 40;
							edge_detection_right(row, forecast_column, Jump_Right[Right_Inflection_Row1].column - 8);
						}
						else
						{
							edge_detection_right(row, 50, 159);
						}
						if(Jump_Right[row].flag == 1)
						{
							point_count++;
						}
						else
						{
							point_count = 0;
						}
						if(point_count >= 6)
						{
							right_add_line_flag = 1;
							right_break_find_row = row + 2;
							break;
						}
					}
				}
				if(right_add_line_flag == 1)
				{
					lose_count = 0;
					Right_End_Row = 45;
					for(row = right_break_find_row - 3; row >= 45; row--)
					{
						forecast_column = Jump_Right[row + 1].column + (Jump_Right[row + 1].column -  Jump_Right[row + 3].column)/2;
						edge_detection_right(row, forecast_column - Range_Column, forecast_column + Range_Column);
						if(Jump_Right[row].flag == 0)
						{
							lose_count++;
							Jump_Right[row].column = Jump_Right[row + 1].column;
							Jump_Right[row].flag = 1;
						}
						else
						{
							lose_count = 0;
						}
						if(lose_count >= 3)
						{
							Jump_Right[row].flag = 0;
							Jump_Right[row + 1].flag = 0;
							Jump_Right[row + 2].flag = 0;
							Right_End_Row = row + 3;
							break;
						}
					}
	/***************中间几行补线**********************/
					x0 = right_break_find_row;
					y0 = Jump_Right[x0].column;
					x1 = Asist_row + 4;
					y1 = Jump_Right[x1].column ;
					k = (float)(1.0 * (y1 - y0) / (x1 - x0));
					b = (float)(y1 - k * x1);
					for(row = x0; row < x1; row++)
					{
						Jump_Right[row].column = Add_Line(row, k, b);
						Jump_Right[row].flag = 1;
						if(Jump_Right[row].column < 0)
						{
							Jump_Right[row].column = 0;
						}
						if(image_buffer[row][Jump_Right[row].column - 4] == Black && image_buffer[row][Jump_Right[row].column - 7] == Black &&
							image_buffer[row- 2][Jump_Right[row].column - 7] == Black)//补出来线右边为黑
						{
							Right_End_Row = Asist_row + 3;
							break;
						}
					}				
				}
			}
		}
	}
/****************************十字补线修正************************************/
	if(Right_Inflection_Flag && Right_Asist_row_flag && Left_End_Row != 119 && Right_Find_Point.flag &&
		(Jump_Left[Left_End_Row].column > Jump_Right[Right_End_Row].column || Left_First_column > 18))
	{
		Left_End_Row = 119;
		Jump_Left[Left_End_Row].column = 0;
	}	
	if(Left_Inflection_Flag && Left_Asist_row_flag && Right_End_Row != 119 && Left_Find_Point.flag && 
		((Jump_Left[Left_End_Row].column > Jump_Right[Right_End_Row].column) || Right_First_column < 142))
		{
			Right_End_Row = 119;
			Jump_Right[Right_End_Row].column = 159;
		}
/**********************************找线错误,左边界在右,右边界在左*******************************************/		
	if(Jump_Left[115].column - Jump_Right[115].column > 0 && Right_End_Row < 116 && Left_End_Row < 116)		
	{
		Right_End_Row = 119;
		Left_End_Row =119;
	}
/****************************弯道干扰补线错误**************************************/
	if(Jump_Right[Right_End_Row].column < 35 && Left_Find_Point.flag && abs(Right_End_Row - Left_End_Row) < 12 &&
		Right_Find_Point.row - Left_Find_Point.row > 20 && 
		abs(Left_Find_Point.row - Right_End_Row) < 12 && Jump_Right[Right_End_Row].column - Jump_Left[Left_End_Row].column < 15)
	{
		Left_End_Row = 119;
		Jump_Left[Left_End_Row].column = 0;
	}
	if(Jump_Left[Left_End_Row].column > 125 && Right_Find_Point.flag && abs(Right_End_Row - Left_End_Row) < 12 && 
		Left_Find_Point.row - Right_Find_Point.row > 20 && 
		abs(Left_Find_Point.row - Right_End_Row) < 12 && Jump_Right[Right_End_Row].column - Jump_Left[Left_End_Row].column < 15)
	{
		Right_End_Row = 119;
		Jump_Right[Right_End_Row].column = 0;
	}
	if(Roundabout_Asist_Flag == 0 && MeetingPlace_flag == 0 && CurMeetingPlace_flag == 0 && A_End_D > 28000)//    
	{
//		if(PTB17_I && (TX_TAB_f[0] & 0x08) == 0)
//		{
///*********************************弯道会车区判断****************************************************/
//			Search_Curve_Meeting_Place();
//		}
//		else
		{
/*********************************直道会车区判断****************************************************/
			Search_Straight_Meeting_Place();
		}
	}
/**************************************直道检测**************************************************/
/***************************************左边界检测**************************************************/
	if(Left_Find_Point.flag == 1 && Left_End_Row < 53 && Right_Find_Point.flag == 1 && Right_End_Row < 53 && 
		Right_Find_Point.row - Right_End_Row > 9 && Left_Find_Point.row - Left_End_Row > 9 && 
	((Left_Find_Point.row < 60 && Jump_Left[Left_End_Row + 2].column - Jump_Left[Left_Find_Point.row].column < 
	(Left_Find_Point.row - Left_End_Row - 2) * 1.8) || Left_Find_Point.row >= 60) &&
	((Right_Find_Point.row < 60 && Jump_Right[Right_Find_Point.row].column - Jump_Right[Right_End_Row + 2].column < 
	(Right_Find_Point.row - Right_End_Row - 2) * 1.8) || Right_Find_Point.row >= 60))
	{
		point_count = 0;
		Left_Straight_Line_Flag = 1;
		forecast_column = Jump_Left[ Left_Find_Point.row - 4].column - Jump_Left[ Left_Find_Point.row].column;
		for(row = Left_Find_Point.row; row > 54; row--)
		{
			if((Jump_Left[row].column - Jump_Left[row - 1].column >= 0 || Jump_Left[row].column - Jump_Left[row - 1].column < -4) || 
			(Jump_Left[row - 4].column - Jump_Left[row].column -  forecast_column > 2 && row > 57))
			{
				point_count++;
				if(point_count > 3)
				{
					Left_Straight_Line_Flag = 0; 
					break;
				}
			}
		}
/***************************************右边界检测**************************************************/
		point_count = 0;
		Right_Straight_Line_Flag = 1;
		forecast_column = Jump_Right[Right_Find_Point.row].column - Jump_Right[Right_Find_Point.row - 4].column;
		for(row = Right_Find_Point.row; row > 54; row--)
		{
			if((Jump_Right[row - 1].column - Jump_Right[row].column >= 0 || Jump_Right[row - 1].column - Jump_Right[row].column < -4) ||
				(Jump_Right[row].column - Jump_Right[row - 4].column - forecast_column > 2 && row > 57))
			{
				point_count++;
				if(point_count > 3)
				{
					Right_Straight_Line_Flag = 0;
					break;
				}
			}
		}	
	}
/**************************************根据左右直道和环岛判断是否为直道*******************************************/	
		if(Right_Straight_Line_Flag && Left_Straight_Line_Flag)// && Left_Roundabout_Flag == 0 && Right_Roundabout_Flag == 0
		{
			Straight_Line_Flag = 1;
		}
		else
		{
			Straight_Line_Flag = 0;
		}

/************************************************************检测坡道**************************************************************/
	if(Left_End_Row < 50 && Right_End_Row < 50 && Straight_Line_Flag && Roundabout_Asist_Flag == 0)
	{
		if(Left_End_Row >= Right_End_Row && Jump_Right[Left_End_Row + 2].column - Jump_Left[Left_End_Row + 2].column > 89)
		{
			Ramp_Count++;
			if(Ramp_Flag == 0 && Ramp_Count > 3)
			{
				Ramp_Flag = 1;
			}
			else if(Ramp_Flag == 2)
			{
				Ramp_Flag = 3;
			}
		}
		else if(Right_End_Row > Left_End_Row  && Jump_Right[Right_End_Row + 2].column - Jump_Left[Right_End_Row + 2].column > 89)
		{
			Ramp_Count++;
			if(Ramp_Flag == 0 && Ramp_Count > 3)
			{
				Ramp_Flag = 1;
			}
			else if(Ramp_Flag == 2)
			{
				Ramp_Flag = 3;
			}
		}
		else
		{
			Ramp_Count = 0;
		}
	}
/************************************************************检测十字**************************************************************/
	if(End_Line_Flag == 0 && Right_End_Row < 53 && Left_End_Row < 53 && Roundabout_Asist_Flag == 0 && Ramp_Flag == 0 && Cross_Flag == 0 && 
		Right_Find_Point.flag && Left_Find_Point.flag && ((left_add_line_flag && right_add_line_flag && 
		right_lose_line_row - right_break_find_row > 14 && left_lose_line_row - left_break_find_row > 14 && 
		abs(right_lose_line_row - left_lose_line_row) < 8) || 
		(Left_First_column > 8 && Right_First_column < 151 && 159 - Right_First_column + Left_First_column > 65 &&
		Right_Find_Point.row < 70 && Left_Find_Point.row < 70)) )//&& Start_Distance > 30000 
	{
		Cross_Flag = 1;
	}
	if(Cross_Flag != 0)
	{
		if((Right_End_Row == 119 && Jump_Left[Left_End_Row].column > 110) || (Left_End_Row == 119 && Jump_Right[Right_End_Row].column < 50))
		{
			Cross_Flag = 0;
		}
	}	
/*******************************************环岛处理***************************************************/
	if(Roundabout_Asist_Flag && Left_Roundabout_Flag == 0 && Right_Roundabout_Flag == 0 && PTB17_I == 1)
	{
		if(Left_End_Row == 119 && Right_End_Row != 119)
		{
			Left_Roundabout_Flag = 1;
		}
		else if(Right_End_Row == 119 && Left_End_Row != 119)
		{
			Right_Roundabout_Flag = 1;
		}
		else if(Left_First_column - 159 + Right_First_column > 24 && Right_Find_Point.flag && Left_Find_Point.flag)
		{
			Left_Roundabout_Flag = 1;
		}
		else if(159 - Right_First_column - Left_First_column > 24 && Right_Find_Point.flag && Left_Find_Point.flag)
		{
			Right_Roundabout_Flag = 1;
		}
		else if(Right_Find_Point.flag && Left_Find_Point.flag && (Left_Straight_Line_Flag || Right_Straight_Line_Flag))
		{
			row = Left_Find_Point.row > Right_Find_Point.row ? Right_Find_Point.row - 1 : Left_Find_Point.row - 1;
			Asist_row = Left_End_Row > Right_End_Row ? Left_End_Row + 3 : Right_End_Row + 3;
			for(; row > Asist_row; row--)
			{
				if(Jump_Left[row].column >= Jump_Left[row - 1].column && Jump_Left[row - 1].column >= Jump_Left[row - 2].column &&
					Jump_Left[row - 2].column >= Jump_Left[row - 3].column)
				{
					Left_Roundabout_Flag = 1;
					break;
				}
				else if(Jump_Right[row].column <= Jump_Right[row - 1].column && Jump_Right[row - 1].column <= Jump_Right[row - 2].column &&
					Jump_Right[row - 2].column <= Jump_Right[row - 3].column)
				{
					Right_Roundabout_Flag = 1;
					break;
				}
			}
		}
	}
	if(Left_Roundabout_Flag)
	{
//		if(Left_First_column > 20 && Roundabout_Asist_Flag == 1 &&  Left_Find_Point.flag && Left_Find_Point.row < 60)
//		{
//			Roundabout_Asist_Flag = 2;
//		}
//		if(Roundabout_Asist_Flag == 2 && Left_Find_Point.row > 51 && Left_Find_Point.flag)
//		{
//			Roundabout_Asist_Flag = 3;
//		}
//		else if(Roundabout_Asist_Flag == 3  && Jump_Right[Right_End_Row].column < 80 && ((Left_Find_Point.flag == 0 && Right_End_Row < 65) || 
//			(Left_Find_Point.flag && Left_First_column < 10)))
//		{
//			Roundabout_Asist_Flag = 4;
//		}
//		else if(Roundabout_Asist_Flag == 4 && Right_First_column > 150 && Right_Find_Point.flag && Jump_Right[Right_End_Row].column < 80)
//		{
//			Roundabout_Asist_Flag = 5;
//		}
//		
		if(Roundabout_Asist_Flag == 1 && Right_Find_Point.flag && Jump_Right[Right_End_Row].column < 80 && 
			(Right_First_column > 150 || (Right_Find_Point.row > 110 && Jump_Right[Right_End_Row].column < 40)) && 
		(Left_Find_Point.flag == 0 || (Left_Find_Point.flag && Jump_Left[Left_End_Row + 1].column >= Jump_Left[Left_End_Row].column && (Left_Find_Point.row >= 110 || 
		(Left_Find_Point.row < 110 && Jump_Left[Left_End_Row].column < 12)))))
		{
			lose_count = 0;
			for(column = 0;column < 160;column++)
			{
				Curve_Meeting[column].flag = 0;
			}
			Search_Curve_line(80, 100, 40);
			if(Curve_Meeting[80].flag)
			{
				for(column = 79;column >= 45 ;column--)
				{
					Search_Curve_line(column, Curve_Meeting[column + 1].row + 6, Curve_Meeting[column + 1].row - 6);
					if(Curve_Meeting[column].flag == 0)
					{
						lose_count = 1;
						break;
					}
					else if(column < 74 && Curve_Meeting[column + 6].row - Curve_Meeting[column].row < -1)
					{
						lose_count = 1;
						break;
					}
				}
				if(lose_count == 0)
				{
					for(column = 81;column < 160 ;column++)
					{
						Search_Curve_line(column, Curve_Meeting[column - 1].row + 6, Curve_Meeting[column - 1].row - 6);
						if(Curve_Meeting[column].flag == 0)
						{
							if(Curve_Meeting[column - 1].row > 115)
							{
								break;
							}
							else
							{
								lose_count = 1;
								break;
							}
						}
						else if(Curve_Meeting[column].row - Curve_Meeting[column - 6].row < 0)
						{
							lose_count = 1;
							break;
						}
					}
				}
			}
			else
			{
				lose_count = 1;
			}
			if(lose_count == 0)
			{
				Roundabout_Asist_Flag = 5;
			}
		}
	}
	else if(Right_Roundabout_Flag)
	{
//		if(Right_First_column < 140 && Roundabout_Asist_Flag == 1 && Right_Find_Point.flag && Right_Find_Point.row < 60)
//		{
//			Roundabout_Asist_Flag = 2;
//		}
//		if(Roundabout_Asist_Flag == 2 && Right_Find_Point.row > 51 && Right_Find_Point.flag)
//		{
//			Roundabout_Asist_Flag = 3;
//		}
//		else if(Roundabout_Asist_Flag == 3  && Jump_Left[Left_End_Row].column > 80 && ((Right_Find_Point.flag == 0 &&  Left_End_Row < 65) ||
//			(Right_Find_Point.flag && Right_First_column > 149)))
//		{
//			Roundabout_Asist_Flag = 4;
//		}
//		else if(Roundabout_Asist_Flag == 4 && Left_First_column < 9 && Left_Find_Point.flag && Jump_Left[Left_End_Row].column > 80)
//		{
//			Roundabout_Asist_Flag = 5;
//		}
		if(Roundabout_Asist_Flag == 1 && Left_Find_Point.flag && Jump_Left[Left_End_Row].column > 80 && 
			(Left_First_column < 9 || (Left_Find_Point.row > 110 && Jump_Left[Left_End_Row].column > 120)) && 
		(Right_Find_Point.flag == 0 || (Right_Find_Point.flag && Jump_Right[Right_End_Row + 1].column <= Jump_Right[Right_End_Row].column && (Right_Find_Point.row >= 110 || 
		(Right_Find_Point.row < 110 && Jump_Right[Right_End_Row].column > 148)))))
		{
			lose_count = 0;
			for(column = 0;column < 160;column++)
			{
				Curve_Meeting[column].flag = 0;
			}
			Search_Curve_line(80, 100, 40);
			if(Curve_Meeting[80].flag)
			{
				for(column = 81;column < 115 ;column++)
				{
					Search_Curve_line(column, Curve_Meeting[column - 1].row + 6, Curve_Meeting[column - 1].row - 6);
					if(Curve_Meeting[column].flag == 0)
					{
						lose_count = 1;
						break;
					}
					else if(column > 86 && Curve_Meeting[column - 6].row - Curve_Meeting[column].row < -1)
					{
						lose_count = 1;
						break;
					}
				}
				if(lose_count == 0)
				{
					for(column = 79;column >= 0 ;column--)
					{
						Search_Curve_line(column, Curve_Meeting[column + 1].row + 6, Curve_Meeting[column + 1].row - 6);
						if(Curve_Meeting[column].flag == 0)
						{
							if(Curve_Meeting[column + 1].row > 115)
							{
								break;
							}
							else
							{
								lose_count = 1;
								break;
							}
						}
						else if(Curve_Meeting[column].row - Curve_Meeting[column + 6].row < 0)
						{
							lose_count = 1;
							break;
						}
					}
				}
			}
			else
			{
				lose_count = 1;
			}
			if(lose_count == 0)
			{
				Roundabout_Asist_Flag = 5;
			}
		}
	}
		
}
/***********************提取中线***********************************
*******************************************************************
*******************************************************************/

void Extract_Middle_Line(void)
{
	int16_t far_row;
	float slope_reciprocal;
	int8_t End_Point_count;
	int16_t row, column;
	int16_t black_count = 0;
	int16_t end_line_row;
	int16_t end_line_column;
	
	if(Right_End_Row >= Left_End_Row)
		Middle_End_Row = Right_End_Row - (int16_t)((Right_End_Row - Left_End_Row) * 0.5);
	else   
		Middle_End_Row = Left_End_Row - (int16_t)((Left_End_Row - Right_End_Row) * 0.5);
	if(Right_End_Row == 119 && Left_End_Row < 100 && Left_Asist_row_flag == 0)
	{
		if(Left_Find_Point.row + 3 < 119)
		{
			Jump_Left[119].column += (Left_Find_Point.row - 116) / 2;
		}
	}
	
	if(Left_End_Row == 119 && Right_End_Row < 100 && Right_Asist_row_flag == 0)
	{
		if(Right_Find_Point.row + 3 < 119)
		{
			Jump_Right[119].column += (116 - Right_Find_Point.row) / 2;
		}
	}
	
	for(row = 119; row >= Middle_End_Row; row--)
	{
		if(row >= Right_End_Row && row >= Left_End_Row)
		{
			Jump_Middle[row].column = (Jump_Left[row].column + Jump_Right[row].column) / 2;
		}
		else
		{
			if(Right_End_Row > Left_End_Row)
			{
				slope_reciprocal=400 * (2 * ((Jump_Left[Left_End_Row].column + Jump_Right[Right_End_Row].column) / 2 
				- Jump_Middle[Right_End_Row].column))/ (Absolute(Right_End_Row - Left_End_Row));    //系数400 放大斜率
				Jump_Middle[row].column = Jump_Middle[Right_End_Row].column + slope_reciprocal * (Right_End_Row - row) / 400;
				if(Jump_Middle[row].column >= 159)              //限幅
				{
					Middle_End_Row = row;
					break;
				}
			} 
			else
			{
				slope_reciprocal = 400 * (2 * ((Jump_Left[Left_End_Row].column + Jump_Right[Right_End_Row].column ) / 2
				- Jump_Middle[Left_End_Row].column)) / (Absolute(Right_End_Row - Left_End_Row)) ;
				Jump_Middle[row].column = Jump_Middle[Left_End_Row].column + slope_reciprocal * (Left_End_Row - row) / 400;
				if(Jump_Middle[row].column <= 0)              //限幅
				{
					Middle_End_Row = row;
					break;
				} 
			}
		}
	}	
/**********************终点线检测*******************************************/
	if(Ramp_Flag == 0 && Left_End_Row < 58 && Right_End_Row < 58 && Ramp_Flag == 0 && Meeting_Car_Flag == 0 &&
		Asist_End_LineCount < 2 && A_End_D > 38000 && Roundabout_Asist_Flag == 0)//
	{
		far_row = Left_End_Row > Right_End_Row ? Left_End_Row : Right_End_Row;
		End_Point_count = 0;
		End_Line_Slow_Flag = 0;
		for(row = far_row + 5; row <= Left_Find_Point.row - 4; row++)
		{
			black_count = 0;
			for(column= Jump_Left[row].column + 1; column < Jump_Middle[row].column + 3; column++)
			{
				if((image_buffer[row][column] == Black && image_buffer[row][column + 1] == Black &&
					image_buffer[row][column + 2] == White && image_buffer[row][column + 3] == White) ||
				(image_buffer[row + 1][column] == Black && image_buffer[row + 1][column + 1] == Black &&
					image_buffer[row + 1][column + 2] == White && image_buffer[row + 1][column + 3] == White))
				{
					black_count++;
				}
				if(black_count > 2)
				{
					end_line_row = row;
					End_Point_count++;
					break;
				}
			}
			if(End_Point_count > 1)
			{
				End_Line_Slow_Flag = 1;
				break;
			}
		}
		if(End_Line_Slow_Flag == 1)
		{
			for(row =  end_line_row - 6; row <= end_line_row + 6; row++)
			{
				black_count = 0;
				for(column= Jump_Middle[row].column; column < Jump_Right[row].column; column++)
				{
					if((image_buffer[row][column] == Black && image_buffer[row][column + 1] == Black && 
						image_buffer[row][column + 2] == White && image_buffer[row][column + 3] == White) ||
					(image_buffer[row + 1][column] == Black && image_buffer[row + 1][column + 1] == Black && 
						image_buffer[row + 1][column + 2] == White && image_buffer[row + 1][column + 3] == White))
					{
						black_count++;
					}
					if(black_count > 2)
					{
						Pro_End_Line_Flag = 1;
						Asist_End_Line = 1;
						End_Line_Slow_Flag = 0;
						Asist_End_LineCount++;
						break;
					}
				}
				if(Asist_End_Line == 1)
				{
					Asist_End_Line = 0;
					break;
				}
			}
		}
		
		if(Asist_End_LineCount == 1)
		{
			Asist_End_LineErr++;
		}

		if(Asist_End_LineErr > 2)
		{
			Asist_End_LineErr = 0;
			Asist_End_LineCount = 0;
			Pro_End_Line_Flag = 0;
			End_Distance  = 0;
			Motor_Set_Speed = Speed_0;
		}
			
		if(Pro_End_Line_Flag == 1)
		{
			if(image_buffer[end_line_row - 5][Jump_Middle[end_line_row - 5].column] == Black)
			{
				Pro_End_Line_Flag = 0;
			}
		}
	}
	if(Pro_End_Line_Flag && Meeting_place_count > 1 && Asist_End_LineCount >= 2)
	{
		Test_F1 = 1;
		End_Line_Flag = 1;
	}
	
/************************************检测车****************************************/
	if(Left_End_Row < 58 && Right_End_Row < 58 && Meeting_Car_Flag == 0 && distance_s < 1400 && (RX_TAB_s[0] & 0x20) && Double_Car_Mode &&
		image_buffer[Middle_End_Row + 1][Jump_Middle[Middle_End_Row + 1].column + 8] == Black && 
		image_buffer[Middle_End_Row + 1][Jump_Middle[Middle_End_Row + 1].column - 8] == Black && 
			image_buffer[Middle_End_Row + 1][Jump_Middle[Middle_End_Row + 1].column] == Black && 
			Jump_Right[Middle_End_Row].column > Jump_Left[Middle_End_Row].column && End_Line_Flag == 0 && A_End_D > 28000)// 
	{
		Asist_Meeting_Car_Flag = 1;
		end_line_column = 0;
		for(row = Middle_End_Row + 1;row < Middle_End_Row + 5; row++)
		{
			for(column = Jump_Left[row].column + 2; column <= Jump_Middle[row].column; column++)
			{
				if(image_buffer[row][column] == White && image_buffer[row][column + 1] == Black && image_buffer[row][column + 2] == Black)
				{
					end_line_column = column;
					break;
				}
			}
			if(end_line_column - Jump_Left[row].column > 16)
			{
				Asist_Meeting_Car_Flag = 0;
			}
			else
			{
				for(column = end_line_column + 3;column <= Jump_Middle[row].column; column++)
				{
					if(image_buffer[row][column] == White)
					{
						Asist_Meeting_Car_Flag = 0;
						break;
					}
				}
			}
			
			if(Asist_Meeting_Car_Flag == 0)
			{
				break;
			}
		}
		
		if(Asist_Meeting_Car_Flag == 1)
		{
			Meeting_Car_Flag = 1;
		}
	}
}
/*****************************************************************
****************************计算舵机打角***************************
******************************************************************/
float Image_Differ;
void Calculate_Image_Differ(void)
{
	int16_t row;
	if(Roundabout_Asist_Flag == 3)
	{
		if(Left_Roundabout_Flag)
		{
			Image_Differ = (Left_Find_Point.row - 52) + 35;// + Jump_Left[Left_Find_Point.row].column;
			Image_Differ = Image_Differ > 60 ? 60 : Image_Differ;
		}
		else
		{
			Image_Differ = (52 - Right_Find_Point.row) - 35;// + Jump_Right[Right_Find_Point.row].column;
			Image_Differ = Image_Differ < -60 ? -60 : Image_Differ;
		}
	}
	else
	{
		if(Middle_End_Row < 110)
		{
			Image_Differ = 0;
			for(row = 118; row >= Middle_End_Row; row--)
			{
				Image_Differ += (80 - Jump_Middle[row].column);
			}
			Image_Differ = Image_Differ / (119 - Middle_End_Row);
		}
	}
}

/*******************************************************************
函数名称：int16 Add_Line(int16 x,float k,float b)
参数：所求点横坐标  起始坐标，终止坐标
函数功能： 补线函数  求出纵坐标         x行 Y列
********************************************************************/
int16_t Add_Line(int16_t x,float k,float b)
{
  int16_t y;
  y=(int16_t)(k*x+b);
  return y;
}

/***************寻找左边跳变点******************/
void edge_detection_left(int16_t row, int16_t start_column, int16_t end_column)
{
    int16_t column;
	int16_t Asist_Middle_Column;
	Asist_Middle_Column = Jump_Left[row + 1].column;
	
	if(start_column > 159) start_column = 159;
    if(end_column < 0) end_column = 0;
	if(Asist_Middle_Column > start_column) Asist_Middle_Column = start_column;
	else if(Asist_Middle_Column < end_column) Asist_Middle_Column = end_column;
   
    if(Asist_Middle_Column < 2) Asist_Middle_Column = 2;
	if(start_column > end_column)
	for(column = Asist_Middle_Column - 1; column < start_column; column++)
	{
		if(image_buffer[row][column] == White && image_buffer[row][column-1] == Black)
		{
			Jump_Left[row].column = column;
			Jump_Left[row].flag = 1;
			break;
		}
	}
	if(Jump_Left[row].flag == 0)
	for(column = Asist_Middle_Column - 2; column > end_column; column--)
	{
		if(image_buffer[row][column] == White && image_buffer[row][column-1] == Black)
		{
			Jump_Left[row].column = column;
			Jump_Left[row].flag = 1;
			break;
		}
	}
}

void edge_detection_left1(int16_t row, int16_t start_column, int16_t end_column)
{
    int16_t column;
    
    if(start_column > 159) start_column = 159;
    if(end_column < 0) end_column = 0;
	if(start_column > end_column)
	for(column = start_column; column > end_column; column--)
	{
		if(image_buffer[row][column] == White && image_buffer[row][column-1] == Black)
		{
			Jump_Left[row].column = column;
			Jump_Left[row].flag=1;
			break;
		}
	}
}

/*************寻找右边跳变点********************/
void edge_detection_right(int16_t row,int16_t start_column,int16_t end_column)
{
    int16_t column;
	int16_t Asist_Middle_Column;
	Asist_Middle_Column = Jump_Right[row + 1].column;
	if(start_column < 0) start_column = 0;
    if(end_column > 159) end_column = 159;
	if(Asist_Middle_Column < start_column) Asist_Middle_Column = start_column;
	else if(Asist_Middle_Column > end_column) Asist_Middle_Column = end_column;

    if(Asist_Middle_Column > 157) Asist_Middle_Column = 157;
	if(start_column < end_column)
	for(column = Asist_Middle_Column + 1; column > start_column; column--)
	{
		if(image_buffer[row][column]==White && image_buffer[row][column + 1] == Black)
		{
			Jump_Right[row].column = column;
			Jump_Right[row].flag = 1;
			break;
		}
	}
	if(Jump_Right[row].flag == 0)
	for(column = Asist_Middle_Column + 2; column < end_column; column++)
	{
		if(image_buffer[row][column]==White && image_buffer[row][column + 1] == Black)
		{
			Jump_Right[row].column = column;
			Jump_Right[row].flag = 1;
			break;
		}
	}
}

void edge_detection_right1(int16_t row,int16_t start_column,int16_t end_column)
{
    int16_t column;

    if(start_column < 0) start_column = 0;
    if(end_column > 159) end_column = 159;
	if(start_column < end_column)
	for(column = start_column; column < end_column; column++)
	{
		if(image_buffer[row][column] == White && image_buffer[row][column + 1] == Black)
		{
			Jump_Right[row].column = column;
			Jump_Right[row].flag = 1;
			break;
		}
	}
}

/************************搜寻左边界拐点*********************************/
void Search_LeftInflectionPoint(int16_t startPoint,int16_t endPoint)
{
	int16_t row;
	int16_t j = 0;
	for(row = startPoint; row >= endPoint + 3; row--)
	{
		if(Jump_Left[row].column > j)
		{
			j = Jump_Left[row].column;
			Left_Inflection_Row1 = row;
		}
		else if(Jump_Left[row].column < j && Jump_Left[row-3].column < j && Jump_Left[row-2].column < j && Jump_Left[startPoint].column - j <= -1)
		{
			Left_Inflection_Row2 = row + 1;
			Left_Inflection_Flag = 1;
			break;
		}
	}
	if(Left_Inflection_Flag)
	{
		for(row = Left_Find_Point.row + 2; row > Left_Inflection_Row1;row--)
		{
			if(Jump_Left[row].column > Jump_Left[row - 1].column)
			{
				Left_Inflection_Flag = 0;
				break;
			}
		}
		if(Left_Inflection_Flag)
		{
			for(row = Left_Inflection_Row2 - 1; row > Left_End_Row + 4;row--)
			{
				if(Jump_Left[row - 1].column > Jump_Left[row].column)
				{
					Left_Inflection_Flag = 0;
					break;
				}
			}
		}
	}
}
/**************************搜寻右边界拐点**********************************/
void Search_RightInflectionPoint(int16_t startPoint,int16_t endPoint)
{
	int16_t row;
	int16_t j = 159;
	for(row = startPoint; row >= endPoint + 3; row--)
	{
		if(Jump_Right[row].column < j)
		{
			j = Jump_Right[row].column;
			Right_Inflection_Row1 = row;
		}
		else if(Jump_Right[row].column > j && Jump_Right[row-3].column > j && Jump_Right[row-2].column > j && Jump_Right[startPoint].column - j >= 1)
		{
			Right_Inflection_Row2 = row + 1;
			Right_Inflection_Flag = 1;
			break;
		}  
	} 
	if(Right_Inflection_Flag)
	{
		for(row = Right_Find_Point.row + 2;row > Right_Inflection_Row1; row--)
		{
			if(Jump_Right[row - 1].column > Jump_Right[row].column)
			{
				Right_Inflection_Flag = 0;
				break;
			}
		}
		if(Right_Inflection_Flag)
		{
			for(row = Right_Inflection_Row2 - 1;row > Right_End_Row + 4; row--)
			{
				if(Jump_Right[row].column > Jump_Right[row - 1].column)
				{
					Right_Inflection_Flag = 0;
					break;
				}
			}
		}
	}
}
/*************************************直道寻找会车区*********************************/
int8_t Left_MeetingPlace_flag = 0;
int8_t Right_MeetingPlace_flag = 0;
int8_t MeetingPlace_flag = 0;
int16_t RJudge_MPlace_row[3] = {0};
int8_t RJudge_MPlace_flag[3] = {0};
int16_t Jump_count = 0;
int16_t Inflection_count = 0;
int16_t LJudge_MPlace_row[3] = {0};
int8_t LJudge_MPlace_flag[3] = {0};
int16_t Meeting_place_count = 0;
void Search_Straight_Meeting_Place(void)
{
	int16_t row;
	int16_t i = 0;
	Jump_count = 0;
	Inflection_count = 0;
	Left_MeetingPlace_flag = 0;
	Right_MeetingPlace_flag = 0;
	LJudge_MPlace_flag[0] = 0;
	LJudge_MPlace_flag[1] = 0;
	LJudge_MPlace_flag[2] = 0;
	
	RJudge_MPlace_flag[0] = 0;
	RJudge_MPlace_flag[1] = 0;
	RJudge_MPlace_flag[2] = 0;
	
/**********************************左边界********************************************/	
	if(Left_End_Row < 55 && Right_End_Row < 55)
	{
		for(row = Left_Find_Point.row; row > Left_End_Row; row--)
		{
			if(Jump_Left[row].column > i)
			{
				i = Jump_Left[row].column;
			}
			else if(Jump_Left[row].column < i && Jump_Left[row - 2].column < Jump_Left[row - 3].column)
			{
				LJudge_MPlace_row[0] = row + 1;
				LJudge_MPlace_flag[0] = 1;
				Inflection_count++;
				break;
			}
			if(Jump_Left[row - 1].column - Jump_Left[row].column > 6)
			{
				LJudge_MPlace_row[0] = row;
				LJudge_MPlace_flag[0] = 1;
				Jump_count++;
				break;
			}
		}
		if(LJudge_MPlace_flag[0] && Left_Find_Point.row - LJudge_MPlace_row[0] < 13)
		{
			i = 0;
			for(row = LJudge_MPlace_row[0] - 2; row > Left_End_Row; row--)
			{
				if(Jump_Left[row].column > i)
				{
					i = Jump_Left[row].column;
				}
				else if(Jump_Left[row].column < i && Jump_Left[row - 2].column < Jump_Left[row - 3].column)
				{
					LJudge_MPlace_row[1] = row + 1;
					LJudge_MPlace_flag[1] = 1;
					Inflection_count++;
					break;
				}
				if(Jump_Left[row - 1].column - Jump_Left[row].column > 4)
				{
					LJudge_MPlace_row[1] = row;
					LJudge_MPlace_flag[1] = 1;
					Jump_count++;
					break;
				}
			}
		}
		if(LJudge_MPlace_flag[1])
		{
			i = 0;
			for(row = LJudge_MPlace_row[1] - 2; row > Left_End_Row; row--)
			{
				if(Jump_Left[row].column > i)
				{
					i = Jump_Left[row].column;
				}
				else if(Jump_Left[row].column < i && Jump_Left[row - 2].column < Jump_Left[row - 3].column)
				{
					LJudge_MPlace_row[2] = row + 1;
					LJudge_MPlace_flag[2] = 1;
					Inflection_count++;
					break;
				}
				if(Jump_Left[row - 1].column - Jump_Left[row].column > 3)
				{
					LJudge_MPlace_row[2] = row;
					LJudge_MPlace_flag[2] = 1;
					Jump_count++;
					break;
				}
			}
		}
		
		if(LJudge_MPlace_flag[0] && LJudge_MPlace_flag[1] && LJudge_MPlace_flag[2] && Jump_count != 0 && Inflection_count != 0)
		{
			if(LJudge_MPlace_row[0] - LJudge_MPlace_row[1] < (10 + (Left_Find_Point.row - 74) / 4) &&
				LJudge_MPlace_row[1] -LJudge_MPlace_row[2] < (10 + (Left_Find_Point.row - 74) / 5))
			{
				Left_MeetingPlace_flag = 1;
			}
		}
/********************************************右边界***********************************************/	
		if(Left_MeetingPlace_flag)
		{
			i = 159;
			Inflection_count = 0;
			Jump_count = 0;
			for(row = Right_Find_Point.row; row > Right_End_Row; row--)
			{
				if(Jump_Right[row].column < i)
				{
					i = Jump_Right[row].column;
				}
				else if(Jump_Right[row].column > i && Jump_Right[row - 2].column > Jump_Right[row - 3].column)
				{
					RJudge_MPlace_row[0] = row + 1;
					RJudge_MPlace_flag[0] = 1;
					Inflection_count++;
					break;
				}
				if(Jump_Right[row].column - Jump_Right[row - 1].column > 6)
				{
					RJudge_MPlace_row[0] = row;
					RJudge_MPlace_flag[0] = 1;
					Jump_count++;
					break;
				}
			}
			if(RJudge_MPlace_flag[0] && Right_Find_Point.row - RJudge_MPlace_row[0] < 13)
			{
				i = 159;
				for(row = RJudge_MPlace_row[0] - 2; row > Right_End_Row; row--)
				{
					if(Jump_Right[row].column < i)
					{
						i = Jump_Right[row].column;
					}
					else if(Jump_Right[row].column > i && Jump_Right[row - 2].column > Jump_Right[row - 3].column)
					{
						RJudge_MPlace_row[1] = row + 1;
						RJudge_MPlace_flag[1] = 1;
						Inflection_count++;
						break;
					}
					if(Jump_Right[row].column - Jump_Right[row - 1].column > 4)
					{
						RJudge_MPlace_row[1] = row;
						RJudge_MPlace_flag[1] = 1;
						Jump_count++;
						break;
					}
				}
			}
			if(RJudge_MPlace_flag[1])
			{
				i = 159;
				for(row = RJudge_MPlace_row[1] - 2; row > Right_End_Row; row--)
				{
					if(Jump_Right[row].column < i)
					{
						i = Jump_Right[row].column;
					}
					else if(Jump_Right[row].column > i && Jump_Right[row - 2].column > Jump_Right[row - 3].column)
					{
						RJudge_MPlace_row[2] = row + 1;
						RJudge_MPlace_flag[2] = 1;
						Inflection_count++;
						break;
					}
					if(Jump_Right[row].column - Jump_Right[row - 1].column > 3)
					{
						RJudge_MPlace_row[2] = row;
						RJudge_MPlace_flag[2] = 1;
						Jump_count++;
						break;
					}
				}
			}
			
			if(RJudge_MPlace_flag[0] && RJudge_MPlace_flag[1] && RJudge_MPlace_flag[2] && Jump_count != 0 && Inflection_count != 0)
			{ 
				if(RJudge_MPlace_row[0] - RJudge_MPlace_row[1] < (10 + (Right_Find_Point.row - 74) / 4) && 
					RJudge_MPlace_row[1] -RJudge_MPlace_row[2] < (10 + (Right_Find_Point.row - 74) / 5))
				{
					Right_MeetingPlace_flag = 1;
				}
			}
		}
	}
	if(Right_MeetingPlace_flag && Left_MeetingPlace_flag)
	{
		Meeting_place_count++;
	}
	else
	{
		Meeting_place_count = 0;
	}
	if(Meeting_place_count > 2)
	{
		if(TX_TAB_f[0] & 0x08)
		{
			Asist_End_LineCount = 2;
			End_Line_Flag = 1;
			Test_F1 = 2;
		}
		else
		{
			MeetingPlace_flag = 1;
		}
	}
}
/**********************************弯道寻找会车区*************************************************/
First_Point Curve_Meeting[Column_Max];
int8_t CurMeetingPlace_flag = 0;
void Search_Curve_Meeting_Place(void)
{
	int16_t column;
	int8_t jump_count = 0;
	int8_t lose_flag = 0;
	int16_t rmax_column;
	int16_t asist_row[2];
	Right_MeetingPlace_flag = 0;
	Left_MeetingPlace_flag = 0;
	//CurMeetingPlace_flag = 0;
	if(Left_End_Row == 119 )//&& ((Right_Find_Point.row - Right_End_Row < 35 && Right_Find_Point.flag && Jump_Right[Right_End_Row].column > 70) ||
		//Right_Find_Point.flag == 0)
	{
		for(column = 0;column < 160;column++)
		{
			Curve_Meeting[column].flag = 0;
		}
		Search_Curve_line(80, 100, 55);
		if(Curve_Meeting[80].flag)
		{
			rmax_column = 157;
			for(column = 79;column >= 0 ;column--)
			{
				if(column < 30)
				{
					Search_Curve_line(column, Curve_Meeting[column + 1].row + 5, Curve_Meeting[column + 1].row - 5);
				}
				else
				{
					Search_Curve_line(column, Curve_Meeting[column + 1].row + 10, Curve_Meeting[column + 1].row - 10);
				}
				if(Curve_Meeting[column].flag == 0)
				{
					lose_flag = 1;
					break;
				}
				if(column < 70)
				{
					if(Curve_Meeting[column].row - Curve_Meeting[column + 8].row > 2)
					{
						lose_flag = 1;
						break;
					}
				}
			}
			if(lose_flag == 0 && abs(Curve_Meeting[1].row - Curve_Meeting[19].row) < 6)
			{
				for(column = 81;column < 160 ;column++)
				{
					Search_Curve_line(column, Curve_Meeting[column - 1].row + 15, Curve_Meeting[column - 1].row - 15);
					if(Curve_Meeting[column].flag == 0)
					{
						if(Curve_Meeting[column - 1].row > 100 && column > 120)
						{
							rmax_column = column - 1;
							if(rmax_column > 157) rmax_column = 157;
							break;
						}
						else
						{
							lose_flag = 1;
							break;
						}
					}
				}
				if(lose_flag == 0)
				{
					RightCur_InfFlag1 = 0;
					RightCur_InfFlag2 = 0;
					if(Right_Find_Point.flag)
					{
						Search_RightCur(Right_Find_Point.row,Right_End_Row);
					}
					for(column = rmax_column;column > 40; column--)
					{
						 if(Curve_Meeting[column - 1].row - Curve_Meeting[column].row > 0)
						 {
							 asist_row[jump_count] = Curve_Meeting[column - 2].row;
							 jump_count++;
							 if((jump_count == 1 && column < 70 && rmax_column > 155) || 
								 Curve_Meeting[column - 3].row - Curve_Meeting[column].row <= 0)
							 {
								 break;
							 }
							 if(!(Curve_Meeting[column].row == Curve_Meeting[column + 1].row || Curve_Meeting[column + 2].row == Curve_Meeting[column + 1].row) && column < rmax_column)// 
							 {
								 lose_flag = 1;
								 break;
							 }
							 else
							 {
								 column -= 7;
							 }
							 if(jump_count == 2 && asist_row[0] - asist_row[1] < 5)
							 {
								 break;
							 }
							 if(jump_count == 2 && lose_flag == 0)
							 {
								 Right_MeetingPlace_flag = 1;
								 break;
							 }
						 }
						 if(Curve_Meeting[column].row - Curve_Meeting[column - 1].row > 2 && RightCur_InfFlag2 )//&& Curve_Meeting[column].row - RightCur_Inf1 < 3
						 {
							 asist_row[jump_count] = Curve_Meeting[column].row;
							 jump_count++;
							 if(jump_count == 2 && asist_row[0] - asist_row[1] < 5)
							 {
								 break;
							 }
							 if(jump_count == 2)
							 {
								 Right_MeetingPlace_flag = 1;
								 break;
							 }
							  column -= 3;
						 }
					}
				}
			}
		}
	}
	else if(Right_End_Row == 119)// && ((Left_Find_Point.row - Left_End_Row < 35 && Left_Find_Point.flag && Jump_Left[Left_End_Row].column < 90) ||
		//Left_Find_Point.flag == 0)
	{
		for(column = 0;column < 160;column++)
		{
			Curve_Meeting[column].flag = 0;
		}
		Search_Curve_line(80, 100, 55);
		if(Curve_Meeting[80].flag)
		{
			rmax_column = 2;
			for(column = 81;column < 160 ;column++)
			{
				if(column > 130)
				{
					Search_Curve_line(column, Curve_Meeting[column - 1].row + 5, Curve_Meeting[column - 1].row - 5);
				}
				else
				{
					Search_Curve_line(column, Curve_Meeting[column - 1].row + 10, Curve_Meeting[column - 1].row - 10);
				}
				if(Curve_Meeting[column].flag == 0)
				{
					lose_flag = 1;
					break;
				}
				if(column > 90)
				{
					if(Curve_Meeting[column].row - Curve_Meeting[column - 8].row > 2)
					{
						lose_flag = 1;
						break;
					}
				}
			}
			if(lose_flag == 0 && abs(Curve_Meeting[158].row - Curve_Meeting[140].row) < 6)
			{
				for(column = 79;column >= 0 ;column--)
				{
					Search_Curve_line(column, Curve_Meeting[column + 1].row + 15, Curve_Meeting[column + 1].row - 15);
					if(Curve_Meeting[column].flag == 0)
					{
						if(Curve_Meeting[column + 1].row > 100 && column < 40)
						{
							rmax_column = column + 1;
							if(rmax_column < 2) rmax_column = 2;
							break;
						}
						else
						{
							lose_flag = 1;
							break;
						}
					}
				}
				if(lose_flag == 0)
				{
					LeftCur_InfFlag1 = 0;
					LeftCur_InfFlag2 = 0;
					if(Left_Find_Point.flag)
					{
						Search_LeftCur(Left_Find_Point.row,Left_End_Row);
					}
					for(column = rmax_column;column < 120; column++)
					{
						 if(Curve_Meeting[column + 1].row - Curve_Meeting[column].row > 0)
						 {
							 asist_row[jump_count] = Curve_Meeting[column + 2].row;
							 jump_count++;
							 if((jump_count == 1 && column > 90 && rmax_column < 5) ||
								 Curve_Meeting[column + 3].row - Curve_Meeting[column].row <= 0)
							 {
								 break;
							 }
							 if(!(Curve_Meeting[column].row == Curve_Meeting[column - 1].row || Curve_Meeting[column - 2].row == Curve_Meeting[column - 1].row) && column > rmax_column )// 
							 {
								 lose_flag = 1;
								 break;
							 }
							 else
							 {
								 column += 7;
							 }
							 if(jump_count == 2 && asist_row[0] - asist_row[1] < 5)
							 {
								 break;
							 }
							 if(jump_count == 2 && lose_flag == 0)
							 {
								 Left_MeetingPlace_flag = 1;
								 break;
							 }
						 }
						 if(Curve_Meeting[column].row - Curve_Meeting[column + 1].row > 2 && LeftCur_InfFlag2 && Curve_Meeting[column].row - LeftCur_Inf1 < 3)
						 {
							 asist_row[jump_count] = Curve_Meeting[column].row;
							 jump_count++;
							 if(jump_count == 2 && asist_row[0] - asist_row[1] < 5)
							 {
								 break;
							 }
							 if(jump_count == 2)
							 {
								 Left_MeetingPlace_flag = 1;
								 break;
							 }
							  column += 3;
						 }
		
					}
				}
			}
		}
	}
	
	if(Right_MeetingPlace_flag || Left_MeetingPlace_flag)
	{
		Meeting_place_count++;
	}
	else
	{
		Meeting_place_count = 0;
	}
	if(Meeting_place_count > 1)
	{
		CurMeetingPlace_flag = 1;
	}
}

void Search_Curve_line(int16_t column, int16_t lower, int16_t higher)
{
	int16_t row;
    if(lower > 119) lower = 119;
    if(higher < 1) higher = 1;
	if(lower > higher)
	for(row = lower; row > higher; row--)
	{
		if(image_buffer[row][column] == White && image_buffer[row - 1][column] == Black)
		{
			Curve_Meeting[column].row = row;
			Curve_Meeting[column].flag = 1;
			break;
		}
	}
}
/************************搜寻左边会车区界拐点*********************************/
int16_t LeftCur_Inf1 = 0;
int8_t LeftCur_InfFlag1 = 0;
int8_t LeftCur_InfFlag2 = 0;
void Search_LeftCur(int16_t startPoint,int16_t endPoint)
{
	int16_t row;
	int16_t j = 0;
	for(row = startPoint; row >= endPoint + 3; row--)
	{
		if(Jump_Left[row].column >= j)
		{
			j = Jump_Left[row].column;
		}
		else if(Jump_Left[startPoint].column - j < 0 && Jump_Left[row-1].column < Jump_Left[row].column)
		{
			LeftCur_InfFlag1 = 1;
			LeftCur_Inf1 = row + 1;
			break;
		}
		if(Jump_Left[row].column > 80)
		{
			break;
		}
	}
	if(LeftCur_InfFlag1)
	{
		for(row = LeftCur_Inf1 - 1; row >= endPoint + 3; row--)
		{
			if(Jump_Left[row].column <= j)
			{
				j = Jump_Left[row].column;
			}
			else if(Jump_Left[row - 2].column > Jump_Left[row].column)
			{
				LeftCur_InfFlag2 = 1;
				break;
			}
		}
	}
}
/**************************搜寻右边界会车区拐点**********************************/
int16_t RightCur_Inf1 = 0;
int8_t RightCur_InfFlag1 = 0;
int8_t RightCur_InfFlag2 = 0;
void Search_RightCur(int16_t startPoint,int16_t endPoint)
{
	int16_t row;
	int16_t j = 159;
	for(row = startPoint; row >= endPoint + 3; row--)
	{
		if(Jump_Right[row].column <= j)
		{
			j = Jump_Right[row].column;
		}
		else if(Jump_Right[row].column < Jump_Right[row-1].column && Jump_Right[startPoint].column - j >= 1)
		{
			RightCur_Inf1 = row + 1;
			RightCur_InfFlag1 = 1;
			break;
		}  
		if(Jump_Right[row].column < 80)
		{
			break;
		}
	}
	if(RightCur_InfFlag1)	
	{
		for(row = RightCur_Inf1 - 1; row >= endPoint + 3; row--)
		{
			if(Jump_Right[row].column >= j)
			{
				j = Jump_Right[row].column;
			}
			else if(Jump_Right[row].column > Jump_Right[row-2].column)
			{
				RightCur_InfFlag2 = 1;
				break;
			}  
		}
	}
}

/*********************************************************
函数名称： Transform_Image()
函数功能： 将图像经过一定处理，显示在上位机上
*********************************************************/
void Transform_Send_Image(void)
{
	int16_t row,column;
	for(row = Row_Max-1; row >= 0; row--)
	{
		for(column = 0; column < Column_Max; column++)
		{     
			if(column == Jump_Left[row].column && Jump_Left[row].flag==1 && row >= Left_End_Row )
			{
				image_buffer[row][column] = White;    //左黑线发送                           
			}
			else if(column == Jump_Middle[row].column && row >= Middle_End_Row)
			{
				image_buffer[row][column] = White;  //中线发送
			}         
			else if((column == Jump_Right[row].column && Jump_Right[row].flag == 1) && row >= Right_End_Row)
			{
				image_buffer[row][column] = White;                               //右黑线发送 
			}
			else if(image_buffer[row][column] == White)
			{
				image_buffer[row][column] = Grey; 
			}
		}
	}
	Seng_Image();
}

