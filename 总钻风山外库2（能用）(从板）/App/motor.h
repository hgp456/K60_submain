#ifndef _MOTOR_H_
#define _MOTOR_H_


extern void Motor_Init(void);
extern void Set_Motor(float MotorA, float MotorB, float MotorC, float MotorD);

extern void Move_Forward(void);  //向前运动
extern void Move_Left(void);     //向左运动
extern void Move_Back(void);     //向后运动
extern void Move_Right(void);    //向右运动

extern void Turn_Left(void);      //饶yaw轴左旋转
extern void Turn_Right(void);     //饶yaw轴右旋转

extern void Stop(void);

extern void MotorA_FWD(void);     //正转，逆时针转动
extern void MotorA_REV(void);     //反转，顺时针转动
extern void MotorA_STOP(void);    //刹停，采用两路IN都为高电平输入来刹车
extern void MotorA_UNSTOP(void);  //解除刹车状态，运动状态仍与刹车前一致


extern void MotorB_FWD(void);
extern void MotorB_REV(void);
extern void MotorB_STOP(void);
extern void MotorB_UNSTOP(void);

extern void MotorC_FWD(void);
extern void MotorC_REV(void);
extern void MotorC_STOP(void);
extern void MotorC_UNSTOP(void);

extern void MotorD_FWD(void);
extern void MotorD_REV(void);
extern void MotorD_STOP(void);
extern void MotorD_UNSTOP(void);

extern void  Motor_FWD(int high,int low,char wheel);
extern void Motor_FORWARD();
extern void Motor_Circle_Right();
extern void Motor_Circle_Left();

#endif
