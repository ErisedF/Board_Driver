#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"

/*初始化时钟*/
void clk_enable(void)
{
	CCM->CCGR0 = 0xffffffff;
	CCM->CCGR1 = 0xffffffff;
	CCM->CCGR2 = 0xffffffff;
	CCM->CCGR3 = 0xffffffff;
	CCM->CCGR4 = 0xffffffff;
	CCM->CCGR5 = 0xffffffff;
	CCM->CCGR6 = 0xffffffff;
}

/*GPIO初始化*/
void led_init(void)
{
	
	/* 1、初始化IO复用 */
	//V1 SW_MUX_GPIO1_IO03 = 0x5;	/* 复用为GPIO1_IO03 */
	//V2 IOMUX_SW_MUX->GPIO1_IO03 = 0x5;
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03,0);


	/* 2、、配置GPIO1_IO03的IO属性	
	 *bit 16:0 HYS关闭
	 *bit [15:14]: 00 默认下拉
     *bit [13]: 0 kepper功能
     *bit [12]: 1 pull/keeper使能
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度100Mhz
     *bit [5:3]: 110 R0/6驱动能力
     *bit [0]: 0 低转换率
     */
	//V1 SW_PAD_GPIO1_IO03 = 0X10B0;	
	//V2 IOMUX_SW_PAD->GPIO1_IO03 = 	0X10B0;
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03,0X10B0);

	/* 3、初始化GPIO 方向 */
	//V1 PIO1_GDIR = 0X0000008;	/* GPIO1_IO03设置为输出 */
	//V2 GPIO1->GDIR = 0X0000008;
	GPIO1->GDIR = 0X0000008;
	/* 4、设置GPIO1_IO03输出低电平，打开LED0 */
	//V1 GPIO1_DR = 0X0;
	//V2 GPIO1->DR = 0X0;
	GPIO1->DR = 0X0;

}

/*开灯关灯*/
void led_on(void)
{
	/* 
	 * 将GPIO1_DR的bit3清零	 
	 */
	GPIO1->DR &= ~(1<<3); 
}
void led_off(void)
{
	/*    
	 * 将GPIO1_DR的bit3置1
	 */
	GPIO1->DR |= (1<<3);
}
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

/*gcc要求mian函数必须为int*/
int main(void) 
{
    /*初始化*/
    clk_enable();
    led_init();

    /*led亮*/

    while (1)
    { 
        led_on();
		n_delay_ms(1000);
		led_off();
		n_delay_ms(1000);
    }
       
    return 0;
}