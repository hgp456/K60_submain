/*

 Send.h
 

*/
#include "common.h"


#ifndef Send_H_
#define Send_H_

#define BYTE0(dwTemp)           (*(char *)(&dwTemp))
#define BYTE1(dwTemp)           (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)           (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)           (*((char *)(&dwTemp) + 3))

extern void SendBox(int16 admin[3]);
extern void recieve_check();
extern void nrf_send();

extern int16 BOX[9];    //匿名科创发送数组
extern uint8 buff[DATA_PACKET]; 
extern uint8 receive_flag;
   extern     int barrier_Y ;
   extern       int Pix_Data_white_len;
   extern      int Pix_Data_white_right_len;
   extern        int Pix_Data_white_left_len;
   extern uint8  Vx2 ;
#endif