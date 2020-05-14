#include "bsp_delay.h"


/*延时*/
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