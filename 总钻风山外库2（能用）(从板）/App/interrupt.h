#ifndef INTERRUPT_H_
#define INTERRUPT_H_


extern void PIT1_IRQHandler();
extern void PORTE_IRQHandler();
extern void PORTD_IRQHandler();
extern void PORTA_IRQHandler();
extern void DMA0_IRQHandler();
extern void FTM1_INPUT_IRQHandler();
extern uint8 daocheflag;
extern uint8 imgbuff[CAMERA_SIZE];
extern uint8 duzhuan_light_flag;
extern void UART2_IRQhandler();
//extern void UART5_IRQhandler();
extern void UART4_IRQhandler();
extern  uint8 a;
extern  uint8 b;
extern  uint8 c;
extern uint8 barrier_flag;
extern uint8 barrier_right_flag;
extern void UART0_IRQhandler();
#endif