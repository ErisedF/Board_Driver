#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "cc.h"

/*枚举 IO输入输出状态*/
typedef enum _gpio_pin_direction
{
    KGPIO_Digital_InpUt     =   0U,   
    KGPIO_Digital_Output    =   1U
}gpio_pin_direction_t;

/*结构体 */
typedef struct _gpio_pin_congfig
{
    gpio_pin_direction_t direction;         //方向
    uint8_t outputLogic;
}_gpio_pin_congfig_t;




/*声明*/
void gpio_pin_write(GPIO_Type *base , int pin , int value);
int  gpio_pin_read(GPIO_Type *base , int pin);
void GPIO_Init(GPIO_Type *base , int pin , _gpio_pin_congfig_t *config);


#endif 