#ifndef _SPEED_CONTROL_H_
#define _SPEED_CONTROL_H_


typedef struct PID_STRUCT_POS
{
    float P;	//P参数
    float I;	//I参数
    float D;	//D参数
	float Err;
    float I_Out_Sum;	//积分输出和
    float Err_Last;		//上次计算偏差
	
    float Limit_Up;		//积分上限幅
    float Limit_Dn;		//积分下限幅
    int16_t Out;
	int16_t Set_Speed;
    float I_Err_Sprt;	//积分分离偏差阈值
}Pid_Pos_T;


typedef struct PID_STRUCT_INC
{
    float P;	//P参数
    float I;	//I参数
    float D;	//D参数
	float Err;

    float Err_Last;		//前次计算偏差

    float Err_Next;		//上次计算偏差
    
    float Tdifferential;
    float Err_Pwm;    //鲁棒
    
    int16_t Out;
	int16_t Set_Speed;
}Pid_Inc_T;


extern Pid_Inc_T Left;
extern Pid_Inc_T Right;
extern Pid_Pos_T E_Distance;
extern Pid_Pos_T M_Distance;
extern Pid_Pos_T C_Distance;
extern Pid_Pos_T B_Distance;
extern int16_t Diff_Speed;
extern int16_t Motor_Set_Speed;
extern int32_t Start_Distance;
extern int16_t Left_Actual_Speed, Right_Actual_Speed;
extern int16_t Left_Pulse, Right_Pulse;
extern int Speed_0;
extern int8_t Start_Flag;
extern float Coefficient_S;


void Pid_Position(Pid_Pos_T *pid, float set, float real);
void Pid_Increment(Pid_Inc_T *pid, float set, float real);
void Crl_Pid_Init(void);
float data_3Power_SCurve(float x, float maxK, float maxPos, float scale, float n);
void Speed_Calculate(void);
void Motor_Out_Pwm(void);


#endif
