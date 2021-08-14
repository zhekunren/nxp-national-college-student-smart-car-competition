#ifndef __MANDARIN_H__
#define __MANDARIN_H__

typedef struct
{
    int32_t SetDist;
    int32_t ActualDist;
    float P;
    float I;
    float D;
    int32_t Err;
    int32_t LE;
	int32_t DE;
	int32_t SE;
	int16_t OUTPUT;
    int32_t Max;
}PID_Mandarin;

extern PID_Mandarin Mandarin;

extern uint32_t distance;
extern uint32_t distance_set;
extern int16_t speed_add;
extern uint32_t distance_s;

void mandarin_init(void);
void mandarin_pit2_init(void);
void mandarin_gpio_init(void);

//2017.03.20 19.16 ++‘ß—ÏPID
void Mandarin_PID_SetMax(PID_Mandarin* Newstruct, int32_t max);
void Mandarin_PID_Init(PID_Mandarin* Newstruct, float kp, float ki, float kd);
int16_t Mandarin_PID_Calculate(PID_Mandarin* Newstruct, int32_t sdist, int32_t acdist);

#endif
