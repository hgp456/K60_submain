/*

 Send.c
 Created on 2016.3.12
 By Yuxin Zheng

*/
#include "common.h"
#include "include.h"
#include "LCD_menu.h"
#include "constant.h"
#include "analysis.h"
#include "init.h"       

  Site_t site = {0,0};
    Site_t site1 = {0,0};
    Site_t site2 = {0,70};
    Size_t imgsize  = {ROW, COL};             //图像大小
    Size_t size;                   //显示区域图像大
      //size.H =120;
      //size.W =160;
uint8 VAR[3]={20,50,70};
uint8 open_or_not = 0;
uint8 col_mid_back = 70;
   
     
     //                                               65 40
      //                                      <20 20-30 30-40 40-50 50-60 60-70 70-80 80-90 90-100 >100
     //                                 0     1     2     3     4      5     6     7     8     9     10   11 //11用于滞回判断中rowarray[countflag+1]
    // float  row_edge_array[12]=   {   86.5 ,85.0 ,72.0 , 63.5 ,53.0 ,49.5 ,44.0, 42.5, 40.5, 30.0 ,29.0 ,29.0};
     uint8 speed_array[12]    =    {     0 ,  15  , 25 ,   35  ,  45  , 55  , 65  , 75  , 85  , 95   ,105  , 115 };  
     float  row_edge_array[12]=   {    86.5 ,85.5 ,RE2 , 68.2 , RE4  , 53.0 ,RE6 , 47.0, RE8 , 42.0 ,42.0,42.0};
     int16 chasu_high_array[11]=   {    0,   0,     0,    0,     30,    30,   30,   30,   35,   75,    75 };
     int16  chasu_low_array[11]=   {     0,   0,     0 ,   0,    10,    10,   10,   10,   10,   30,   30 };
     int16 chasu_highduty_array[11]={    750,  750,  750,  750,  750,  750,  430,  450,  350,  350,  400 };
     int16  chasu_lowduty_array[11]={    950,  950,  950,  950,  970,  950,  930,  900,  900,  900,  800 };
     float row_delta[12]=           {   0,    0,    0,     0,    5,   6.5,  7.5,   7.5  ,  7.5,  7.5 ,  0,   0};



void menu_display()
{
  
  Site_t site1; 
  
  site1.x = 80;
  site1.y = 10;
  
  LCD_str(site1, "TH", BLUE, RED);
  
  site1.x = 110;
  site1.y = 10;
  
  LCD_num(site1, threshold, BLUE, RED);
  
  site1.x = 110;
  site1.y = 30;
  
  LCD_num(site1, row_cut, BLUE, RED);
  
  site1.x = 80;
  site1.y = 30;
  
  LCD_str(site1, "RC", BLUE, RED);
  
  site1.x = 110;
  site1.y = 50;
  
  LCD_num(site1,open_or_not , BLUE, RED);
  
  site1.x = 80;
  site1.y = 50;
  
  LCD_str(site1, "SW", BLUE, RED);
  
  
  
  
  
  
  site1.x = 40;
  site1.y = 10;
  
  LCD_num(site1, col_mid_back, BLUE, RED);
  
  site1.x = 10;
  site1.y = 10;
  
  LCD_str(site1, "CM", BLUE, RED);
  
  
  
  
  
  site1.x = 40;
  site1.y = 30;
  
  LCD_num(site1, row_edge_array[4]*10, BLUE, RED);
  
  site1.x = 10;
  site1.y = 30;
  
  LCD_str(site1, "RE4", BLUE, RED);
  
  
  
  
  
  site1.x = 40;
  site1.y = 50;
  
  LCD_num(site1, row_edge_array[6]*10, BLUE, RED);
  
  site1.x = 10;
  site1.y = 50;
  
  LCD_str(site1, "RE6", BLUE, RED);
  
  
  
  
  site1.x = 40;
  site1.y = 70;
  
  LCD_num(site1, row_edge_array[8]*10, BLUE, RED);
  
  site1.x = 10;
  site1.y = 70;
  
  LCD_str(site1, "RE8", BLUE, RED);
    
  
}

