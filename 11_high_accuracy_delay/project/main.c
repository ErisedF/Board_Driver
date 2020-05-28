#include "imx6ul.h"

#include "bsp_clk.h"
#include "bsp_gpio.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_exti.h"
#include "bsp_epit.h"
#include"bsp_key_filter.h"



/*gcc要求mian函数必须为int*/
int main(void) 
{
    //int key_value;
    //int i = 0;
    //unsigned char led_state = OFF;
    //unsigned char beep_state = OFF;
    /*初始化*/
    int_init();
    delay_init();
    imx6ull_Clk_Init();
    clk_enable();
    led_init();
    beep_init();
    key_Init();
    //epit_init(0,66000000/2); /*500ms*/
    keyFilter_init();



    led_on();
    beep_on();
    n_delay_ms(1000);


    led_off();
    beep_off();
    n_delay_ms(1000);
 

    while (1)
    { 
       
        // key_value  = key_GetValue();
        // if(key_value);
        // {
        //     switch (key_value)
        //     {
        //     case KEY0_VALUE:
        //         beep_state = !beep_state;
        //         beep_switch (beep_state);
        //         break;
        //     }
        // }

    //      i++;

    //     /*判断程序是否执行*/
    //      if(i == 50)
    //      {
    //          i = 0;
    //          led_state = !led_state;
    //          led_switch (0,led_state);
    //      }
    //      n_delay_ms(10);
     }
       
    return 0;
}

