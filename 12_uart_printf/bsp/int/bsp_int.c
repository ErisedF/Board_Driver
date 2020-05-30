#include "bsp_int.h"

static unsigned int irqNesting;     /*中断嵌套计数*/

/**
* @file bsp_int.c
* @Date: 2020-05-26 11:04:31
* 
* @brief   中断处理函数表及其初始化
* 
* @note    元素为结构体
* 
* @param   ('')
* 
* @retval  ('')
*/
static _sys_irq_handle_t irqTable[NUMBER_OF_INT_VECTORS];       /*数组内元素为结构体*/
void system_irqTable_init(void)
{
    unsigned int i = 0;
    irqNesting = 0;     /*嵌套计数清零*/
    for(i = 0; i < NUMBER_OF_INT_VECTORS; i++)
    {
        irqTable[i].irqHandler = default_irqHandler;        /*函数指针指向default_irqHandler函数的地址*/
        irqTable[i].userParam  = NULL;

    }
}

/**
* @file bsp_int.c
* @Date: 2020-05-26 11:21:24
* 
* @brief   默认中断处理函数
* 
* @note    用于给中断函数结构体数组赋初始值
* 
* @param   giccIar      中断号
* @param   userParam    用户参数
* 
* @retval  ('')
*/
void default_irqHandler (unsigned int giccIar, void *userParam)
//system_irq_handler_t default_irqHandler    //不能用
{
    while (1)
    {
        /* code system_irq_handler_t default_irqHandler*/
    }
    
}

/**
* @file bsp_int.c
* @Date: 2020-05-26 13:57:20
* 
* @brief   注册中断处理函数
* 
* @note    用于告诉soc中断信息和链接具体中断函数，并写入函数指针数组中
* 
* @param    irq         中断号
* @param    handler     中断号对应的中断处理函数
* @param    userParam   用户参数
* 
* @retval  
*/
void system_register_irqHandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam)
{
    irqTable[irq].irqHandler = handler;
    irqTable[irq].userParam = userParam;
}

/**
* @file bsp_int.c
* @Date: 2020-05-26 14:10:33
* 
* @brief   具体的中断处理函数
* 
* @note    启动文件中IRQ_Handler，会调用此函数，再次函数中调用函数指针指向的中断函数
* 
* @param   
* 
* @retval  
*/
void system_irqhandler(unsigned int giccIar)
{
    uint32_t intID = giccIar & 0x3ff;    /*bit 0-9 是中断ID*/
    /*判断中断ID是否合理*/
    if(intID >= NUMBER_OF_INT_VECTORS)
    {
        return;
    }

     irqNesting++;      /*进入中断*/

    /*根据中断ID，读取中断处理函数，并执行*/
     irqTable[intID].irqHandler(intID, irqTable[intID].userParam);

     irqNesting--;      /*退出中断*/


}



/**
* @file bsp_int.c
* @Date: 2020-05-27 16:55:04
* 
* @brief   中断初始化
* 
* @note    GIC_Init(); SDK官方函数
*           完成中断指针函数数组初始化
*           中断向量偏移
* 
* @param   
* 
* @retval  
*/
void int_init(void)
{
    GIC_Init(); /*GIC初始化*/
    system_irqTable_init(); /*中断处理函数表及其初始化*/ 
    __set_VBAR(0X87800000); /*设置中断向量偏移量*/
}