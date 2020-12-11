#include "common.h"
#include "include.h"
#include "PID.h"
#include "Send.h"
#include "analysis.h"
#include "motor.h"
#include "init.h"
#include "Encoder1.h"
#include "circleflag.h"
#include "analysis.h"
#include "LCD_menu.h"
uint8 imgbuff[CAMERA_SIZE]; 
uint64 t_process,t_into;
        uint8 a = 0xfe;
	uint8 b = 0xfa;
	uint8 c = 0xff;
        uint8 a1 = 0xfd;
        uint8 b1 = 0xfb;
        uint8 c1 = 0xfc;
         uint8 TOF_left_byteReceived;
         uint8 TOF_right_byteReceived;
 int16 barrier_dis_left;
 int16 barrier_dis_right;
 uint8 barrier_flag = 0;
uint8 barrier_right_flag = 0;
void send_barrier_count()
{
  static uint8 data_barrier = 0;
  static uint8 send = 0x00;
  static uint8 last_send = 0x00;
  if(barrier_dis_left <1000  )
  {
    send =  0x01;
  }
  else if(barrier_dis_right <1000)
  {
    send = 0x02;
    
  }
  else
  {
    send = 0x00;
  }
    data_barrier = send;
    uart_putchar(UART1,data_barrier);
    last_send = send ;
}
void send_count()
{
	static uint8 data_buff[7]={0};
	static uint8 LF_high = 0x00;
	static uint8 LF_low = 0x00;
	static uint8 LB_high = 0x00;
	static uint8 LB_low = 0x00;
      
	
        LF_low = count_LF & 0x00ff;
	LF_high = count_LF >> 8;
	
        LB_low = count_LB & 0x00ff;
	LB_high = count_LB1 >> 8;
      
        data_buff[0] = LF_high;
	data_buff[1] = LF_low;
	data_buff[2] = LB_high;
	data_buff[3] = LB_low;
      
     
          uart_putchar(UART2,a);//发送开始标志位
	  uart_putchar(UART2,b);//发送标志位 
          uart_putbuff(UART2, data_buff, 4);//发送一帧数据
          uart_putchar(UART2,c);//发送结束标   志位
	for (unsigned int i = 0; i < strlen(data_buff); i++)
			data_buff[i] = '\0' ; //for循环清空数组
       
	
     
}

void send_Photo_data()
{
  if(open_or_not ==1)
  {
        static uint8 data_camrea_buff[6]={0};

        static uint8 Barrier_dis_low = 0x00;
        static uint8 Barrier_dis_high = 0x00;
        static uint8 last_col_longest_send =  0;
        static uint8 col_position_send = 0;
        static uint8 fakeflag_send = 0 ;
        static uint8 NT_send = 0 ;
        
        last_col_longest_send =  last_col_longest;
        col_position_send = col_position;
        fakeflag_send  = fakeflag ;
        NT_send = NT;
        data_camrea_buff[0] = last_col_longest_send;
        data_camrea_buff[1] = col_position_send;
        data_camrea_buff[2] = fakeflag_send;
        data_camrea_buff[3] = NT_send;
     
          uart_putchar(UART5,a1);//发送开始标志位
	  uart_putchar(UART5,b1);//发送标志位 
          uart_putbuff(UART5, data_camrea_buff, 4);//发送一帧数据
          uart_putchar(UART5,c1);//发送结束标   志位
	for (unsigned int i = 0; i < strlen(data_camrea_buff); i++)
			data_camrea_buff[i] = '\0' ; //for循环清空数组
  }
       
}
  
