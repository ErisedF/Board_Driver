#include "bsp_beep.h"

/*GPIO初始化*/
void beep_init(void)
{
	
	/* 1、初始化IO复用 */
	//V1 SW_MUX_GPIO1_IO03 = 0x5;	/* 复用为GPIO1_IO03 */
	//V2 IOMUX_SW_MUX->GPIO1_IO03 = 0x5;
	IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,0);	


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
	IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,0X10B0);

	/* 3、初始化GPIO 方向 */
	//V1 PIO1_GDIR = 0X0000008;	/* GPIO1_IO03设置为输出 */
	//V2 GPIO1->GDIR = 0X0000008;
	GPIO5->GDIR |= (1 << 1);	
	/* 4、设置GPIO1_IO03输出低电平，打开LED0 */
	//V1 GPIO1_DR = 0X0;
	//V2 GPIO1->DR = 0X0;
	GPIO5->DR |= (1 << 1);	  //  默认高电平 关闭

}

/*开灯关灯*/
void beep_on(void)
{
	/* 
	 * 将GPIO1_DR的bit3清零	 
	 */
	GPIO5->DR &= ~(1 << 1); //on
}

void beep_off(void)
{
	/*    
	 * 将GPIO1_DR的bit3置1
	 */
	GPIO5->DR |= (1 << 1);    //off
}

/*led控制*/
void beep_switch (int status)
{

     if(status == ON)
     {
        beep_on();
     }
     else if(status == OFF)
    {
        beep_off();
	}
   
}