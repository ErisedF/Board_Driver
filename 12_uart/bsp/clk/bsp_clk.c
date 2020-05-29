#include "bsp_clk.h"

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


/*初始化时钟*/
void imx6ull_Clk_Init(void)
{
	unsigned int reg = 0;
	/*******************************************************
	 * 设置主频
	 ******************************************************/
	/*初始化 主频为528MHz*/
	/*1. 判断当前始终设置状态,并设置为step_clk，step_clk为修改主频时的临时时钟
	*CCM Clock Switcher Register (CCM_CCSR) ->SIEP_SEL  ->PLL1_SW_CLK_SEL
	*/
	if((((CCM -> CCSR) >> 2) & 0x1) == 0)		/*pll1_main_clk*/
	{
		//pll1_main_clk
		//CCSR: step_sel -> 0
		CCM -> CCSR &= ~(1<<8);		//设置为 osc_clk
		//CCSR: pll1_sw_clk_sel -> 1
		CCM -> CCSR |= (1<<2);		//设置为 step_clk
	}

	/*2. 设置主屏时钟PLL1=1056MHz
	*Fout = Fin * div_select/2.0. Fin = 24MHz Fout = 1056MHz -> div_select = 88
	*DIV_SELECT  ENABLE 
	*/
	CCM_ANALOG -> PLL_ARM = ((88 << 0) & 0x7f) | (1 << 13);		//696MHz
	//CCM_ANALOG -> PLL_ARM = ((58 << 0) & 0x7f) | (1 << 13); 

	/*3. 设置 PLL1分频 系数为2
	*18.6.5 CCM Arm Clock Root Register (CCM_CACRR) -> ARM_PODF = 1
	*/
	CCM -> CACRR = 1;
	//CCM -> CACRR = 0;//696MHz

	/*4. 切换主频到 pll1_main_clk*/
			//pll1_main_clk
		//CCSR: step_sel -> 0
		//CCM -> CCSR &= ~(1<<8);		//设置为 osc_clk
		//CCSR: pll1_sw_clk_sel -> 1
		CCM -> CCSR &= ~(1<<2);		//设置为 step_clk

	/*******************************************************
	 * 设置其他PLL的PFD PLL2 PLL3 PLLx_PFDx：12～35
	 ******************************************************/
	/*设置PLL2*/
	reg = CCM_ANALOG -> PFD_528;		//访问寄存器
	reg &= ~(0x3f3f3f3f);		//PFDx_FRAC清零
	reg |= 32 << 24;				/* PLL2_PFD3=528*18/ <32> =297Mhz 	*/
	reg |= 24 << 16;				/* PLL2_PFD2=528*18/ <24> =396Mhz(DDR使用的时钟，最大400Mhz) */
	reg |= 16 << 8;				/* PLL2_PFD1=528*18/ <16> =594Mhz 	*/
	reg |= 27 << 0;				/* PLL2_PFD0=528*18/ <27> =352Mhz  	*/
	CCM_ANALOG -> PFD_528 = reg;

	/*设置PLL3*/
	reg = 0;
	reg = CCM_ANALOG->PFD_480;
	reg &= ~(0X3F3F3F3F);		/* 清除原来的设置 							*/
	reg |= 19 << 24;				/* PLL3_PFD3=480*18/ <19> =454.74Mhz 	*/
	reg |= 17 << 16;				/* PLL3_PFD2=480*18/ <17> =508.24Mhz 	*/
	reg |= 16 << 8;				/* PLL3_PFD1=480*18/ <16> =540Mhz		*/
	reg |= 12 << 0;				/* PLL3_PFD0=480*18/ <12> =720Mhz	 	*/
	CCM_ANALOG->PFD_480=reg;	/* 设置PLL3_PFD0~3 					*/	



	/*******************************************************
	 * 设置其他其他外设时钟源
	 * 1.AHB_CLK_ROOT    132MHz
	 * 2.IPG_CLK_ROOT   	66MHz
	 * 3.PERCLK_CLK_ROOT		66MHz
	 ******************************************************/
	/*1.AHB_CLK_ROOT*/
	/**
	 * CBCMR[PRE_PERIPH_CLK_SEL]  -> 01 derive clock from PLL2 PFD2
	 * CBCDR[PERIPH_CLK_SEL]  -> 0 PLL2 (pll2_main_clk)
	 * CBCDR[AHB_PODF] -> CBCDR[AHB_PODF]
	*/
	//1.1 CBCMR[PRE_PERIPH_CLK_SEL]  -> 01 derive clock from PLL2 PFD2
	CCM -> CBCMR &= ~(3 << 18);		//CBCMR[PRE_PERIPH_CLK_SEL]  清零
	CCM -> CBCMR |=  (1 << 18);		//赋值 1

	//1.2 CBCDR[PERIPH_CLK_SEL]  -> 0 PLL2 (pll2_main_clk)
	CCM -> CBCDR &= ~(1 << 25);		//CBCMR[PRE_PERIPH_CLK_SEL]  清零
	//CBCDR[PERIPH_CLK_SEL] 该位的忙状态握手信号 PERIPH_CLK_SEL_BUSY
	while ((CCM -> CDHIPR) & (1 << 5)); //1  忙

	//1.3 CBCDR[AHB_PODF]  -> 0 PLL2 (pll2_main_clk)
	CCM -> CBCDR &= ~(7 << 10);		//CBCDR[AHB_PODF] 清零
	CCM -> CBCDR |=  (2 << 10);		//CBCDR[AHB_PODF]

	//CBCDR[AHB_PODF] 该位的忙状态握手信号 AHB_PODF_BUSY
	while ((CCM -> CDHIPR) & (1 << 1)); //1  忙
	

	/*2.IPG_CLK_ROOT*/
	/**
	 * CBCDR[IPG_PODF] -> 01 divide by 2
	*/
	CCM -> CBCDR &= ~(3 << 8);		//清零
	CCM -> CBCDR |=  (1 << 8);

	/*3.PERCLK_CLK_ROOT*/
	/**
	 * CSCMR1[PERCLK_PODF] -> 000000 divide by 1
	 * CSCMR1[PERCLK_CLK_SEL] ->0 derive clock from ipg clk root
	*/
	CCM -> CSCMR1 &= ~(0X7F); //即是清理，也是赋值 PERCLK_PODF  PERCLK_CLK_SEL
	uart_clk_init();
}
/**
* @file bsp_clk.c
* @Date: 2020-05-29 19:28:49
* 
* @brief   设置UART_CLK_ROOT 80   80mhz
* 
* @note    derive clock from pll3_80m
* 
* @param   
* 
* @retval  
*/
void uart_clk_init()
{
	CCM ->CSCDR1 &= ~(1<<6);	//18.6.10 CCM Serial Clock Divider Register 1 (CCM_CSCDR1) derive clock from pll3_80m
	CCM ->CSCDR1 &= ~(0x3f);	//000000 divide by 1
}