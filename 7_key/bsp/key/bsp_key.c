#include "bsp_key.h"


/*a按键初始化*/
//0101 ALT5 — Select mux mode: ALT5 mux port: GPIO1_IO18 of instance: gpio1
void key_Init (void)
{
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18,0);      //复用配置
    // 0 | 11 | 1 | 1 | 0| 000 | 10 | 000 | 00 | 0
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18,0XF080);    //电气配置

    GPIO1 -> GDIR &= ~(1<<18);//第18位清0
}

/*读取按键值
*0：按下       1：未按下
*/
int key_Read(void)
{
    int ret;
    ret = ((GPIO1 -> DR) >> 18) & 0X1;
    return ret;
}

/*
*获取按键值
*/
int key_GetValue(void)
{
    int ret;
    static unsigned char release = 1;   //按键释放
    if((release == 1) && (key_Read() == 0))
    {
        n_delay_ms(10);
        release = 0;
        if(key_Read() == 0)     //延时有效
        {
            ret = KEY0_VALUE;
        }
        else
        {
            ret = 0;
        }

    }
    else if(key_Read() == 1)
    {
        ret = KEY_NONE;
        release = 1;
    }

    return ret;
}

