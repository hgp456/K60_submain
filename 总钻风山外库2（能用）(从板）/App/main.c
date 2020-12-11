





    
        
		
#include "common.h"
#include "include.h"
#include "PID.h"
#include "Send.h"
#include "analysis.h"
#include "motor.h"
#include "init.h"
#include "Encoder1.h"
#include "circleflag.h"
#include "OTSU.h"
#include "analayse.h"
uint8 imgthreshold;
uint8 imgthreshold_1=0;
uint8 image_1[ROW][COL];

int barrier_Y = 0;
int Pix_Data_white_len=0;
int Pix_Data_white_right_len = 0;
int Pix_Data_white_left_len = 0;
uint8  Vx2 = 0;
uint8   Vx3 = 0;
uint8 send = 0x00;
   uint8 last_send = 0x00;
/*void find_edge()
{
  int i,j;
  for(i=0;i<ROW;i++)
  {
    for(j=0;j<COL;j++)
    {
      if(image[i][j]==255&&image[i+1][j]==0)
      {
        image_1[i][j]= 255;
      }
      else
      {
        image_1[i][j] = 0;
      }
      if(image[i][j]==255&&)
    }
  }
}*/

void uart5_init()
{

       uart_init(UART5,9600);
      // set_vector_handler(UART5_RX_TX_VECTORn,UART5_IRQhandler);  // 设置中断服务函数到中断向量表里 ，假如 VCAN_PORT == UART3 ，那么 UART0_RX_TX_VECTORn + VCAN_PORT*2 = UART3_RX_TX_VECTORn
        uart_rx_irq_en(UART5);
         enable_irq(UART5_RX_TX_IRQn);  //key？

      
}
void  main(void)
{
  //int i;
   // uint8 *p;
  init_all();

    nrf_init();
    set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);
    enable_irq(PORTE_IRQn);
  //uart_init(UART5,9600);
   
  for(;;)
    {
       if(mt9v032_finish_flag)
        {
          
          mt9v032_finish_flag=0;

          Photoanly();
          send_Photo_data();

         // nrf_send();
          
        }
    }
}