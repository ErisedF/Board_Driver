#include "bsp_delay.h"



/***
 * ======================================================
 * 执行空语句实现延时
 * ======================================================
 * 
 */

/**
* @file bsp_delay.c
* @Date: 2020-05-28 17:17:07
* 
* @brief   执行空语句实现延时
* 
* @note    
* 
* @param   
* 
* @retval  
*/
void delay_ms(volatile unsigned int n)
{
	while(n--)
	{

	}
}
void n_delay_ms(volatile unsigned int n)
{
	while(n--)
	{
		delay_ms(0x0800);
	}
}


/***
 * ======================================================
 * GPT 定时器实现
 * ======================================================
 * 
 */

/**
* @file bsp_delay.c
* @Date: 2020-05-28 17:17:37
* 
* @brief   初始化函数	GPT精确延时
* 
* @note    
* 
* @param   
* 
* @retval  
*/
void delay_init(void)
{
	/*30.6.1 GPT Control Register (GPTx_CR)*/
	GPT1 ->CR = 0;			/*清零*/

	/*Software reset.*/
	GPT1 ->CR |= (1 <<15);		//开启软复位 1 GPT is in reset state  0 GPT is not in reset state
	while ((GPT1 ->CR >> 15)&0x1);		//等待完成复位

	GPT1 ->CR = (1  << 1) | (1 << 6) | (0 << 9);		//Peripheral Clock (ipg_clk)=66MHz	GPT counter value is reset to 0 when it is disabled.

	/*30.6.2 GPT Prescaler Register (GPTx_PR)*/
	GPT1 ->PR = 65;		//66分频 66000000/66 = 1000000mhz

	/**
	 * 设置输出比较通达1
	 * 只有通道1 支持Restart mode
	 * 30.6.5 GPT Output Compare Register 1 (GPTx_OCR1)
	 * 设置为最大0xffffffff = 4,294,967,295 us ～71.5h
	*/
	GPT1 ->OCR[0] = 0xffffffff;		//周期为1s

	
	// /*30.6.4 GPT Interrupt Register (GPTx_IR)*/
	// GPT1 ->IR |= (1 << 0);		/*使能 通道1 输出比较中断*/

	// /*使能GIC*/
	// GIC_EnableIRQ(GPT1_IRQn);
	// /*注册中断服务函数*/
	// system_register_irqHandler(GPT1_IRQn, GPT1_IRQn_irqHandler, NULL);  //注册中断

	// /*使能GPT1*/
	GPT1 ->CR |= (1 << 0);
}

/**
* @file bsp_delay.c
* @Date: 2020-05-28 18:51:32
* 
* @brief   us延时
* 
* @note    通过计数器值 计数器值是多少，就是几 us
* 
* @param   
* 
* @retval  
*/
void delay_us(unsigned int us)
{
	unsigned int oldCNT,newCNT;
	unsigned int CNT_Value = 0;

	/*获取当前计数器值*/
	oldCNT = GPT1->CNT;

	while(1)
	{
		newCNT = GPT1->CNT;
		if(newCNT != oldCNT)		
		{
			if(newCNT > oldCNT)		/*新值比旧值 大，说明 没有溢出*/
			{
				CNT_Value += (newCNT - oldCNT);
			}
			else
			{
				CNT_Value += 0xffffffff  - oldCNT + newCNT;
			}

			oldCNT = newCNT;

			if(CNT_Value >= us)
			{
				break;
			}
		}
	}
}
/**
* @file bsp_delay.c
* @Date: 2020-05-28 19:22:33
* 
* @brief   ms延时
* 
* @note    
* 
* @param   
* 
* @retval  
*/
void delay_ms_1(unsigned int ms)
{
	int i = 0;
	for(i = 0 ; i< ms ; i++)
	{
		delay_us( 1000 );
	}
}



// /**
// * @file bsp_delay.c
// * @Date: 2020-05-28 17:56:30
// * 
// * @brief   GPTI中断服务函数
// * 
// * @note    
// * 
// * @param   
// * 
// * @retval  
// */
// void GPT1_IRQn_irqHandler(unsigned int giccIar, void *param)
// {
// 	static unsigned int led_sta = 0;
// 	if((GPT1 ->SR)&(1 << 0))
// 	{
// 		led_sta = !led_sta;
// 		led_switch (0,led_sta);
// 	}

// 	/*清除中断标志位*/
// 	GPT1 ->SR |= (1 << 0);
// }