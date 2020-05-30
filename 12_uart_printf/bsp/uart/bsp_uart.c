#include "bsp_uart.h"

/***
 * ======================================================
 * 配置
 * ======================================================
 * 
 */
/***/
/**
* @file bsp_uart.c
* @Date: 2020-05-29 14:31:06
* 
* @brief   初始化UART1
* 
* @note    固定波特率 115200
*          55.15.5 UART Control Register 3 (UARTx_UCR3) this bit should always be set.
* 
* @param   
* 
* @retval  
*/
void uart_init(void)
{
    /*初始化UART的IO*/
    uart_io_init();

    /*关闭uart1*/
    uart_x_disable(UART1);

    /*复位uart1*/
    uart_x_softWareReset(UART1);

    /*清空UCR1*/
    UART1 ->UCR1 = 0;
    UART1 ->UCR1 &= ~(1 << 14);     //关闭自动波特率检测
    /*配置  8位数据位   奇偶校验    停止位*/
    /*55.15.4 UART Control Register 2 (UARTx_UCR2)*/
    //UART1 ->UCR2 = 0x1; 
    UART1 ->UCR2 |= (1 << 1) | (1 << 2) | (1 << 5) | (1 << 14);

    /*55.15.5 UART Control Register 3 (UARTx_UCR3)*/
    UART1 ->UCR3 |= (1 << 2);  //this bit should always be set.

    // /**
    //  * 设置波特率   115200
    // */
    // /*1 设置分频    55.15.7 UART FIFO Control Register (UARTx_UFCR)*/  
    // //UART1 ->UFCR &= ~(7 << 7);  //清零
    // UART1 ->UFCR = 5 << 7;   //101   1分频  80MHZ
    // /*2    UBIR     55.15.12 UART BRM Incremental Register (UARTx_UBIR)*/
    // UART1 ->UBIR = 71;
    // /*3  UBMR     55.15.13 UART BRM Modulator Register (UARTx_UBMR)*/
    // UART1 ->UBMR = 3124;
    uart_setbaudrate(UART1, 115200, 80000000);

    /*使能uart1*/
    uart_x_enable(UART1);
}

/**
* @file bsp_uart.c
* @Date: 2020-05-29 14:32:32
* 
* @brief   UART1 GPIO 初始化
* 
* @note    	 电气数学  0x10b0
*            bit 16:0 HYS关闭
*	         bit [15:14]: 00 默认下拉
*            bit [13]: 0 kepper功能
*            bit [12]: 1 pull/keeper使能
*            bit [11]: 0 关闭开路输出
*            bit [7:6]: 10 速度100Mhz
*            bit [5:3]: 110 R0/6驱动能力
*            bit [0]: 0 低转换率
*    
* 
* @param   
* 
* @retval  
*/
void uart_io_init(void)
{
    //_gpio_pin_congfig_t uart_config; //定义结构体指针
    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX,0);      //TX复用配置
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX,0);      //RX复用配置
    // 0 | 11 | 1 | 1 | 0| 000 | 10 | 000 | 00 | 0
    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX,0x10B0);    //TX电气配置  32.6.163 SW_PAD_CTL_PAD_UART1_TX_DATA SW PAD ControlRegister
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX,0x10B0);    //RX电气配置  32.6.164 SW_PAD_CTL_PAD_UART1_RX_DATA SW PADControl Register
}
/**
* @file bsp_uart.c
* @Date: 2020-05-29 15:00:43
* 
* @brief   关闭uart1
* 
* @note    
* 
* @param   
* 
* @retval  
*/
void uart_x_disable(UART_Type *base)
{
    base->UCR1 &= ~(1 << 0);    //55.15.3 UART Control Register 1 (UARTx_UCR1)
}
/**
* @file bsp_uart.c
* @Date: 2020-05-29 15:05:59
* 
* @brief   打开uart1
* 
* @note    
* 
* @param   
* 
* @retval  
*/
void uart_x_enable(UART_Type *base)
{
    base->UCR1 |= (1 << 0);    //55.15.3 UART Control Register 1 (UARTx_UCR1)
}
/**
* @file bsp_uart.c
* @Date: 2020-05-29 15:07:39
* 
* @brief   复位uart1
* 
* @note    1. Clear the SRST_B bit (UCR2[0])
*          2. Wait for software reset complete: poll SOFTRST bit (UTS[0]) until it is 0.
*          3. Re-program baud rate registers: Re-write UBIR and UBMR.
* 
* @param   
* 
* @retval  
*/
void uart_x_softWareReset(UART_Type *base)
{
    /*Clear the SRST_B bit*/
    base->UCR2 &= ~(1<<0);    //55.15.4 UART Control Register 2 (UARTx_UCR2)
    /*Wait for software reset complete*/
    while((base->UCR2 & 0x1) == 0);       //55.15.16 UART Test Register (UARTx_UTS)
    /*Re-program baud rate*/
}

/*
 * @description 		: 波特率计算公式，
 *    			  	  	  可以用此函数计算出指定串口对应的UFCR，
 * 				          UBIR和UBMR这三个寄存器的值
 * @param - base		: 要计算的串口。
 * @param - baudrate	: 要使用的波特率。
 * @param - srcclock_hz	:串口时钟源频率，单位Hz
 * @return		: 无
 */
