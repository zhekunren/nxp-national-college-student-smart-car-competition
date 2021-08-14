#ifndef _DEAL_IMAGE_H_
#define _DEAL_IMAGE_H_

#define Range_Column  9

#define  Row_Max     CAMERA_H 
#define  Column_Max  CAMERA_W 
#define  Middle_Column    (Column_Max/2) 
#define  White       254
#define  Black       0
#define  Grey        80


typedef struct
{
    int16_t column;
    int8_t flag;
}IMAGE;

typedef struct
{
	int8_t flag;
	int16_t row;
}First_Point;



extern int16_t Left_End_Row;
extern int16_t Right_End_Row;
extern int16_t Middle_End_Row;
extern int8_t Ramp_Flag;
extern float Image_Differ;
extern IMAGE Jump_Left[Row_Max];
extern IMAGE Jump_Right[Row_Max];
extern int8_t Straight_Line_Flag;
extern int8_t End_Line_Flag;
extern int8_t Pro_End_Line_Flag;
extern int8_t Enable_Meeting_Car;
extern int8_t Meeting_Car_Flag;
extern int32_t End_Distance;
extern int8_t MeetingPlace_flag;
extern int8_t left_add_line_flag;
extern int8_t right_add_line_flag;
extern int8_t CurMeetingPlace_flag;
extern int8_t Left_MeetingPlace_flag;
extern int8_t Right_MeetingPlace_flag;
extern int16_t LeftCur_Inf1;
extern int8_t LeftCur_InfFlag1;
extern int8_t LeftCur_InfFlag2;
extern int16_t RightCur_Inf1;
extern int8_t RightCur_InfFlag1;
extern int8_t RightCur_InfFlag2;
extern First_Point Left_Find_Point;
extern First_Point Right_Find_Point;
extern int16_t Meeting_place_count;
extern int8_t Asist_End_LineCount;
extern int8_t Asist_End_LineErr;
extern First_Point Curve_Meeting[Column_Max];


void Calculate_Image_Differ(void);
void Search_Black_Edge(void);
void Extract_Middle_Line(void);
int16_t Add_Line(int16_t x,float k,float b);
void edge_detection_left(int16_t row,int16_t start_column,int16_t end_column);
void edge_detection_left1(int16_t row, int16_t start_column, int16_t end_column);
void edge_detection_right(int16_t row,int16_t start_column,int16_t end_column);
void edge_detection_right1(int16_t row,int16_t start_column,int16_t end_column);
void Transform_Send_Image(void);
void Search_LeftInflectionPoint(int16_t startPoint,int16_t endPoint);
void Search_RightInflectionPoint(int16_t startPoint,int16_t endPoint);
void Search_Straight_Meeting_Place(void);
void Search_Curve_Meeting_Place(void);
void Search_Curve_line(int16_t column, int16_t lower, int16_t higher);
void Search_LeftCur(int16_t startPoint,int16_t endPoint);
void Search_RightCur(int16_t startPoint,int16_t endPoint);



#endif
