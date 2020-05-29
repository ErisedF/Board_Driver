#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "imx6ul.h"


#include "bsp_gpio.h"

/*设置*/
void uart_init(void);
void uart_io_init(void);
void uart_x_disable(UART_Type *base);
void uart_x_enable(UART_Type *base);
void uart_x_softWareReset(UART_Type *base);

/*发送接受数据*/
void transmit_char(unsigned char c);
unsigned char receive_char(void);
void transmit_string(char *str);


#endif
