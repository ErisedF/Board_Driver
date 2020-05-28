#include "bsp_gpio.h"

/***
 * ======================================================
 * 输入输出
 * ======================================================
 * 
 */

/**
* @file bsp_gpio.c
* @Date: 2020-05-26 18:02:37
* 
* @brief   gpio输出高低电平
* 
* @note    
* 
* @param   base     基地址
* @param   pin      引脚
* @param   value    写如迎角的值
* 
* @retval  
*/
void gpio_pin_write(GPIO_Type *base , int pin , int value)
{
    if(value == 0)
    {
        base->DR &= ~(1 << pin);    //写0
    }
    else
    {
        base->DR |= (1 << pin);     //写1
    }
    
}
/**
* @file bsp_gpio.c
* @Date: 2020-05-26 18:04:35
* 
* @brief   gpio读取高低电平
* 
* @note    
* 
* @param   base     基地址
* @param   pin      引脚  
* 
* @retval  
*/
int gpio_pin_read(GPIO_Type *base , int pin)
{
    return ((base ->DR) >> pin ) & 0X1;

}
    
/**
* @file bsp_gpio.c
* @Date: 2020-05-26 18:43:00
* 
* @brief   GPIO初始化
* 
* @note    
* 
* @param   base     基地址
* @param   pin      引脚    
* @param   config   引脚配置结构体 
* 
* @retval  
*/
void GPIO_Init(GPIO_Type *base , int pin , _gpio_pin_congfig_t *config)
{
    /*输入输出*/
    if(config->direction == KGPIO_Digital_InpUt)        //输入
    {
        base->GDIR &= ~(1 << pin);
    }
    else
    {
        base->GDIR |= (1 << pin);                       //输出
        /*设置默认输出*/
        gpio_pin_write(base , pin , config->outputLogic);
    }

    /*中断*/
    gpio_int_config(base , pin , config->interruptmode);


}
/***
 * ======================================================
 * 中断
 * ======================================================
 * 
 */


/**
* @file bsp_gpio.c
* @Date: 2020-05-26 18:05:24
* 
* @brief   使能 禁止指定IO中断
* 
* @note    28.5.6 GPIO interrupt mask register (GPIOx_IMR)
* 
* @param   base     基地址      GPIOX
* @param   pin      引脚   
* 
* @retval  
*/
void gpio_int_enable(GPIO_Type *base , int pin)
{
    base->IMR |= (1<<pin);
}
void gpio_int_disable(GPIO_Type *base , int pin)
{
    base->IMR &= ~(1<<pin);
}

/**
* @file bsp_gpio.c
* @Date: 2020-05-26 18:37:08
* 
* @brief   清除中断标志位
* 
* @note    28.5.7 GPIO interrupt status register (GPIOx_ISR)
*           写1清除
* 
* @param   base     基地址      GPIOX
* @param   pin      引脚   
* 
* @retval  
*/
void gpio_clear_intflag(GPIO_Type *base , int pin)
{
    base->ISR |= (1 << pin);
}
/**
* @file bsp_gpio.c
* @Date: 2020-05-26 19:04:07
* 
* @brief   GPIO中断初始化函数
* 
* @note    28.5.5 GPIO interrupt configuration register2 (GPIOx_ICR2)
* 
* @param   base     基地址      GPIOX
* @param   pin      引脚   
* @param   interrupt_mode      结构体类型 中断触发模式 
* 
* @retval  
*/
void gpio_int_config(GPIO_Type *base , int pin ,_gpio_interrupt_mode_t interrupt_mode)
{
    volatile uint32_t *icr;     //用于确定是 icr1 还是 icr2
    uint32_t icr_x_shift;
    icr_x_shift = pin;

    base->EDGE_SEL &= ~(1 << pin);      //必须清零，否着icr会被覆盖

    if(pin < 16)/*低16位IO*/
    {
        icr = &(base->ICR1);
    }
    else
    {
        icr = &(base->ICR2);
        icr_x_shift -= 16;
    }

    switch (interrupt_mode)
    {
    case KGPIO_Int_LowLevel:
        *icr &= (~(3 << (icr_x_shift * 2)));       //    00   每两位设置一个模式 所以*2
        break;
    case KGPIO_Int_HighLevel:
        *icr &= (~(3 << (icr_x_shift * 2)));       //    01  每两位设置一个模式 所以*2   清零
        *icr |=   (1 << (icr_x_shift * 2));
        break; 
    case KGPIO_Int_RisingEdge:
        *icr &= (~(3 << (icr_x_shift * 2)));       //    10  每两位设置一个模式 所以*2   清零
        *icr |=   (2 << (icr_x_shift * 2));  
        break; 
    case KGPIO_Int_FallingEdge:
        *icr &= (~(3 << (icr_x_shift * 2)));       //    11  每两位设置一个模式 所以*2   清零
        *icr |=   (3 << (icr_x_shift * 2));
        break;
    case KGPIO_Int_Rising_OR_FallingEdge:
        base->EDGE_SEL |= (1 << pin);
        break;
    default:
        break;
    }
    
}
