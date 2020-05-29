#ifndef __BSP_KEY_FILTER_H
#define __BSP_KEY_FILTER_H

#include "imx6ul.h"

#include"bsp_gpio.h"
#include"bsp_int.h"
#include"bsp_epit.h"
#include"bsp_beep.h"
#include"bsp_key.h"


void keyFilter_init(void);
void gpio1_16_31_irqHandler(unsigned int giccIar, void *param);

void filterTimer_init(unsigned int reloadValue);
void epit1_irq_handler(unsigned int giccIar, void *param);



#endif