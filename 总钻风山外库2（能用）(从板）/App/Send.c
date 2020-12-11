/*

 Send.c
 Created on 2016.3.12
 By Yuxin Zheng

*/

#include "common.h"
#include "include.h"
#include "Send.h"
#include "PID.h"
#include "analysis.h"
#include "motor.h"
#include "init.h"
#include "Encoder1.h"

uint8 buff[DATA_PACKET]; 
int16 BOX[9]={0,0,0,0,0,0,0,0,0};      //匿名科创发送数组
uint8 receive_flag=0;



void nrf_send()
{
   BOX[0]=(uint8)Vx2;
  //BOX[0]=line_flag;
  //BOX[1]=pre_line_flag;
   BOX[1]=Pix_Data_white_left_len;
//   BOX[0]=Rmotor_PID->count;
 // BOX[1]=Lmotor_PID->count;
  
   BOX[2]=Pix_Data_white_right_len;
//   BOX[3]=(uint8)avercount;
 //  BOX[3]=SERVODUTY;
  BOX[3]=(uint8)barrier_Y;

 //  BOX[3]=speedflag_while_circle*10;
 //  BOX[3]=duzhuan_flag;
 //  BOX[4]=circle_count;
  //   BOX[4]=slow_flag;
   BOX[4]=Pix_Data_white_len;
 // BOX[4]=p_SD5_PID.left_setpoint;
 //  BOX[5]=p_SD5_PID.right_setpoint;
   //BOX[5]=line_flag*20;
  // BOX[5]=setsetpoint;
   BOX[5]=(uint8)0;
  // BOX[6]=count_R;
   BOX[6]=0;
   //BOX[6]=countflag_while_circle*10;
//   BOX[6]=line_flag;
//    BOX[7]=spd;
 //  BOX[7]=line_count;
 //  BOX[8]=circle_count;
    BOX[8]=0;
    BOX[7]=(int)0;
    
   //BOX[8]=NT;
//   BOX[8]=circleflag*10;   
     // BOX[7]=duty_L/10;
    // BOX[8]=col_longest;
//    BOX[8]=(uint8)fabs(p_SD5_PID.SetPoint-col_position);
   SendBox(BOX);                       
   DELAY_US(200);
}
  
  /*
void nrf_send(void)
{
  buff[0]=12;
  buff[1]=0x03;//首位相同即可示波器
  buff[2]=0xFC;//同上
  buff[3]=(uint8)count_A; //A轮
  buff[4]=(uint8)count_B; //B轮
  buff[5]=(uint8)count_C;  //C轮
  buff[6]=(uint8)count_D;  //D轮
  buff[7]=0;
   buff[8]=0;
   buff[9]=54;
  buff[10]=55;
  buff[11]=0xFC;
  buff[12]=0x03;
  nrf_tx(buff, DATA_PACKET);
}*/

void SendBox(int16 admin[3])
{
  uint8 b[24];
  int j=0;
  int _temp; 
  
  b[0]=23;
  b[1]=0xAA;
  b[2]=0xAA;
  b[3]=0x02;
  b[4]=18;//数据数目
  _temp = (int16)(admin[0]);//roll
  b[5]=BYTE1(_temp);
  b[6]=BYTE0(_temp);
  _temp = (int16)(admin[1]);//最终融合角度PIT
  b[7]=BYTE1(_temp);
  b[8]=BYTE0(_temp);
  _temp = (int16)(admin[2]);//Z轴输出YAW
  b[9]=BYTE1(_temp);
  b[10]=BYTE0(_temp);
  _temp = (int16)(admin[3]);//Z轴输出YAW
  b[11]=BYTE1(_temp);
  b[12]=BYTE0(_temp);
 _temp = (int16)(admin[4]);//Z轴输出YAW
  b[13]=BYTE1(_temp);
  b[14]=BYTE0(_temp);
  _temp = (int16)(admin[5]);//Z轴输出YAW
  b[15]=BYTE1(_temp);
  b[16]=BYTE0(_temp);
  _temp = (int16)(admin[6]);//Z轴输出YAW
  b[17]=BYTE1(_temp);
  b[18]=BYTE0(_temp);
  _temp = (int16)(admin[7]);//Z轴输出YAW
  b[19]=BYTE1(_temp);
  b[20]=BYTE0(_temp);
  _temp = (int16)(admin[8]);//Z轴输出YAW
  b[21]=BYTE1(_temp);
  b[22]=BYTE0(_temp);
  b[23]=0;
  for(j=1;j<23;j++)
    b[23]=b[23]+b[j];
  //uart_putbuff (UART0,b,18);//发送数组b，用无线
  nrf_tx(b,23);
}


void recieve_check()
{
     do                             //不在接收态，执行一次检查是否按下；
                                    //  在接收态，循环执行，等待接收并检查是否跳出；
     {
        if(key_check(KEY_A) == KEY_DOWN)         //等待按键 弹起
        {
            if( receive_flag == 0)
            {
                disable_irq (PIT1_IRQn);
                led(LED0,LED_ON);
               // PID_right_reinit(1,HIGH_SPEED);
                //PID_left_reinit(1,HIGH_SPEED);
                
                receive_flag++;
            }
            else if( receive_flag == 1)
            {
              
                led(LED0,LED_OFF);  
                led(LED1,LED_ON); 
                
                receive_flag=0;
            }
          /*  else if( receive_flag == 2)
            {
                //enable_irq (PIT1_IRQn);
                led(LED1,LED_OFF);  
                receive_flag++;
            }
            else if(receive_flag == 3)
            {
                led(LED1,LED_ON);
                 receive_flag++;
              
            }
            else if (receive_flag == 4)
            {  
               enable_irq (PIT1_IRQn);
                led(LED1,LED_OFF);
                 receive_flag=0;
            }
          else if (receive_flag ==5)
            {
               led(LED1,LED_ON); 
                receive_flag++;
            }
            else if (receive_flag ==6)
            {
               led(LED1,LED_OFF);
               receive_flag++;
            }
            else if (receive_flag ==7)
            {
              led(LED1,LED_ON); 
              receive_flag++;
            }
            else if (receive_flag ==8)
            {
              led(LED1,LED_OFF);
              receive_flag++;
            }
            else if (receive_flag ==9)
            {
              led(LED1,LED_ON); 
              receive_flag=0;
            }*/
        }
        while(key_check(KEY_A) == KEY_DOWN); 
         
        if( receive_flag != 0 )  
          Receive_PID();
          
     }
     while( receive_flag != 0 );
}




