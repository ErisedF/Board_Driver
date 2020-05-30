#include "bsp_exti.h"

/**
* @file exti.c
* @Date: 2020-05-26 21:09:42
* 
* @brief   初始化外部中断
* 
* @note    GPIO1_IO18
* 
* @param   
* 
* @retval  
*/
void exti_init(void)
{
    _gpio_pin_congfig_t key_config; //定义结构体指针
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18,0);      //复用配置
    // 0 | 11 | 1 | 1 | 0| 000 | 10 | 000 | 00 | 0
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18,0XF080);    //电气配置

    key_config.direction = KGPIO_Digital_InpUt; //指针赋值输入
    key_config.interruptmode = KGPIO_Int_FallingEdge;  //下降沿触发
    GPIO_Init(GPIO1 , 18 , &key_config);   //指针
    
    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);   //GIC使能中断

    system_register_irqHandler(GPIO1_Combined_16_31_IRQn, gpio1_io18_irqHandler, NULL);  //注册中断

    gpio_int_enable(GPIO1 , 18);        //GPIO使能中断
    
}

/**
* @file exti.c
* @Date: 2020-05-26 21:34:01
* 
* @brief   GPIO1 IO18的中断处理函数
* 
* @note    一定要清除中断标志位，否则led灯不闪烁
* 
* @param   
* 
* @retval  
*/

void gpio1_io18_irqHandler(unsigned int giccIar, void *param)
{
    static unsigned char beep_sta = 0;

    if(gpio_pin_read(GPIO1,18) == 0)
    {
        beep_sta = !beep_sta;
        beep_switch(beep_sta);
    }
    gpio_clear_intflag(GPIO1 , 18);     //清除中断标志位
}