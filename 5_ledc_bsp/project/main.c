#include "main.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"



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
