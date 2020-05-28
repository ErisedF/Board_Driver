#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

#include "imx6ul.h"
#include "bsp_int.h"
#include "bsp_led.h"


void n_delay_ms(volatile unsigned int n);


void delay_init(void);
void GPT1_IRQn_irqHandler(unsigned int giccIar, void *param);

#endif 