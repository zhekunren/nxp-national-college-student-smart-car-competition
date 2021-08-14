#ifndef _SPEED_CONTROL_H_
#define _SPEED_CONTROL_H_


typedef struct PID_STRUCT_POS
{
    float P;	//P����
    float I;	//I����
    float D;	//D����
	float Err;
    float I_Out_Sum;	//���������
    float Err_Last;		//�ϴμ���ƫ��
	
    float Limit_Up;		//�������޷�
    float Limit_Dn;		//�������޷�
    int16_t Out;
	int16_t Set_Speed;
    float I_Err_Sprt;	//���ַ���ƫ����ֵ
}Pid_Pos_T;


typedef struct PID_STRUCT_INC
{
    float P;	//P����
    float I;	//I����
    float D;	//D����
	float Err;

    float Err_Last;		//ǰ�μ���ƫ��

    float Err_Next;		//�ϴμ���ƫ��
    
    float Tdifferential;
    float Err_Pwm;    //³��
    
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
