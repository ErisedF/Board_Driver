#ifndef __BSP_EPIT_H
#define __BSP_EPIT_H

#include "imx6ul.h"
#include "bsp_int.h"
#include "bsp_led.h"

void epit_timer_stop(void);
void epit_timer_restart(unsigned int reloadValue);

void epit_init(unsigned int perscalar, unsigned int reloadValue);
void epit_irq_handler(unsigned int giccIar, void *param);

#endif
