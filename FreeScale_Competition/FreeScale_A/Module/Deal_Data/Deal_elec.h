#ifndef _DATA_ELEC_H_
#define _DATA_ELEC_H_


#define abs(n) ((n)>0?(n):(-(n)))  //绝对值定义

extern int32_t AD[6];
extern float tmpAD[6];
extern int32_t GetADMax[6],GetADMin[6];
extern int G_AD[6];
extern float Cbh01, CbhOld01,Cbh34, CbhOld34;
extern int DirGan,DirGanOld;
extern float OffsetDan;
extern int16_t L_AD[6];
extern int8_t Cross_Flag;
extern int16_t R_ADMax;
extern int16_t Sum_T;
extern int8_t Roundabout_Flag;
extern int8_t Roundabout_Asist_Flag;
extern int32_t Roundabout_Distance;
extern int16_t Roundabout_Count;
extern int8_t Left_Roundabout_Flag;
extern int8_t Right_Roundabout_Flag;
extern int16_t Change_R_Coumt;
extern int min01,max01,min34,L_min01;

void GetHengMaxMin(void);
void GetShuMaxMin(void);
void GetADValue(void);
void GuiYi(void);
void Deal_DianGan(void);	
void GetADValue_filter(void);
void GuiYi_double(void);
void Round(void);


#endif
