#ifndef _MEETING_CAR_H_
#define _MEETING_CAR_H_

extern int8_t Waiting_Flag;
extern int8_t Meeting_Car_Choice_Flag;
extern int8_t End_Meeting_Flag;
extern int8_t Double_Car_Mode;
extern int8_t Meeting_Back_Flag;
extern int16_t Double_Car_Count;
extern int8_t Cur_Enable_M_Car;
extern int32_t Cur_Dis;
extern int32_t Cur_Buffer_D;
extern int8_t Cur_M_A_f;
extern int32_t A_End_D;
extern int32_t UpRamp_Distance;
extern int32_t DownRamp_Distance;
extern int16_t Test_Tx_C;
extern int8_t End_Falg;
extern int8_t Asist_Meeting_Flag;
extern int8_t Test_F;
extern int8_t Test_F1;
extern int8_t Test_F2;
//extern int32_t R_D;



void T_Data(void);
void R_Data(void);
void Meeting_Car_Process(void);
void Meeting_Speed_Choice(void);




#endif
