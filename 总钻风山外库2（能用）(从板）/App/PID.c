#include "PID.h"
#include "constant.h"
#include "math.h"
#include "Encoder1.h"
#include "common.h"
#include "include.h"
#include "Send.h"
#include "circleflag.h"

float   PID_Threshold = 0; //积分分离

int Aim_Speed[4];
int Last_Encoder[4];
int Encoder[4];
PID  PID_Motor[4];
unsigned char nmr = 0;
float  K = 0.8;
int  Ki_switch = 0;
float stepIn = (maximum - minimum)*0.1 + minimum;  //步进值 仍需计算 未调整
int kFactor = 0;//用于判断步进值的加减
float gama;  //微分先行 滤波系数
/*typedef struct
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

	float  Gama;  //微分先行
	float  c1;
	float  c2;
	float  c3;
	float  temp;


}PID;*/



void pid()
{
        pid_set();

	for (nmr = 0; nmr < 4; nmr++)
	{
		PID_Motor[nmr].err = Aim_Speed[nmr] - Encoder[nmr];  //我认为是等于 


		if ( PID_Motor[nmr].err > PID_Threshold)   //积分分离  这个值需要实际确定
		{
			Ki_switch = 0;
		}
		else
		{
			Ki_switch = 1;
		}

		PID_Motor[nmr].err = PID_Motor[nmr].err - PID_Motor[nmr].err_last_1;
		PID_Motor[nmr].ierr = PID_Motor[nmr].err; 
		PID_Motor[nmr].derr = PID_Motor[nmr].err - 2*PID_Motor[nmr].err_last_1 + PID_Motor[nmr].err_last_2;  //换上增量式 但是现在不知道往年程序中为什么除以7.5

		PID_Motor[nmr].temp = gama * PID_Motor[nmr].Kd + PID_Motor[nmr].Kp;
		PID_Motor[nmr].c3 = PID_Motor[nmr].Kd / PID_Motor[nmr].temp;
		PID_Motor[nmr].c2 = (PID_Motor[nmr].Kd + PID_Motor[nmr].Kp) / PID_Motor[nmr].temp; //（kd+kp）/temp
		PID_Motor[nmr].c1 = gama * PID_Motor[nmr].c3;
		
		PID_Motor[nmr].Pout = PID_Motor[nmr].Kp * PID_Motor[nmr].err;
		//输出I
		PID_Motor[nmr].Iout = PID_Motor[nmr].Ki * PID_Motor[nmr].ierr*Ki_switch; //积分分离

		PID_Motor[nmr].Iout = Get_MxMi(PID_Motor[nmr].Iout, 200, -200);// //限波 这个值也需要改
		//输出D
		PID_Motor[nmr].Dout = PID_Motor[nmr].c1 * PID_Motor[nmr].Dout + PID_Motor[nmr].c2 * Encoder[nmr] + PID_Motor[nmr].c3 * Last_Encoder[nmr]; //微分先行设定值
		//PID_Motor[nmr].Dout = PID_Motor[nmr].Kd *PID_Motor[nmr].derr; 这个为原本的Dout 
		//更新误差
		PID_Motor[nmr].err_last_1 = PID_Motor[nmr].err;
		PID_Motor[nmr].err_last_2 = PID_Motor[nmr].err_last_1;
		//输出
		PID_Motor[nmr].OUT = PID_Motor[nmr].Pout + PID_Motor[nmr].Iout + PID_Motor[nmr].Dout;

		PID_Motor[nmr].OUT = Get_MxMi(PID_Motor[nmr].OUT, 4599.0, -4599.0); //限波 这个值也需要改
		
		Last_Encoder[nmr] = Encoder[nmr];//微分先行
                         
	}
         Set_Motor(PID_Motor[0].OUT,PID_Motor[1].OUT,PID_Motor[2].OUT,PID_Motor[3].OUT);//更改PID参数值
}

void Set_Motor(float PID_Motor_0, float PID_Motor_1, float PID_Motor_2,float  PID_Motor_3) //或者直接写在里面
{
		ftm_pwm_duty(MOTOR_FTM, MOTOR3_PWM,PID_Motor_0);   // A
		ftm_pwm_duty(NEW_MOTOR_FTM, NEW_MOTOR1_PWM,PID_Motor_1); //B
		ftm_pwm_duty(MOTOR_FTM, MOTOR2_PWM,PID_Motor_2); //C
		ftm_pwm_duty(NEW_MOTOR_FTM, MOTOR4_PWM,PID_Motor_3); //D
}
	//Set_Motor(float (PID_Motor[0].out),float (PID_Motor[1].out), float (PID_Motor[2].out),float (PID_Motor[3].OUT));
	//Set_Motor(1500,1500,1500,1500);
	//printf("Set spedd successfullly!");


float StepInProcessing(float input_duty_you_want, float sp) //步进值   实际有可能不选 或者和微分先行换着用  因为从图像判断速度增幅的敏锐性有影响
{
	float duty_you_want;
	float what_you_want;
	duty_you_want = input_duty_you_want;
	for (nmr = 0; nmr < 4; nmr++)
	{
		if (fabs(Aim_Speed[nmr] - duty_you_want) <= stepIn)
		{
			Aim_Speed[nmr] = (int)duty_you_want;
		}
		else
		{
			if (Aim_Speed[nmr] - duty_you_want > 0)
			{
				kFactor = 1;
			}

			else if (Aim_Speed[nmr] - duty_you_want < 0)
			{
				kFactor = -1;
			}
			else
			{
				kFactor = 0;
			}
			Aim_Speed[nmr] = (int)duty_you_want + kFactor * (int)stepIn;//这里整数还是浮点需要再确认一下
			what_you_want = Aim_Speed[nmr];
		}
	}
	return  what_you_want;
}

void Receive_PID()
{
   uint8 relen;
   relen = nrf_rx(buff,DATA_PACKET);               //等待接收一个数据包，数据存储在buff里
        if(relen != 0)
        {   if( buff[1]=='?')
                ;
            else
            {
              if( receive_flag == 1 )    
              {
                PID_Motor[0].Kp=buff[1]/16*1000 + buff[1]%16*100 + buff[2]/16*10+buff[2]%16 ;
                PID_Motor[0].Ki=buff[3]/16*1000 + buff[3]%16*100 + buff[4]/16*10+buff[4]%16 ;
                PID_Motor[0].Kd=buff[5]/16*1000 + buff[5]%16*100 + buff[6]/16*10+buff[6]%16 ;
                
                
              }
              
            /*  else if( receive_flag == 2 ) 
              {
                PID_Motor[1].Kp=buff[1]/16*1000 + buff[1]%16*100 + buff[2]/16*10+buff[2]%16 ;
                PID_Motor[1].Ki=buff[3]/16*1000 + buff[3]%16*100 + buff[4]/16*10+buff[4]%16 ;
                PID_Motor[1].Kd=buff[5]/16*1000 + buff[5]%16*100 + buff[6]/16*10+buff[6]%16 ;
              }
              
              else if(receive_flag ==3)
              {
                PID_Motor[2].Kp=buff[1]/16*1000 + buff[1]%16*100 + buff[2]/16*10+buff[2]%16 ;
                PID_Motor[2].Ki=buff[3]/16*1000 + buff[3]%16*100 + buff[4]/16*10+buff[4]%16 ;
                PID_Motor[2].Kd=buff[5]/16*1000 + buff[5]%16*100 + buff[6]/16*10+buff[6]%16 ;
              }
              else if(receive_flag ==4)
              {
                PID_Motor[3].Kp=buff[1]/16*1000 + buff[1]%16*100 + buff[2]/16*10+buff[2]%16 ;
                PID_Motor[3].Ki=buff[3]/16*1000 + buff[3]%16*100 + buff[4]/16*10+buff[4]%16 ;
                PID_Motor[3].Kd=buff[5]/16*1000 + buff[5]%16*100 + buff[6]/16*10+buff[6]%16 ;
              }*/
             // else if(receive_flag ==5)
          //    {
          //      Circle_Right_Duty[0]==buff[1]/16*1000 + buff[1]%16*100 + buff[2]/16*10+buff[2]%16 ;
          //      Circle_Right_Duty[1]==buff[3]/16*1000 + buff[3]%16*100 + buff[4]/16*10+buff[4]%16 ;
           //     Circle_Right_Duty[2]==buff[5]/16*1000 + buff[5]%16*100 + buff[6]/16*10+buff[6]%16 ;
           //     Circle_Right_Duty[3]==buff[7]/16*1000 + buff[7]%16*100 + buff[8]/16*10+buff[8]%16 ;
            //    //Aim_Speed[nmr] = [];
               
              //}
                
              
            }
            if( receive_flag == 1 )
              sprintf((char *)buff,"\x1e p=%ld,i=%ld,d=%ld A轮\n",(int16)PID_Motor[0].Kp,(int16)PID_Motor[0].Ki,(int16)PID_Motor[0].Kd); //把str和i合并成一个字符串到buff里，再进行发送
            else if( receive_flag == 2 ) 
              sprintf((char *)buff,"\x1e p=%ld,i=%ld,d=%ld B轮\n",(int16)PID_Motor[1].Kp,(int16)PID_Motor[1].Ki,(int16)PID_Motor[1].Kd);
            else if( receive_flag == 3)
              sprintf((char *)buff,"\x1e p=%ld,i=%ld,d=%ld C轮\n",(int16)PID_Motor[2].Kp,(int16)PID_Motor[2].Ki,(int16)PID_Motor[2].Kd);
            else if (receive_flag ==4)
              sprintf((char *)buff,"\x1e p=%ld,i=%ld,d=%ld D轮\n",(int16)PID_Motor[3].Kp,(int16)PID_Motor[3].Ki,(int16)PID_Motor[3].Kd);
          //  else if (receive_flag ==5)
         //     sprintf((char *)buff,"\x1e 右转 A轮=%ld,B轮=%ld,C轮=%ld D轮=%ld\n",(int16)Circle_Right_Duty[0],(int16)Circle_Right_Duty[1],(int16)Circle_Right_Duty[2],Circle_Right_Duty[3]);
               
            nrf_tx(buff,DATA_PACKET);        
        }

}



float Get_MxMi(float num,float max,float min)
{
	if(num > max)
		return max;
	else if(num < min)
		return min;
	else
		return num;
}

void pid_set()  //一组参数
{
    Encoder[0] = count_A;  //赋值
    Encoder[1] = count_B;
    Encoder[2] = count_C;
    Encoder[3] = count_D;
    
    PID_Motor[1].Kp =  PID_Motor[0].Kp;
    PID_Motor[2].Kp =  PID_Motor[0].Kp;
    PID_Motor[3].Kp =  PID_Motor[0].Kp;  //先用一组参数
    
    PID_Motor[1].Ki = PID_Motor[0].Ki;
    PID_Motor[2].Ki = PID_Motor[0].Ki;
    PID_Motor[3].Ki = PID_Motor[0].Ki;
    
    PID_Motor[1].Kd = PID_Motor[0].Kd;
    PID_Motor[2].Kd = PID_Motor[0].Kd;
    PID_Motor[3].Kd = PID_Motor[0].Kd;
    
    gama =  0; //微分先行 滤波系数
    PID_Threshold = 0;  //积分分离
     
    //motion_check();
    
    
    
    
}

//微分先行参考 已进入主程序
/*定义结构体和公用体*/
/*
typedef struct

{

	float setpoint;       //设定值

	float proportiongain;     //比例系数

	float integralgain;      //积分系数

	float derivativegain;    //微分系数

	float lasterror;     //前一拍偏差

	float result;     //输出值

	float integral;   //积分值

	float derivative;      //微分项

	float lastPv;     //前一拍的测量值

	float gama;      //微分先行滤波系数

}PID;

void PIDRegulation(PID *vPID, float processValue)

{

  float thisError;


    
  thisError=vPID->Aim_Speed[nmr]-Encoder[nmr];  //更改后的误差 我认为Process的value 为测量值
  temp= gama   * PID_Motor[nmr].Kd  + PID_Motor[nmr].Kp;

  c3= PID_Motor[nmr].Kd/temp; // kd /temp

  c2=(PID_Motor[nmr].Kd+ PID_Motor[nmr].Kp)/temp; //（kd+kp）/temp

  c1= gama*c3;//gama*kd/temp

  PID_Motor[nmr].Dout =c1* PID_Motor[nmr].Dout +c2* processValue+c3* vPID-> Last_Encoder[nmr];

 

vPID->result=vPID->PID_Motor[nmr].Kp*thisError+vPID->integralgain*vPID->integral+vPID-> derivative;

  vPID->lasterror=thisError;


}
*/
