#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

#include "imx6ul.h"
#include "bsp_int.h"
#include "bsp_led.h"
/*空函数*/
void delay_ms(volatile unsigned int n);
void n_delay_ms(volatile unsigned int n);

/*定时器*/
void delay_init(void);
void delay_us(unsigned int us);
void delay_ms_1(unsigned int ms);
//void GPT1_IRQn_irqHandler(unsigned int giccIar, void *param);

#endif 