void UART0_IRQhandler()//预留口
{
        static uint8_t rec_TOF_right[7]={0};
	static uint8_t rec_TOF_right_state = 0;
	static uint8_t  data_TOF_right_cnt = 0;                  
	uart_getchar(UART0,&TOF_right_byteReceived);
	if(TOF_right_byteReceived == 0x0D &&rec_TOF_right_state==0)                              
	{
		rec_TOF_right_state = 1;
		rec_TOF_right[0] = TOF_right_byteReceived;
	}			
	else if(TOF_right_byteReceived == 0x0A && rec_TOF_right_state==1)													
	{
		rec_TOF_right_state=2;
		rec_TOF_right[1] = TOF_right_byteReceived;
	}
	else if(rec_TOF_right_state == 2 )
	{
          if(TOF_right_byteReceived!=0x6D)
          {
		rec_TOF_right[2+data_TOF_right_cnt++] = TOF_right_byteReceived;
          }
          else
          {
            rec_TOF_right_state =3;
          }
                
		
	}
	else if(rec_TOF_right_state == 3)
	{
               
		if(TOF_right_byteReceived == 0x6D)  																			
		{
                            
                        if(data_TOF_right_cnt==1)
                        {
                          rec_TOF_right[2]=rec_TOF_right[2]-0x30;
                          barrier_dis_right = rec_TOF_right[2] ;
                          //barrier_anas();
                         //  send_barrier_count();
                        }
                        else if(data_TOF_right_cnt==2)
                        {
                          rec_TOF_right[2]=rec_TOF_right[2]-0x30;
                          rec_TOF_right[3]=rec_TOF_right[3]-0x30;
                           barrier_dis_right = rec_TOF_right[2] * 10 + rec_TOF_right[3]*1;
                          // barrier_anas();
                          //  send_barrier_count();
                        }
                        else if (data_TOF_right_cnt==3)
                        {
                          rec_TOF_right[2]=rec_TOF_right[2]-0x30;
                          rec_TOF_right[3]=rec_TOF_right[3]-0x30;
                          rec_TOF_right[4]=rec_TOF_right[4]-0x30;
                           barrier_dis_right = rec_TOF_right[2] * 100 + rec_TOF_right[3]*10 + rec_TOF_right[4]*1;
                          // barrier_anas();
                          //  send_barrier_count();
                        }
                        else if(data_TOF_right_cnt==4)
                        {
                           rec_TOF_right[2]=rec_TOF_right[2]-0x30;
                          rec_TOF_right[3]=rec_TOF_right[3]-0x30;
                          rec_TOF_right[4]=rec_TOF_right[4]-0x30;
                          rec_TOF_right[5]=rec_TOF_right[5]-0x30;
                           barrier_dis_right = rec_TOF_right[2] * 1000 + rec_TOF_right[3]*100 + rec_TOF_right[4]*10+rec_TOF_right[5]; 
                             //    send_barrier_count();
                        }
                        send_barrier_count();
			rec_TOF_right_state = 0;                                          
			data_TOF_right_cnt = 0;
			return;
		}
		else    
		{
                         
                          barrier_right_flag  = 0;
			data_TOF_right_cnt = 0;																						 
			rec_TOF_right_state = 0;
			return;
		}

	}
	else
	{
                barrier_right_flag = 0;
		rec_TOF_right_state = 0;                                                 
		data_TOF_right_cnt = 0;
        }
  
}