void uart_setbaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz)
{
    uint32_t numerator = 0u;		//分子
    uint32_t denominator = 0U;		//分母
    uint32_t divisor = 0U;
    uint32_t refFreqDiv = 0U;
    uint32_t divider = 1U;
    uint64_t baudDiff = 0U;
    uint64_t tempNumerator = 0U;
    uint32_t tempDenominator = 0u;

    /* get the approximately maximum divisor */
    numerator = srcclock_hz;
    denominator = baudrate << 4;
    divisor = 1;

    while (denominator != 0)
    {
        divisor = denominator;
        denominator = numerator % denominator;
        numerator = divisor;
    }

    numerator = srcclock_hz / divisor;
    denominator = (baudrate << 4) / divisor;

    /* numerator ranges from 1 ~ 7 * 64k */
    /* denominator ranges from 1 ~ 64k */
    if ((numerator > (UART_UBIR_INC_MASK * 7)) || (denominator > UART_UBIR_INC_MASK))
    {
        uint32_t m = (numerator - 1) / (UART_UBIR_INC_MASK * 7) + 1;
        uint32_t n = (denominator - 1) / UART_UBIR_INC_MASK + 1;
        uint32_t max = m > n ? m : n;
        numerator /= max;
        denominator /= max;
        if (0 == numerator)
        {
            numerator = 1;
        }
        if (0 == denominator)
        {
            denominator = 1;
        }
    }
    divider = (numerator - 1) / UART_UBIR_INC_MASK + 1;

    switch (divider)
    {
        case 1:
            refFreqDiv = 0x05;
            break;
        case 2:
            refFreqDiv = 0x04;
            break;
        case 3:
            refFreqDiv = 0x03;
            break;
        case 4:
            refFreqDiv = 0x02;
            break;
        case 5:
            refFreqDiv = 0x01;
            break;
        case 6:
            refFreqDiv = 0x00;
            break;
        case 7:
            refFreqDiv = 0x06;
            break;
        default:
            refFreqDiv = 0x05;
            break;
    }
    /* Compare the difference between baudRate_Bps and calculated baud rate.
     * Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1)).
     * baudDiff = (srcClock_Hz/divider)/( 16 * ((numerator / divider)/ denominator).
     */
    tempNumerator = srcclock_hz;
    tempDenominator = (numerator << 4);
    divisor = 1;
    /* get the approximately maximum divisor */
    while (tempDenominator != 0)
    {
        divisor = tempDenominator;
        tempDenominator = tempNumerator % tempDenominator;
        tempNumerator = divisor;
    }
    tempNumerator = srcclock_hz / divisor;
    tempDenominator = (numerator << 4) / divisor;
    baudDiff = (tempNumerator * denominator) / tempDenominator;
    baudDiff = (baudDiff >= baudrate) ? (baudDiff - baudrate) : (baudrate - baudDiff);

    if (baudDiff < (baudrate / 100) * 3)
    {
        base->UFCR &= ~UART_UFCR_RFDIV_MASK;
        base->UFCR |= UART_UFCR_RFDIV(refFreqDiv);
        base->UBIR = UART_UBIR_INC(denominator - 1); //要先写UBIR寄存器，然后在写UBMR寄存器，3592页 
        base->UBMR = UART_UBMR_MOD(numerator / divider - 1);
    }
}

/***
 * ======================================================
 * 发送接受数据
 * ======================================================
 * 
 */
/**
* @file bsp_uart.c
* @Date: 2020-05-29 16:34:34
* 
* @brief   UART1发送一个字符
* 
* @note    
* 
* @param   
* 
* @retval  
*/
void putc(unsigned char c)
{
    while((((UART1 ->USR2)>>3) & (0X01)) == 0);   //判断是否完成发送  55.15.9 UART Status Register 2 (UARTx_USR2)

    UART1 ->UTXD = c & 0xff;   //将发送数据写入寄存器  55.15.9 UART Status Register 2 (UARTx_USR2)

}
/**
* @file bsp_uart.c
* @Date: 2020-05-29 16:52:54
* 
* @brief   UART1接受一个字符
* 
* @note    55.15.9 UART Status Register 2 (UARTx_USR2)
* 
* @param   
* 
* @retval  ((UART1 ->URXD) & 0XFF)； Received Data.
*/
unsigned char getc(void)
{
	while((UART1->USR2 & 0x1) == 0);/* 等待接收完成 */
	return UART1->URXD;				/* 返回接收到的数据 */
}

/**
* @file bsp_uart.c
* @Date: 2020-05-29 16:53:38
* 
* @brief   UART1发送字符串
* 
* @note    
* 
* @param   str  直线字符串的指针
* 
* @retval  
*/
void puts(char *str)
{
    char *p = str;
    while(*p)
    {
        putc(*p++);
    }
}


/***
 * ======================================================
 * 防报错
 * ======================================================
 * 
 */
/*
 * @description : 防止编译器报错
 * @param 		: 无
 * @return		: 无
 */
void raise(int sig_nr) 
{

}

