#ifndef _ENCODER1_H_
#define _ENCODER1_H_

#include "include.h"
#include "common.h" 

extern void count_get_test();
extern void count_get();


extern int16 count_buff[2];


extern int16 count_A;
extern int16 count_B;
extern int16 count_C;
extern int16 count_D;

extern int16 count_LF;
extern int16 count_LB;
extern int16 count_RF;
extern int16 count_RB;
extern int16 count_LB1;

extern int Encoder[5];
extern void receive_count();

#endif