#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "cc.h"

/*枚举 中断触发类型*/
typedef enum _gpio_interrupt_mode
{
    KGPIO_IntNo_Mode                    =   0U,     //无中断触发
    KGPIO_Int_LowLevel                  =   1U,     //低电平处罚
    KGPIO_Int_HighLevel                 =   2U,     //高电平触发
    KGPIO_Int_RisingEdge                =   3U,     //上升沿触发
    KGPIO_Int_FallingEdge               =   4U,     //下降沿触发
    KGPIO_Int_Rising_OR_FallingEdge     =   5U      //上升或下降沿触发
}_gpio_interrupt_mode_t;


/*枚举 IO输入输出状态*/
typedef enum _gpio_pin_direction
{
    KGPIO_Digital_InpUt     =   0U,   
    KGPIO_Digital_Output    =   1U
}gpio_pin_direction_t;


/**
* @file bsp_gpio.h
* @Date: 2020-05-26 17:54:01
* @defgroup     gpio的引脚配置结构体
*/
typedef struct _gpio_pin_congfig
{
    /*输入输出*/
    gpio_pin_direction_t direction;         //方向
    uint8_t outputLogic;                    //默认输出值

    /*中断*/
    _gpio_interrupt_mode_t interruptmode;


}_gpio_pin_congfig_t;




/*声明*/
/*中断*/
void gpio_int_enable(GPIO_Type *base , int pin);
void gpio_int_disable(GPIO_Type *base , int pin);
void gpio_clear_intflag(GPIO_Type *base , int pin);
void gpio_int_config(GPIO_Type *base , int pin ,_gpio_interrupt_mode_t interrupt_mode);


/*输入输出*/
void gpio_pin_write(GPIO_Type *base , int pin , int value);
int  gpio_pin_read(GPIO_Type *base , int pin);
void GPIO_Init(GPIO_Type *base , int pin , _gpio_pin_congfig_t *config);


#endif 