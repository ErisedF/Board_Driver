#include"bsp_key_filter.h"


/**
* @file bsp_key_filter.c
* @Date: 2020-05-28 10:55:13
* 
* @brief   按键初始化 GPIO设置
* 
* @note    0101 ALT5 — Select mux mode: ALT5 mux port: GPIO1_IO18 of instance: gpio1
*           GPIO1_IO18配置为中断输入
* 
* @param   
* 
* @retval  
*/
void keyFilter_init(void)
{
    _gpio_pin_congfig_t key_config; //定义结构体指针
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18,0);      //复用配置
    // 0 | 11 | 1 | 1 | 0| 000 | 10 | 000 | 00 | 0
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18,0XF080);    //电气配置

    key_config.direction = KGPIO_Digital_InpUt; //指针赋值输入
    key_config.interruptmode = KGPIO_Int_FallingEdge;  //下降沿触发
    GPIO_Init(GPIO1 , 18 , &key_config);   //指针
    
    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);   //GIC使能中断

    system_register_irqHandler(GPIO1_Combined_16_31_IRQn, gpio1_16_31_irqHandler, NULL);  //注册中断

    gpio_int_enable(GPIO1 , 18);        //GPIO使能中断

    filterTimer_init( 660000 ); //10ms
    
}
/**
* @file bsp_key_filter.c
* @Date: 2020-05-28 11:08:47
* 
* @brief   按键滤波定时器初始化
* 
* @note    epit定时器初始化
* 
* @param   
* 
* @retval  
*/
void filterTimer_init(unsigned int reloadValue)
{
    /*24.6.1 Control register (EPITx_CR)*/
    EPIT1 ->CR = 0;     //清零
    EPIT1 ->CR = ((1 << 1) | (1 << 2) | (1 << 3) | (0 << 4) | (1 << 24));     

    /*24.6.3 Load register (EPITx_LR)*/
    EPIT1 ->LR = reloadValue;   //加载寄存器，技术值

    /*24.6.4 Compare register (EPITx_CMPR)*/
    EPIT1 ->CMPR = 0;       //到达比较值。触发中断

    /*初始化中断*/
    GIC_EnableIRQ(EPIT1_IRQn);      //使能对应中断的 GIC控制器
    system_register_irqHandler(EPIT1_IRQn, epit1_irq_handler, NULL);  //注册中断

    /*使能EPIT*/
    //EPIT1 ->CR |= (1 <<0);
}

/**
* @file bsp_key_filter.c
* @Date: 2020-05-28 11:25:33
* 
* @brief   EPIT1定时器中断处理函数
* 
* @note    
* 
* @param   
* 
* @retval  
*/
void epit1_irq_handler(unsigned int giccIar, void *param)
{
    static unsigned char beep_state = OFF;

    if(EPIT1->SR & (1<<0))
    {
        /*关闭定时器*/
        epit_timer_stop();
        if(key_Read() == 0)
        {
            beep_state = !beep_state;
            beep_switch (beep_state);
        }
    }
    /*清除中断标志位*/
    EPIT1->SR |= (1<<0);
}


/**
* @file bsp_key_filter.c
* @Date: 2020-05-28 11:00:26
* 
* @brief   按键中断函数
* 
* @note    进入按键中断后，开启定时器中断
* 
* @param   
* 
* @retval  
*/
void gpio1_16_31_irqHandler(unsigned int giccIar, void *param)
{
    /*开启定时器*/
    epit_timer_restart(660000);


    /*清除中断标志位*/
    gpio_clear_intflag(GPIO1 , 18);     //清除中断标志位
}

