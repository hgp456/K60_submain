#ifndef __LIDAR_H
#define __LIDAR_H

#include "stdio.h"	
//#include "sys.h" 
#include "common.h"
#include "include.h"
extern uint8 lidar_buffer[5];
extern uint16 lidar_data[3];
extern uint16 _sum;
extern uint8 data;
void Data_Receive_Prepar();
void Rec_Test(uint8 data);
void Data_Receive_();
void data_pro(uint8 *data,uint8 num);

//DMA
extern void uart5_rx_dma_init (uint32 baud);
extern void dma_uart5_init(DMA_CHn CHn, void *DADDR, uint32 count, uint32 cfg);
extern void DMA10_IRQHandler(void);
extern void DMA_Error_IRQHandler(void);
extern void DMA_init();
#define DMA_CNT     3600
extern uint8   DMA_BUFF[DMA_CNT];

#endif