void UART4_IRQhandler()//预留口
{
        static uint8_t rec_TOF_left[7]={0};
	static uint8_t rec_TOF_left_state = 0;
	static uint8_t  data_TOF_left_cnt = 0;                  
	uart_getchar(UART4,&TOF_left_byteReceived);
	if(TOF_left_byteReceived == 0x0D &&rec_TOF_left_state==0)                              
	{
		rec_TOF_left_state = 1;
		rec_TOF_left[0] = TOF_left_byteReceived;
	}			
	else if(TOF_left_byteReceived == 0x0A && rec_TOF_left_state==1)													
	{
		rec_TOF_left_state=2;
		rec_TOF_left[1] = TOF_left_byteReceived;
	}
	else if(rec_TOF_left_state == 2 )
	{
          if(TOF_left_byteReceived!=0x6D)
          {
		rec_TOF_left[2+data_TOF_left_cnt++] = TOF_left_byteReceived;
          }
          else
          {
            rec_TOF_left_state =3;
          }
                
		
	}
	else if(rec_TOF_left_state == 3)
	{
               
		if(TOF_left_byteReceived == 0x6D)  																			
		{
                            
                        if(data_TOF_left_cnt==1)
                        {
                          rec_TOF_left[2]=rec_TOF_left[2]-0x30;
                          barrier_dis_left = rec_TOF_left[2] ;
                          //barrier_anas();
                         //  send_barrier_count();
                        }
                        else if(data_TOF_left_cnt==2)
                        {
                          rec_TOF_left[2]=rec_TOF_left[2]-0x30;
                          rec_TOF_left[3]=rec_TOF_left[3]-0x30;
                           barrier_dis_left = rec_TOF_left[2] * 10 + rec_TOF_left[3]*1;
                          // barrier_anas();
                          //  send_barrier_count();
                        }
                        else if (data_TOF_left_cnt==3)
                        {
                          rec_TOF_left[2]=rec_TOF_left[2]-0x30;
                          rec_TOF_left[3]=rec_TOF_left[3]-0x30;
                          rec_TOF_left[4]=rec_TOF_left[4]-0x30;
                           barrier_dis_left = rec_TOF_left[2] * 100 + rec_TOF_left[3]*10 + rec_TOF_left[4]*1;
                          // barrier_anas();
                          //  send_barrier_count();
                        }
                        else if(data_TOF_left_cnt==4)
                        {
                           rec_TOF_left[2]=rec_TOF_left[2]-0x30;
                          rec_TOF_left[3]=rec_TOF_left[3]-0x30;
                          rec_TOF_left[4]=rec_TOF_left[4]-0x30;
                          rec_TOF_left[5]=rec_TOF_left[5]-0x30;
                           barrier_dis_left = rec_TOF_left[2] * 1000 + rec_TOF_left[3]*100 + rec_TOF_left[4]*10+rec_TOF_left[5]; 
                             //    send_barrier_count();
                        }
                         send_barrier_count();
			rec_TOF_left_state = 0;                                          
			data_TOF_left_cnt = 0;
			return;
		}
		else    
		{
                         
                          barrier_flag  = 0;
			data_TOF_left_cnt = 0;																						 
			rec_TOF_left_state = 0;
			return;
		}

	}
	else
	{
                barrier_flag = 0;
		rec_TOF_left_state = 0;                                                 
		data_TOF_left_cnt = 0;
        }
  
}
void PIT1_IRQHandler()  //还需更改
{
	PIT_Flag_Clear(PIT1);
	count_get();
        send_count();
        //receive_count();
      //含PID（）;      
}


/*void UART2_IRQhandler()
{  
                led(LED0,LED_ON);
               // receive_count();

}*/

void PORTE_IRQHandler()
{
	uint8 n;  //引脚号
	uint32 flag;

	flag = PORTE_ISFR;
	PORTE_ISFR = ~0;  //清中断标志位

	n = 27;
	if (flag& (1 << n))
	{
		nrf_handler();
	}
}


void PORTA_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
      VSYNC();
      
    }
}

void DMA0_IRQHandler()
{
    DMA_IRQ_CLEAN(DMA_CH0);
    row_finished();
}

void DMA10_IRQHandler(void)
{
  
  t_into = pit_time_get_ms(PIT2);
  pit_time_start(PIT0); 
  led_turn(LED1);
  Data_Receive_();
    DMA_IRQ_CLEAN(DMA_CH10);
  t_process=pit_time_get_ms(PIT0);
  pit_time_start(PIT2);
  
}

void DMA_Error_IRQHandler(void)
{
  uint32 error = DMA_ES_REG(DMA_BASE_PTR);
  led_turn(LED1);
}


void PIT0_IRQHandler()
{
  
   PIT_Flag_Clear(PIT0);
}

