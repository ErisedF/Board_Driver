#ifndef __BSP_EXTI_H
#define __BSP_EXTI_H

#include "imx6ul.h"

#include "bsp_beep.h"
#include "bsp_gpio.h"
#include "bsp_int.h"


/*声明*/
void exti_init(void);
void gpio1_io18_irqHandler(unsigned int giccIar, void *param);

#endif