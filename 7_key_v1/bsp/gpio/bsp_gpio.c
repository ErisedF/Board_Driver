#include "bsp_gpio.h"


/*gpio输出高低电平*/
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

/*gpio读取高低电平*/
int gpio_pin_read(GPIO_Type *base , int pin)
{
    return ((base ->DR) >> pin ) & 0X1;

}
    

/*gpio初始化*/
void GPIO_Init(GPIO_Type *base , int pin , _gpio_pin_congfig_t *config)
{
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
}

    
