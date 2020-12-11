#include "analysis.h"
#include "PID.h"
#include "include.h"
#include "common.h"
#include "circleflag.h"
//     设定值（Aim_Speed）                      A B C D   这个地方设定的时候可以做大十倍 然后再最后设定时候除以十；

int modeflag = 0; // 0 直线 1右转弯 2左转弯 3 4 5 6 7
#define COL_MID 94
unsigned char nmr1=0;
//暂未考虑闭环
/*void motor_judgement()
{
	if (row_position >= LONG_DISTANCE_EDGE && row_position != 120)  //设为加速mode
	{
	  //冲冲冲
	}
}*/


	
		

void motor_circle_judgement()
{
	if (modeflag == 0)
	{
		// McNamm_wheel_control(-0.5,20,0);
               // Vx = 20;
                 McNamm_wheel_control(0,20,0);
	}
	else if (modeflag == 1) //右转弯
	{
          
          
         
         McNamm_wheel_control(0,20,50);
		/*for (nmr1 = 0; nmr1 < 4; nmr1++)
		{
			Aim_Speed[nmr1] = Circle_Right_Duty[nmr1]; //将向右转所需参数打入 可能需要更新 暂未完成 搭框架
		}
                pid();//有一个问题 这样Aim_Speed[nmr] 的值是否会更改 我觉得会*/
	} 
	else if (modeflag == 2 ) //右转弯 
	{
           McNamm_wheel_control(0,20,30);
          
          
          
		/*for (nmr1 = 0; nmr1 < 4; nmr1++)
		{
			Aim_Speed[nmr1] = Circle_Left_Duty[nmr1];  //将向右转所需参数打入 可能需要更新 暂未完成 搭框架
		}
                 pid();*/
	}
	else if (modeflag == 3 )//左偏移
	{
              McNamm_wheel_control(0,20,-30);
		/*for (nmr1 = 0; nmr1 < 4; nmr1++)
		{
			Aim_Speed[nmr1] = Line_Left_Duty[nmr1];  
                }
                 pid();*/
	}
	else if (modeflag == 4 )//右偏移
          {
            McNamm_wheel_control(20,20,0);
		/*for (nmr1 = 0; nmr1 < 4; nmr1++)
		{
			Aim_Speed[nmr1] = Line_Right_Duty[nmr1];
		}
                 pid();*/
          }
	else if (modeflag == 5) //倒车
        {
          McNamm_wheel_control(0,-20,0);
		/*for (nmr1 = 0; nmr1 < 4; nmr1++)
		{
			Aim_Speed[nmr1] = Line_Right_Duty[nmr1];
		}
                 pid();*/
        }
	else if (modeflag == 6)
          {//倒车
            McNamm_wheel_control(0,-20,0);
		/*for (nmr1 = 0; nmr1 < 4; nmr1++)
		{
			Aim_Speed[nmr1] = Left_Sidesway[nmr1];
		}
                 pid();*/
          }
	//else if modeflag = 7
		//for (nmr = 0; nmr<4; nmr++)

}