void set_prameter()
{

  static uint8 flag=1;
  
  if(down==0)              
   {
     DELAY_MS(100);
     while(down==0);
     
     flag++;
     if ( flag == 8)
       flag = 1;
 
   }
    if(up==0)              
   {
     DELAY_MS(100);
     while(up==0);
     
     flag--;
     if ( flag == 0)
       flag = 7;
   }
  
   if(right==0)              
   {
      DELAY_MS(100);
      while(right==0);
      
      switch (flag)
      {
        case 1:  menu_display();
                 threshold++;
                 VAR[0]=threshold;
             //   while(!flash_write_buf(SECTOR_NUM,0,sizeof(VAR),VAR));        //扇区127,偏移地址为0,写入数据源地址:buff,数目sizeof(buff)
                 
                 site1.x = 110;
                 site1.y = 10;     
                 LCD_num(site1, threshold, BLUE, RED);
                 break;
                   
        case 2:  menu_display();
                 row_cut++;
                // VAR[1]= row_edge;
             //  while(!flash_write_buf(SECTOR_NUM,0,sizeof(VAR),VAR));        //扇区127,偏移地址为0,写入数据源地址:buff,数目sizeof(buff)
                 site1.x = 110;
                 site1.y = 30;     
                 LCD_num(site1, row_cut,BLUE, RED);
                 break;
                   
        case 3: menu_display();
                 open_or_not++;
             //  while(!flash_write_buf(SECTOR_NUM,0,sizeof(VAR),VAR));        //扇区127,偏移地址为0,写入数据源地址:buff,数目sizeof(buff)
                 site1.x = 110;
                 site1.y = 50;     
                 LCD_num(site1, open_or_not,BLUE, RED);
                 break;   
                
        case 4: menu_display();
                col_mid_back++;
             // while(!flash_write_buf(SECTOR_NUM,0,sizeof(VAR),VAR));        //扇区127,偏移地址为0,写入数据源地址:buff,数目sizeof(buff)
                site1.x = 40;
                site1.y = 10;     
                LCD_num(site1, col_mid_back,BLUE, RED);
                break;   
                 
        case 5: menu_display();
                row_edge_array[4]+=0.5;
             // while(!flash_write_buf(SECTOR_NUM,0,sizeof(VAR),VAR));        //扇区127,偏移地址为0,写入数据源地址:buff,数目sizeof(buff)
                site1.x = 40;
                site1.y = 30;     
                LCD_num(site1,row_edge_array[4]*10,BLUE, RED);
                break;                  
                 
        case 6: menu_display();
                row_edge_array[6]+=0.5;
             // while(!flash_write_buf(SECTOR_NUM,0,sizeof(VAR),VAR));        //扇区127,偏移地址为0,写入数据源地址:buff,数目sizeof(buff)
                site1.x = 40;
                site1.y = 50;     
                LCD_num(site1,row_edge_array[6]*10,BLUE, RED);
                break; 
        
        case 7: menu_display();
                row_edge_array[8]+=0.5;
             // while(!flash_write_buf(SECTOR_NUM,0,sizeof(VAR),VAR));        //扇区127,偏移地址为0,写入数据源地址:buff,数目sizeof(buff)
                site1.x = 40;
                site1.y = 70;     
                LCD_num(site1,row_edge_array[8]*10,BLUE, RED);
                break;         
        
     
        
      }
   }
  
  if(left==0)              
   {
      DELAY_MS(100);
      while(left==0);
      
      switch (flag)
      {
        case 1: menu_display();
                 threshold--;
                 VAR[0]=threshold;
            //   while(!flash_write_buf(SECTOR_NUM,0,sizeof(VAR),VAR));        //扇区127,偏移地址为0,写入数据源地址:buff,数目sizeof(buff)
                 site1.x = 110;
                 site1.y = 10;     
                 LCD_num(site1,threshold,BLUE, RED);
                 break;
                   
        case 2: menu_display();
                 row_cut--;
              //VAR[1]=row_edge;
           //    while(!flash_write_buf(SECTOR_NUM,0,sizeof(VAR),VAR));        //扇区127,偏移地址为0,写入数据源地址:buff,数目sizeof(buff)
                 site1.x = 110;
                 site1.y = 30;     
                 LCD_num(site1, row_cut,BLUE, RED);
                 break;
                   
        case 3: menu_display();
                 open_or_not--;
          //   while(!flash_write_buf(SECTOR_NUM,0,sizeof(VAR),VAR));        //扇区127,偏移地址为0,写入数据源地址:buff,数目sizeof(buff)   
                 site1.x = 110;
                 site1.y = 50;     
                 LCD_num(site1, open_or_not,BLUE, RED);
                 break;    
                 
        case 4: menu_display();
               col_mid_back--;
             // while(!flash_write_buf(SECTOR_NUM,0,sizeof(VAR),VAR));        //扇区127,偏移地址为0,写入数据源地址:buff,数目sizeof(buff)
                site1.x = 40;
                site1.y = 10;     
                LCD_num(site1,col_mid_back,BLUE, RED);
                break;   
                 
        case 5: menu_display();
                row_edge_array[4]-=0.5;
             // while(!flash_write_buf(SECTOR_NUM,0,sizeof(VAR),VAR));        //扇区127,偏移地址为0,写入数据源地址:buff,数目sizeof(buff)
                site1.x = 40;
                site1.y = 30;     
                LCD_num(site1,row_edge_array[4]*10,BLUE, RED);
                break;                  
                 
        case 6: menu_display();
                row_edge_array[6]-=0.5;
             // while(!flash_write_buf(SECTOR_NUM,0,sizeof(VAR),VAR));        //扇区127,偏移地址为0,写入数据源地址:buff,数目sizeof(buff)
                site1.x = 40;
                site1.y = 50;     
                LCD_num(site1,row_edge_array[6]*10,BLUE, RED);
                break; 
        
        case 7: menu_display();
                row_edge_array[8]-=0.5;
             // while(!flash_write_buf(SECTOR_NUM,0,sizeof(VAR),VAR));        //扇区127,偏移地址为0,写入数据源地址:buff,数目sizeof(buff)
                site1.x = 40;
                site1.y = 70;     
                LCD_num(site1,row_edge_array[8]*10,BLUE, RED);
                break;         
                 
                 
                 
      }     
   }
  
   switch (flag)
      {
        case 1: menu_display();
                 site1.x = 110;
                 site1.y = 10;     
                 LCD_num(site1,threshold,BLUE, WHITE);
                 break;
                   
        case 2: menu_display();
                 site1.x = 110;
                 site1.y = 30;     
                 LCD_num(site1, row_cut,BLUE,  WHITE);
                 break;
                   
        case 3: menu_display();
                 site1.x = 110;
                 site1.y = 50;     
                 LCD_num(site1, open_or_not,BLUE,  WHITE);
                 break;    
                 
        case 4: menu_display();
                site1.x = 40;
                site1.y = 10;     
                LCD_num(site1, col_mid_back,BLUE,  WHITE);
                break;   
                 
        case 5: menu_display();
                site1.x = 40;
                site1.y = 30;     
                LCD_num(site1,row_edge_array[4]*10,BLUE,  WHITE);
                break;                  
                 
        case 6: menu_display();
                site1.x = 40;
                site1.y = 50;     
                LCD_num(site1,row_edge_array[6]*10,BLUE, WHITE);
                break; 
        
        case 7: menu_display();
                site1.x = 40;
                site1.y = 70;     
                LCD_num(site1,row_edge_array[8]*10,BLUE,  WHITE);
                break;   
      }

}



