#ifndef _Servo_Image_Out_H_
#define _Servo_Image_Out_H_

#define PwmServoMid   857
extern int16_t PwmServoOut;

extern float DirectionKp, DirectionKd;
extern int16_t DirectionPDout;
extern float ExpectOffset;
extern float ExpectOffset, ExpectOffset_old;
extern float MpuNowError, MpuLastError;
extern int16_t PDoutAll;
extern float Steering_P, Steering_D;
extern float T_ExpectOffset_old;



typedef struct
{
	float P;
	float D;
	float OUT;
}Servo_Engine;
extern Servo_Engine Servo;

void MpuChuanJiPID(void);
void Servo_Image_Out(void);
void Servo_Image_Init(void);
void Servo_DinaGan_Out(void);
void servo_gyro_out(void);
void Servo_DinaGan_Out(void);
void Steering_Init(void);

#endif
