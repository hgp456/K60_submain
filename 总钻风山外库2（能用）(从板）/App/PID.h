#ifndef _PID_H_
#define _PID_H_

#include "motor.h"

#define minimum 0
#define maximum 1000
extern int Aim_Speed[4];
extern int Encoder[5];

typedef struct
{
	float Kp;
	float Ki;
	float Kd;

	float err;
	float ierr;
	float derr;

	float err_last;
	
	float Pout;
	float Iout;
	float Dout;

	float Iout_MAX;

	float OUT;

	float  c1 ;  //Œ¢∑÷œ»––
	float  c2 ;
	float  c3 ;
	float  temp ;
        
        float err_last_1;
        float err_last_2;


}PID;
        
extern void Set_Motor(float PID_Motor_0, float PID_Motor_1, float PID_Motor_2, float PID_Motor_3);
extern float Get_MxMi(float num, float max, float min);
extern void PID_Init(void);
extern void motor_circle_judgement();
extern void pid();
extern void Receive_PID();
extern void pid_set();


#endif
