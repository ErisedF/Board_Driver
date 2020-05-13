#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "cc.h"

#include "bsp_delay.h"

/*按键值*/
enum keyvalue { KEY_NONE = 0 , KEY0_VALUE = 1};


/*函数声明*/
void key_Init (void);
int key_Read(void);
int key_GetValue(void);

#endif