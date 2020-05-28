#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "cc.h"


#include "bsp_gpio.h"

void led_init(void);
void led_on(void);
void led_off(void);
void led_switch (int led_x , int status);

#endif