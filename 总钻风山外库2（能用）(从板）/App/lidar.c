#include "lidar.h"
#include "common.h"
#include "include.h"
//#include "sys.h"
//#include "usart.h"
//#include "led.h"
uint8 lidar_buffer[5]={0};
uint16 lidar_data[3]={0};
uint16 last_lidar_data[3]={0};
uint16 _sum=0;
extern uint8 scan[2];
  uint8 data;
//先接受A5 5A 05 00 00 40 81  作为试验
/*接收A5，5A*/
void Data_Receive_Prepar()
{
	static uint8 RxBuffer[10];
	static uint8 _data_len = 0,_data_cnt = 0;
	static uint8 state = 0;
	uint8 i=0;
      
        //uart_getchar(UART5,&data);
    //    uart_getchar(UART5,&data);
	if(state==0&&data==0xA5)
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0x5A)
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2&&data<0XF1)  //质量？
	{
		state=3;
		RxBuffer[2]=data;
	}
	else if(state==3&&data<50)
	{
		state = 4;
		RxBuffer[3]=1;
		_data_len = 3;
		_data_cnt = 0;
	}
	else if(state==4&&data<0xEF)
	{
		_data_len--;
		//RxBuffer[4+_data_cnt++]=data;
                RxBuffer[4]=data;
		if(_data_len==0)
		{
			state = 5;
			//flag=1;
			printf("OK\n");
		}
	}
	else if(state==5)
	{
		state = 0;
//		flag=1;
//		USART2->DR=0x80;  
//    while((USART2->SR&0X40)==0);//循环发送,直到发送完毕 
//		RxBuffer[4+_data_cnt]=data;
//		Data_Receive_Anl(RxBuffer,_data_cnt+5);
//		for(i=0;i<7;i++)
//			{
//				_sum=RxBuffer[i]+_sum;
//			}	
	}
	else
		state = 0;
}
/*接受数据*/
void Data_Receive_()
{
	static uint8 RxBuffer[10];
	static uint8 _data_len = 0,_data_cnt = 0;
	static uint8 state = 0;
	uint8 temp=0;uint16 k=0;
        uint8 checkflag_1= 0xfd;
        uint8 checkflag_2= 0xfc;
        uint8 endflag = 0xff;
        
	temp=data>>1;
	//uart_getchar(UART5,&data);

        for(k=0;k<DMA_CNT;k++)
        {
          data=DMA_BUFF[k];
          temp=data>>1;
	if(state==0&&((temp^data)&0x01))
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&((data<<7)==0x80))//角度（同时有校验位）
	{
		state=2;
		RxBuffer[1]=data;
	}
	else if(state==2)//&&data<0XF1  //角度
	{
		state=3;
		RxBuffer[2]=data;
	}
	else if(state==3&&data>32)//距离
	{
		state = 4;
		RxBuffer[3]=data;
		_data_len = 1;
		_data_cnt = 0;
	}
	else if(state==4&&data<0x0F)
	{
		_data_len--;
		RxBuffer[4]=data;
		if(_data_len==0)
		{
			state = 5;
			memcpy(lidar_buffer,RxBuffer,5);
			//data_pro(lidar_buffer ,5);
                        uart_putchar(UART4,checkflag_1);
                        uart_putchar(UART4,checkflag_2);
                        uart_putbuff(UART4,lidar_buffer,5);
                        uart_putchar(UART4,endflag);
//			for(i=0;i<5;i++)
//			{
//				USART2->DR=RxBuffer[i];  
//				while((USART2->SR&0X40)==0);//循环发送,直到发送完毕 ;
//			}
//			printf("收到\n");
			//调用数据处理函数
		}
	}
	else if(state==5)
	{
		state = 0;
//		flag=1;
//		USART2->DR=0x80;  
//    while((USART2->SR&0X40)==0);//循环发送,直到发送完毕 
//		RxBuffer[4+_data_cnt]=data;
//		Data_Receive_Anl(RxBuffer,_data_cnt+5);
//		for(i=0;i<7;i++)
//			{
//				_sum=RxBuffer[i]+_sum;
//			}	
	}
	else
		state = 0;
        }
       
}
/*数据处理*/
void Rec_Test(uint8 data)
{
	static uint8 RxBuffer[50];
	static uint8 _data_len = 0,_data_cnt = 0;
	static uint8 state = 0;
	uint8 i=0;
	if(state==0&&data==0xA5)
	{
		state=1;
		RxBuffer[0]=data;
	}
	else if(state==1&&data==0x20)
	{
		state=2;
		RxBuffer[1]=data;
//		Usart1_Send(scan,2);
		printf("OK\n");
	}
//	else if(state==2&&data<0XF1)
//	{
//		state=3;
//		RxBuffer[2]=data;
//	}
//	else if(state==3&&data<50)
//	{
//		state = 4;
//		RxBuffer[3]=data;
//		_data_len = 3;
//		_data_cnt = 0;
//	}
//	else if(state==4&&_data_len>0)
//	{
//		_data_len--;
//		RxBuffer[4+_data_cnt++]=data;
//		if(_data_len==0)
//			state = 5;
//	}
//	else if(state==5)
//	{
//		state = 0;
////		RxBuffer[4+_data_cnt]=data;
////		Data_Receive_Anl(RxBuffer,_data_cnt+5);
//		for(i=0;i<7;i++)
//			{
//				_sum=RxBuffer[i]+_sum;
//			}	
//	}
	else
		state = 0;
}
/*数据处理*/
void data_pro(uint8 *data,uint8 num)
{
	uint16 dis=0,ang=0;
	ang=*(data+2);
	lidar_data[0]=(uint16)(((ang<<8)|*(data+1))>>1)/64.0;//角度数据
        
	dis=*(data+4);
	lidar_data[1]=(uint16)(dis<<8|*(data+3))/4.0;
        
     //   printf("角度=%ld",(uint16)lidar_data[0]);
    //    printf("长度=%ld",(uint16)lidar_data[1]);
        printf("\n");
        last_lidar_data[0] = lidar_data[0];
        last_lidar_data[1] = lidar_data[1];
     
        
}