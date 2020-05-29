#ifndef __BSP_INT_H
#define __BSP_INT_H

#include "imx6ul.h"



/**
 * @brief   定义中断函数形式
 * 
 * @note    复杂函数定义，函数指针system_irq_handler_t
 * 
 * @param   giccIar 中断ID
 * @param   param   传入参数
 * 
 * @retval  none
*/
typedef void (*system_irq_handler_t) (unsigned int giccIar, void *param);   

/**
* @file bsp_int.h
* @date: 2020-05-26 11:01:48
* 
* @brief   ('中断处理函数结构体')
* 
* @note    ('')
* 
* @param   ('')
* 
* @retval  ('')
*/
typedef struct _sys_irq_handle
{
    system_irq_handler_t irqHandler;    /*中断处理函数 ==>*void (*irqHandler) (unsigned int giccIar, void *param); */
    void *userParam;                 /* 中断服务函数参数 */
}_sys_irq_handle_t;



void int_init(void);
void system_irqTable_init(void);
void system_irqhandler(unsigned int giccIar);
void default_irqHandler (unsigned int giccIar, void *userParam);
void system_register_irqHandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam);






#endif