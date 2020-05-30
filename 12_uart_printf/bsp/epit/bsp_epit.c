#include "bsp_epit.h"


/**
* @file bsp_exti.c
* @Date: 2020-05-28 11:33:18
* 
* @brief   epit定时器关闭
* 
* @note    
* 
* @param   
* 
* @retval  
*/
void epit_timer_stop(void)
{
    EPIT1 -> CR &= ~(1<<0);
}
/**
* @file bsp_exti.c
* @Date: 2020-05-28 11:36:29
* 
* @brief   epit定时器重启
* 
* @note    先关闭，赋值后再打开
* 
* @param   
* 
* @retval  
*/
void epit_timer_restart(unsigned int reloadValue)
{
    /*先关闭定时器*/
    epit_timer_stop();

    EPIT1 ->LR = reloadValue;   //加载寄存器，计数值

    /*打开*/
    EPIT1 -> CR |= (1<<0);

}


/**
* @file bsp_epit.c
* @Date: 2020-05-27 20:25:25
* 
* @brief   初始化EPIT
* 
* @note    比较值为零，接入的时钟平吕为66MHz
* 
* @param   perscalar        分频系数
* @param   reloadValue      重装载值
* 
* @retval  
*/
void epit_init(unsigned int perscalar, unsigned int reloadValue)
{
    if(perscalar > 4095)        //防止分频系数溢出
        perscalar = 4095;

    /*24.6.1 Control register (EPITx_CR)*/
    EPIT1 ->CR = 0;     //清零
    EPIT1 ->CR = ((1 << 1) | (1 << 2) | (1 << 3) | (perscalar << 4) | (1 << 24));     

    /*24.6.3 Load register (EPITx_LR)*/
    EPIT1 ->LR = reloadValue;   //加载寄存器，技术值

    /*24.6.4 Compare register (EPITx_CMPR)*/
    EPIT1 ->CMPR = 0;       //到达比较值。触发中断

    /*初始化中断*/
    GIC_EnableIRQ(EPIT1_IRQn);      //使能对应中断的 GIC控制器
    system_register_irqHandler(EPIT1_IRQn, epit_irq_handler, NULL);  //注册中断

    /*使能EPIT*/
    EPIT1 ->CR |= (1 <<0);


}


/**
* @file bsp_epit.c
* @Date: 2020-05-27 20:50:16
* 
* @brief   中断服务函数
* 
* @note    
* 
* @param    giccIar         中断号
* @param    handler     中断号对应的中断处理函数  
* 
* @retval  
*/
void epit_irq_handler(unsigned int giccIar, void *param)
{
    static unsigned int led_sta = 0;
    led_sta = !led_sta;
    /*24.6.2 Status register (EPITx_SR)*/
    if((EPIT1->SR)&(1<<0))       //最低位表示中断状态，1表示发生中断
    {
        led_switch (0,led_sta);
    }

    /*清除中断标志位*/
    EPIT1->SR |= (1 <<0);      //写1清零

}