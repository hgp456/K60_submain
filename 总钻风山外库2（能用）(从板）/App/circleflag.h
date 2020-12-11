#ifndef CIRCLEFLAG_H_
#define CIRCLEFLAG_H_

void motor_circle_judgement();
extern void mode_choose();
extern int Circle_Right_Duty[4]; //向右转
extern int Circle_Left_Duty[4]; //向左转
extern int Line_Left_Duty[4]; // 直走靠左走
extern int Line_Right_Duty[4]; //直走靠右走
extern int Left_Sidesway[4]; // 向左横移
extern int Right_Sidesway[4]; //向右横移
extern int Line_Back_Duty[4];//倒车
extern uint8 ted_flag;
#endif